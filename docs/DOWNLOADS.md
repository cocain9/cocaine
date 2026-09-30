# Cocaine Windows package

The complete Windows build haes:

- `x86/Cocaine.dll`: the game module fer archived 2018 CS:GO.
- `x86/CocaineLauncher.exe` an `x86/CocaineInjector.exe`: the matching Windows tools.
- `x64/CocaineLauncher.exe` an `x64/CocaineInjector.exe`: tools fer x64 DLLs. There is nae x64 Cocaine game DLL.

Extract the hale ZIP. Fer the auld Cocaine DLL, open `x86/CocaineLauncher.exe`, chyse the matching target process, chyse `x86/Cocaine.dll`, an click **Lade DLL**. Haud the lancher an injector in the same folder. Uise Windows 10 version 1709 or later.

**The game DLL disna support current CS2.** A build or synthetic-host check disna prove game compatibility. See `docs/COMPATIBILITY.md` an `docs/INJECTOR.md` fer limits.

## GitHub downloads

Open the repository's **Actions** page, chyse a successful **Validate Cocaine** rin fer the branch ye want, an download **Cocaine-Windows-Release** frae **Artifacts**. Extract that artifact, syne extract its `Cocaine-Windows-Release.zip`. GitHub may ask ye tae sign in fer artifact downloads. Debug builds are available in a sindry artifact.

The workflow builds the DLL an baith tools on Windows, rins the synthetic injector an lancher smoke checks, an packages the files. Packaging fails gin the DLL or ony tool is missin or haes the wrang architecture. Artifacts are published efter those checks pass.
