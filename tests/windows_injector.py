#!/usr/bin/env python3
"""Exercise the native loader on disposable fixtures in a Windows developer prompt."""
import argparse
import os
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument("injector", type=Path)
args = parser.parse_args()
if os.name != "nt":
    raise SystemExit("Windows is required for the injector integration test")
injector = args.injector.resolve(strict=True)
with tempfile.TemporaryDirectory(prefix="cocaine-loader-") as temporary:
    directory = Path(temporary)
    host_source = directory / "host.cpp"
    dll_source = directory / "fixture.cpp"
    host_source.write_text('#include <windows.h>\n#include <cstdio>\nint main() { std::puts("ready"); std::fflush(stdout); Sleep(60000); }\n')
    dll_source.write_text('#include <windows.h>\nBOOL WINAPI DllMain(HMODULE, DWORD, LPVOID) { return TRUE; }\n')
    subprocess.run(["cl", "/nologo", "/EHsc", str(host_source), "/Fe:host.exe"], cwd=directory, check=True)
    subprocess.run(["cl", "/nologo", "/EHsc", "/LD", str(dll_source), "/Fe:fixture.dll"], cwd=directory, check=True)
    fixture = directory / "fixture.dll"
    # Non-ASCII DLL paths exercise the wide-character path end to end.
    dll = directory / "fixture-\u00e9.dll"
    fixture.rename(dll)
    host = subprocess.Popen([str(directory / "host.exe")], stdout=subprocess.PIPE, text=True)
    try:
        assert host.stdout.readline().strip() == "ready", "Fixture did not reach main"
        first = subprocess.run([str(injector), str(host.pid), str(dll)], capture_output=True, text=True, timeout=25)
        assert first.returncode == 0 and "DLL laded." in first.stdout, (first.stdout, first.stderr)
        second = subprocess.run([str(injector), str(host.pid), str(dll)], capture_output=True, text=True, timeout=25)
        assert second.returncode == 0 and "awready laded" in second.stdout, (second.stdout, second.stderr)
        invalid = subprocess.run([str(injector), str(host.pid), str(host_source)], capture_output=True, text=True, timeout=25)
        assert invalid.returncode == 1, (invalid.stdout, invalid.stderr)
        assert host.poll() is None, "Fixture host exited unexpectedly"
    finally:
        host.terminate()
        host.wait(timeout=10)
print("Native injector load, duplicate-load and malformed-image checks passed.")
