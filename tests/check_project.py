#!/usr/bin/env python3
"""Check source inventory, project references and active program branding."""
from pathlib import Path
import re
import xml.etree.ElementTree as ET

root = Path(__file__).resolve().parents[1]
ns = {"m": "http://schemas.microsoft.com/developer/msbuild/2003"}
project = ET.parse(root / "Cocaine.vcxproj")
filters = ET.parse(root / "Cocaine.vcxproj.filters")
for kind, suffix in (("ClCompile", ".cpp"), ("ClInclude", ".h")):
    included = {item.attrib["Include"] for item in project.findall(f".//m:{kind}[@Include]", ns)}
    filtered = {item.attrib["Include"] for item in filters.findall(f".//m:{kind}[@Include]", ns)}
    actual = {p.name for p in root.glob(f"*{suffix}")}
    assert included == actual, (kind, included ^ actual)
    assert included == filtered, (kind, included ^ filtered)
    for name in included:
        assert (root / name).is_file(), name
signature_count = 0
for path in list(root.glob("*.cpp")) + list(root.glob("*.h")):
    for include in re.findall(r'^#include "([^"\n]+)"', path.read_text(encoding="latin-1"), flags=re.M):
        assert (root / include).is_file(), (path.name, include)
    assert not re.search(r"csgocheat", path.read_text(encoding="latin-1"), re.I), path.name
    for match in re.finditer(r'pattern::(?:find|FindAll|find_unique|require)\([^;]*?"([^"\n]*)"', path.read_text(encoding="latin-1")):
        tokens = match[1].split()
        assert tokens and all(re.fullmatch(r"(?:[0-9A-Fa-f]{2}|\?\??)", token) for token in tokens), (path.name, match[1])
        signature_count += 1
# Mandatory startup signatures must use the failure-reporting path before hooks are installed.
startup = (root / "csgo.cpp").read_text().split("// some weird tier0 stuff", 1)[0]
assert "pattern::require(" in startup
assert "pattern::find(" not in startup
solution = (root / "Cocaine.sln").read_text(encoding="utf-8-sig")
assert '"Cocaine", "Cocaine.vcxproj"' in solution
assert project.find("m:PropertyGroup/m:ProjectName", ns).text == "Cocaine"
assert project.find("m:PropertyGroup/m:RootNamespace", ns).text == "Cocaine"
assert all(item.text == "Precise" for item in project.findall(".//m:FloatingPointModel", ns))
print("Cocaine source inventory, project references and branding passed;", signature_count, "literal signatures validated.")

# Resource assets must be shipped and registered in both project inventories.
assert project.find(".//m:ResourceCompile[@Include='fonts.rc']", ns) is not None
assert filters.find(".//m:ResourceCompile[@Include='fonts.rc']", ns) is not None
for name in ["IBMPlexSans-Regular.ttf", "IBMPlexSans-Medium.ttf", "OFL.txt"]:
    assert (root / "assets" / "fonts" / name).is_file()
loader = ET.parse(root / "injector" / "CocaineInjector.vcxproj")
for kind in ["ClCompile", "ClInclude"]:
    for item in loader.findall(f".//m:{kind}[@Include]", ns):
        assert (root / "injector" / item.attrib["Include"]).is_file()
assert {i.attrib["Include"] for i in loader.findall(".//m:ProjectConfiguration", ns)} == {
    "Debug|Win32", "Release|Win32", "Debug|x64", "Release|x64"}
assert '"CocaineInjector", "injector\\CocaineInjector.vcxproj"' in solution
launcher = ET.parse(root / "launcher" / "CocaineLauncher.vcxproj")
assert {i.attrib["Include"] for i in launcher.findall(".//m:ProjectConfiguration", ns)} == {
    "Debug|Win32", "Release|Win32", "Debug|x64", "Release|x64"}
assert (root / "launcher" / "main.cpp").is_file()
assert launcher.find(".//m:ClCompile[@Include='main.cpp']", ns) is not None
assert launcher.find(".//m:Link/m:SubSystem", ns).text == "Windows"
assert '"CocaineLauncher", "launcher\\CocaineLauncher.vcxproj"' in solution
launcher_guid = "{D4C36C26-615B-4B27-88B0-96EE187052D1}"
for configuration in ("Debug", "Release"):
    for mapping in ("ActiveCfg", "Build.0"):
        assert f"{launcher_guid}.{configuration}|x86.{mapping} = {configuration}|Win32" in solution
print("Bundled fonts, injector, and launcher project inventories passed.")
