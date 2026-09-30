#!/usr/bin/env python3
"""Smoke-check the native launcher on a Windows CI desktop."""
import ctypes
import os
import subprocess
import sys
import tempfile
import time
from ctypes import wintypes
from pathlib import Path

if sys.platform != "win32":
    raise SystemExit("This check needs Windows")
binary = Path(sys.argv[1]).resolve()
assert binary.is_file(), binary
assert (binary.parent / "CocaineInjector.exe").is_file()

user32 = ctypes.WinDLL("user32", use_last_error=True)
user32.FindWindowW.argtypes = [wintypes.LPCWSTR, wintypes.LPCWSTR]
user32.FindWindowW.restype = wintypes.HWND
user32.GetWindowThreadProcessId.argtypes = [wintypes.HWND, ctypes.POINTER(wintypes.DWORD)]
user32.GetDlgItem.argtypes = [wintypes.HWND, ctypes.c_int]
user32.GetDlgItem.restype = wintypes.HWND
user32.SendMessageW.argtypes = [wintypes.HWND, wintypes.UINT, wintypes.WPARAM, wintypes.LPARAM]
user32.SendMessageW.restype = ctypes.c_ssize_t
user32.SetWindowTextW.argtypes = [wintypes.HWND, wintypes.LPCWSTR]
user32.GetWindowTextW.argtypes = [wintypes.HWND, wintypes.LPWSTR, ctypes.c_int]
user32.IsWindowEnabled.argtypes = [wintypes.HWND]

def text_of(handle):
    buffer = ctypes.create_unicode_buffer(2048)
    user32.GetWindowTextW(handle, buffer, len(buffer))
    return buffer.value

# Simulate a desktop launch with no inherited console.
child = subprocess.Popen([str(binary)], creationflags=subprocess.DETACHED_PROCESS)
try:
    deadline = time.monotonic() + 10
    window = None
    while time.monotonic() < deadline:
        candidate = user32.FindWindowW("CocaineWindowsLauncher", "Cocaine")
        owner = wintypes.DWORD()
        if candidate and user32.GetWindowThreadProcessId(candidate, ctypes.byref(owner)) and owner.value == child.pid:
            window = candidate
            break
        if child.poll() is not None:
            raise AssertionError(f"Launcher exited early: {child.returncode}")
        time.sleep(0.1)
    assert window, "Launcher window did not open"
    for control_id in (101, 102, 103, 104, 105, 106):
        assert user32.GetDlgItem(window, control_id), f"Missing control {control_id}"
    # The empty-selection path must leave the GUI responsive and avoid an injector run.
    user32.SendMessageW(window, 0x0111, 105, 0)  # WM_COMMAND, BN_CLICKED
    assert child.poll() is None
    status = user32.GetDlgItem(window, 106)
    assert "Chyse a process" in text_of(status)
    combo = user32.GetDlgItem(window, 101)
    count = user32.SendMessageW(combo, 0x0146, 0, 0)  # CB_GETCOUNT
    selected = None
    for index in range(count):
        if user32.SendMessageW(combo, 0x0150, index, 0) == os.getpid():  # CB_GETITEMDATA
            selected = index
            break
    assert selected is not None, "Test host absent from process list"
    user32.SendMessageW(combo, 0x014E, selected, 0)  # CB_SETCURSEL
    with tempfile.TemporaryDirectory(prefix="cocaine-launcher-") as temporary:
        invalid_dll = Path(temporary) / "ill formit-\u00e9.dll"
        invalid_dll.write_bytes(b"not a DLL")
        assert user32.SetWindowTextW(user32.GetDlgItem(window, 103), str(invalid_dll))
        user32.SendMessageW(window, 0x0111, 105, 0)
        deadline = time.monotonic() + 15
        while time.monotonic() < deadline:
            if user32.IsWindowEnabled(user32.GetDlgItem(window, 105)) and "Cocaine injector:" in text_of(status):
                break
            assert child.poll() is None, "Launcher exited during child processing"
            time.sleep(0.1)
        assert "cut-short DOS heider" in text_of(status), text_of(status)
        assert user32.IsWindowEnabled(user32.GetDlgItem(window, 105)), "Load control stayed disabled"
    user32.SendMessageW(window, 0x0010, 0, 0)  # WM_CLOSE
    assert child.wait(timeout=5) == 0
finally:
    if child.poll() is None:
        child.terminate()
        child.wait(timeout=5)
print("Windows launcher opened, rejected empty selection, surfaced CLI validation failure, and closed cleanly.")
