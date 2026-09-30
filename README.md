# Cocaine

Cocaine is a Windows x86 C++ project frae archived source fer **2018 Counter-Strike: Global Offensive**. It builds a native DLL, a Windows lancher EXE, an a sindry command-line injector EXE. **The DLL disna wirk wi current CS2.** The wantit target an the engine port it needs are set oot in [docs/COMPATIBILITY.md](docs/COMPATIBILITY.md).

Ye can edit the source richt in this repository. The auld archive bides in Git history; [docs/UPSTREAM.md](docs/UPSTREAM.md) credits its author.

## Build on Windows

Fer commands tae build an package the DLL an EXEs, see [Windows packages](docs/DOWNLOADS.md). Automatic Actions downloads need the prepared workflow tae be published; that is currently blocked by the GitHub connection's workflow permissions.

Ye need:

- Visual Studio 2022 wi **Desktop development with C++**.
- MSVC v143 x86 tools an the Windows 10/11 SDK.
- C++17, as set in the project.

Open `Cocaine.sln`, pick **Release | x86**, an build. Frae a Visual Studio Developer Command Prompt:

```bat
msbuild Cocaine.sln /m /p:Configuration=Release /p:Platform=x86
```

The DLL gangs tae `output\Release\Cocaine.dll`. The lancher an injector EXEs gang tae `output\Win32\Release\`; haud the twa EXEs in the same folder. Fer development, uise `Configuration=Debug`: it turns optimisation aff an writes debug information. The code haes x86 assembly an platform-specific interfaces, sae the DLL haes nae x64 or native Linux build. The lancher an injector support Win32 an x64; [docs/INJECTOR.md](docs/INJECTOR.md) haes their build paths an usage.

## Settins

- New settins: `Documents\Cocaine\1.cfg` through `6.cfg`.
- Hotkeys: `Documents\Cocaine\keys.cfg`.
- Files haud Base64-encoded JSON. Base64 is encoding, nae encryption.
- Gin a new file isna there, loadin tries the matching `.sup` file in `Documents\user` (the auld place). Hainin writes tae the new place anerly, an leaves the auld file be.
- Ill-formit files report a load failure. Kent values are checkit afore ony settins are applied; unkent fields are owerset fer compatibility. Auld dropdown picks are owerset, an slider values are haudit within their limits.
- Hainin writes a temporary file an replaces the destination anerly efter a guid write an close. This guards the auld file against ordinary write failures; it canna promise recovery frae ilka power or hardware failure.

## Checks

Portable checks need Python 3 an a C++17 compiler (GCC 11+ or MSVC v143):

```sh
python3 tests/check_project.py
python3 tests/check_locale.py
python3 tests/run.py --sanitize
```

On Windows, rin `python tests/run.py` in an x86 Visual Studio Developer Command Prompt. That check exercises native file replacement. `--sanitize` louses AddressSanitizer an UndefinedBehaviorSanitizer wi GCC on Linux.

The regression suites compile the actual settin methods, menu controls, scanner, an stairtup exception handling wi wee host/render adapters. They check ill-formit files, bounds, failed hains, auld-file fallback, hotkey callbacks, settin round trips, signature syntax, overlapping matches, unclear signatures, an stairtup failures. They dinna simulate the game or verify its drawing, hooks, or binary offsets.

GitHub Actions rins portable checks an baith Windows build configurations. The workflow bein there disna mean those Windows builds hae been verified yet.

See [docs/MAINTENANCE.md](docs/MAINTENANCE.md) fer changes, the source map, an checks still tae dae.

## Whan the game updates

Cocaine finds mony field offsets frae game tables an locates functions wi byte signatures. A needit stairtup signature maun match juist aince; a missin or unclear match stops stairtup an writes a module/pattern diagnostic tae Windows debug output. [docs/PATTERN_SCANNING.md](docs/PATTERN_SCANNING.md) explains syntax, updater references, an compatibility limits.

## New controls

- **Chams style:** original material, flat shadin, wireframe, an scoped opacity. Turn scoped blend on unner Players tae uise the opacity settin. The auld player colours an blend settins bide there.
- **Anti-aim:** pick a movin state an preset, syne **Set preset**. Presets set the existing pitch/yaw values fer that state; global enable, fake yaw an fake lag bide unner yer control.

[docs/FEATURES.md](docs/FEATURES.md) explains their behaviour an check limits.

## Native menu in Scots

The C++ menu haes bundled IBM Plex Sans fonts, charcoal panels, sage accents, a clear sidebar, an a wee rose. Aa 366 existing controls hae Scots display entries. The auld settin keys, item indices, an callbacks bide the same; technical game terms an model names can be the same in Scots. [docs/INTERFACE.md](docs/INTERFACE.md) explains the look, font licence, an preview limits.
