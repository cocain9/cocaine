# Loader comparison — 2026-09-30

Reference: [Potato-Injector at 5b619a0](https://github.com/leo4048111/Potato-Injector/tree/5b619a02ba4cd04f6688ee539a3468ee3479ed39). This is a source inspection, nae Windows benchmark. Nae reference code wis copied. A blanket claim that Cocaine is better isna justified by the checks available.

| Area | Cocaine | Potato source inspected |
| --- | --- | --- |
| Architecture | Sindry x86 an x64 tools; DLL, injector an target architecture checked | Wrapper accepts x64 images an targets |
| Target choice | Explicit PID frae the process picker | Process-name lookup, wi custom names |
| Loading | Windows `LoadLibraryW` | BlackBone `MapImage` |
| Dependencies | Windows system libraries an C++ runtime | BlackBone, ImGui an Direct3D 9 |
| PE validation | Executable/DLL flags, architecture/magic, section-table bounds, image/header sizes, entry-point range, raw/virtual section extents | Image/section checks plus import/relocation directory range checks |
| File memory | Bounded heider/section reads; disna copy the hale file | Reads the hale file intae a byte vector |
| Repeat load | Checks matching file identity afore loadin an module presence efter | Nae explicit duplicate-file guard in the wrapper inspected; BlackBone internals werena audited |
| Waits | 15-second remote-loader thread wait; keeps the argument buffer alive if loading is still running | 30-second process wait, 20-second module wait; mapping call haes nae explicit deadline in the wrapper |
| UI state | Child process handle guards concurrent loads; status shows CLI result | Detached injection thread an atomic busy flag; richer process/status panels |

Primary sources: [mapping wrapper](https://github.com/leo4048111/Potato-Injector/blob/5b619a02ba4cd04f6688ee539a3468ee3479ed39/injector/injector.cpp), [file reader](https://github.com/leo4048111/Potato-Injector/blob/5b619a02ba4cd04f6688ee539a3468ee3479ed39/utils/utils.hpp), [menu](https://github.com/leo4048111/Potato-Injector/blob/5b619a02ba4cd04f6688ee539a3468ee3479ed39/gui/Menu.cpp), [build instructions](https://github.com/leo4048111/Potato-Injector/blob/5b619a02ba4cd04f6688ee539a3468ee3479ed39/README.md).

## Improvements made frae the comparison

- Added missing image/header, entry-point an section raw/virtual bounds checks tae Cocaine's streaming parser. The Windows loader remains responsible fer import, relocation an dependency resolution.
- Extended malformed-image regression cases fer baith architectures, includin overflowed ranges, truncation, zero-filled sections an zero entry points.
- Gied the launcher a valid inherited stdin handle whan started frae Explorer.
- Reserved completion polling afore startin the injector, sae a timer allocation failure canna strand a running child wi disabled controls.

The injector sanitizer tests, baith architectures' EXE cross-builds an launcher/injector static scan passed. The stricter parser also accepted real compiler-produced x86/x64 fixture DLLs in a host-side sanitizer check. Those are test fixtures, nae Cocaine game DLLs or Windows runtime checks.

## Completion gates still open

1. Publish the workflow at `.github/workflows/validate.yml` on the source branch.
2. Pass MSVC Debug/Release builds, injector integration checks an launcher GUI smoke checks on Windows.
3. Package the actual `Cocaine.dll` wi the matching tools an inspect the artifact.
4. Validate game stairtup against a supported build. The archived x86 game DLL still disna support current CS2.

These gates are needit afore a complete package or overall reliability claim can be made. See [VALIDATION.md](VALIDATION.md).
