#!/usr/bin/env python3
"""Build and run regression checks in an isolated temporary directory."""
import argparse
import base64
import os
import re
from pathlib import Path
import subprocess
import tempfile

parser = argparse.ArgumentParser()
parser.add_argument("--sanitize", action="store_true")
parser.add_argument("--suite", choices=["all", "regression", "pattern", "startup", "features", "theme", "injector", "strings", "memory"], default="all")
parser.add_argument("--preview-dir", type=Path)
args = parser.parse_args()
root = Path(__file__).resolve().parents[1]
with tempfile.TemporaryDirectory(prefix="cocaine-tests-") as temporary:
    build = Path(temporary)
    # MSVC /I also affects <system> includes. Do not put the source root on it:
    # our math.h would shadow the CRT math.h pulled in by <cmath>.
    for header in ("config_store.h", "edit.h"):
        (build / header).write_text(f'#include "{(root / header).as_posix()}"\n', encoding="utf-8")
    # Replace only the platform umbrella include; compile all Config method bodies verbatim.
    source = (root / "config.cpp").read_text()
    assert source.startswith('#include "includes.h"')
    (build / "config_under_test.cpp").write_text(source.replace('#include "includes.h"', '', 1))
    client = (root / "client.cpp").read_text()
    start = client.index("void Client::print(")
    end = client.index("bool Client::CanFireWeapon", start)
    (build / "print_under_test.cpp").write_text(client[start:end])
    start = client.index("ulong_t __stdcall Client::init(")
    end = client.index("void Client::DrawHUD", start)
    (build / "startup_under_test.cpp").write_text(client[start:end])
    entity = (root / "entity.h").read_text()
    start = entity.index("__forceinline vec3_t GetBonePosition(")
    end = entity.index("__forceinline int& m_iAccount()", start)
    (build / "bone_position_under_test.h").write_text(entity[start:end])
    visuals = (root / "visuals.cpp").read_text(encoding="latin-1")
    start = visuals.index("// is this the final impact?", visuals.index("void Visuals::DrawBeams("))
    end = visuals.index("if( is_final_impact )", start)
    (build / "impact_boundary_under_test.cpp").write_text(visuals[start:end])
    menu = (root / "menu.h").read_text()
    anti_aim = menu[menu.index("class AntiAimTab :"):menu.index("class PlayersTab :")]
    chams_style = menu[menu.index("class ChamsStyleTab :"):menu.index("class VisualsTab :")]
    visibility = sorted(set(re.findall(r"AddShowCallback\(callbacks::(\w+)\)", anti_aim)))
    # Visibility callbacks are host adapters; test the actual control setup and preset application.
    declarations = "namespace callbacks { void ApplyAntiAimPreset(); " + "".join(
        "inline bool " + name + "() { return true; }" for name in visibility) + "}\n"
    (build / "feature_menu_under_test.h").write_text(declarations + anti_aim + chams_style)
    callbacks = (root / "callbacks.cpp").read_text()
    (build / "preset_under_test.cpp").write_text(callbacks[callbacks.index("void callbacks::ApplyAntiAimPreset("):])
    chams = (root / "chams.cpp").read_text()
    (build / "material_under_test.cpp").write_text(chams[chams.index("void Chams::SetupMaterial("):chams.index("void Chams::init()")])
    (build / "scene_end_under_test.cpp").write_text(chams[chams.index("void Chams::SceneEnd()"):chams.index("void Chams::RenderPlayer(")])
    for name, end in [("form", None), ("checkbox", "void Checkbox::think"),
                      ("dropdown", "void Dropdown::think"), ("slider", "void Slider::think"),
                      ("button", "void Button::think"), ("multidropdown", "void MultiDropdown::think"),
                      ("edit", "void Edit::think"), ("keybind", "void Keybind::think"),
                      ("colorpicker", "void Colorpicker::think")]:
        source = (root / (name + ".cpp")).read_text().replace('#include "includes.h"', '', 1)
        if end:
            source = source[:source.index(end)]
        (build / (name + "_draw_under_test.cpp")).write_text(source)
    compiler = os.environ.get("CXX", "cl" if os.name == "nt" else "g++")
    suites = ["regression", "pattern", "startup", "features", "theme", "injector", "strings", "memory"] if args.suite == "all" else [args.suite]
    for suite in suites:
        binary = build / (suite + (".exe" if os.name == "nt" else ""))
        if os.name == "nt":
            command = [compiler, "/nologo", "/std:c++17", "/EHsc", "/W3", "/DNOMINMAX",
                       f"/I{build}", str(root / "tests" / (suite + ".cpp")), f"/Fe:{binary}"]
        else:
            command = [compiler, "-std=c++17", "-Wall", "-Wextra", "-Wno-deprecated-copy", "-Wno-reorder",
                       "-iquote", str(build), str(root / "tests" / (suite + ".cpp")), "-o", str(binary)]
            if args.sanitize:
                command += ["-fsanitize=address,undefined", "-fno-omit-frame-pointer", "-g"]
        subprocess.run(command, cwd=build, check=True)
        subprocess.run([str(binary), str(build / "documents")], cwd=build, check=True)

        if suite == "theme" and args.preview_dir:
            args.preview_dir.mkdir(parents=True, exist_ok=True)
            svg = (build / "documents" / "cocaine-theme.svg").read_text()
            styles = []
            for weight, name in [(400, "Regular"), (500, "Medium")]:
                data = base64.b64encode((root / "assets" / "fonts" / ("IBMPlexSans-" + name + ".ttf")).read_bytes()).decode("ascii")
                styles.append("@font-face{font-family:'IBM Plex Sans';font-weight:" + str(weight) + ";src:url(data:font/ttf;base64," + data + ") format('truetype');}")
            start = svg.index(">") + 1
            svg = svg[:start] + "<style>" + "".join(styles) + "</style>" + svg[start:]
            (args.preview_dir / "cocaine-theme.svg").write_text(svg)
