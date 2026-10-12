#!/usr/bin/env python3
"""Package the actual Windows build; never publish a bundle missing its DLL."""
import argparse
from pathlib import Path
import struct
from zipfile import ZIP_DEFLATED, ZipFile

parser = argparse.ArgumentParser()
parser.add_argument("--configuration", choices=("Debug", "Release"), default="Release")
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
configuration = args.configuration
files = [(root / "output" / configuration / "Cocaine.dll", "x86/Cocaine.dll", 0x14C, True)]
for platform, folder, machine in (("Win32", "x86", 0x14C), ("x64", "x64", 0x8664)):
    for name in ("CocaineLauncher.exe", "CocaineInjector.exe"):
        files.append((root / "output" / platform / configuration / name, f"{folder}/{name}", machine, False))

# Validate every input before creating any distributable, including the PE DLL flag.
for path, _, machine, is_dll in files:
    data = path.read_bytes()
    assert data[:2] == b"MZ", f"Invalid DOS header: {path}"
    offset = struct.unpack_from("<I", data, 0x3C)[0]
    assert data[offset:offset + 4] == b"PE\0\0", f"Invalid PE header: {path}"
    assert struct.unpack_from("<H", data, offset + 4)[0] == machine, f"Wrong architecture: {path}"
    flags = struct.unpack_from("<H", data, offset + 22)[0]
    assert bool(flags & 0x2000) == is_dll, f"Wrong image type: {path}"

destination = root / "dist" / f"Cocaine-Windows-{configuration}.zip"
destination.parent.mkdir(exist_ok=True)
with ZipFile(destination, "w", ZIP_DEFLATED) as archive:
    for path, name, _, _ in files:
        archive.write(path, name)
    archive.write(root / "docs" / "DOWNLOADS.md", "README.md")
    archive.write(root / "docs" / "INJECTOR.md", "docs/INJECTOR.md")
    archive.write(root / "docs" / "COMPATIBILITY.md", "docs/COMPATIBILITY.md")
    archive.write(root / "docs" / "UPSTREAM.md", "docs/UPSTREAM.md")
    archive.write(root / "assets" / "fonts" / "OFL.txt", "licences/fonts-OFL.txt")
print(destination)
