#!/usr/bin/env python3
"""Smoke-check the native launcher on a Windows CI desktop."""
import ctypes
import subprocess
import sys
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

child = subprocess.Popen([str(binary)])
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
    for control_id in (101, 102, 103, 104, 105):
        assert user32.GetDlgItem(window, control_id), f"Missing control {control_id}"
    # The empty-selection path must leave the GUI responsive and avoid an injector run.
    user32.SendMessageW(window, 0x0111, 105, 0)  # WM_COMMAND, BN_CLICKED
    assert child.poll() is None
    user32.SendMessageW(window, 0x0010, 0, 0)  # WM_CLOSE
    assert child.wait(timeout=5) == 0
finally:
    if child.poll() is None:
        child.terminate()
        child.wait(timeout=5)
print("Windows launcher opened, exposed all controls, and closed cleanly.")
