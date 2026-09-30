# Cocaine Windows package

The complete Windows build haes:

- `x86/Cocaine.dll`: the game module fer archived 2018 CS:GO.
- `x86/CocaineLauncher.exe` an `x86/CocaineInjector.exe`: the matching Windows tools.
- `x64/CocaineLauncher.exe` an `x64/CocaineInjector.exe`: tools fer x64 DLLs. There is nae x64 Cocaine game DLL.

Extract the hale ZIP. Fer the auld Cocaine DLL, open `x86/CocaineLauncher.exe`, chyse the matching target process, chyse `x86/Cocaine.dll`, an click **Lade DLL**. Haud the lancher an injector in the same folder. Uise Windows 10 version 1709 or later.

**The game DLL disna support current CS2.** A build or synthetic-host check disna prove game compatibility. See `docs/COMPATIBILITY.md` an `docs/INJECTOR.md` fer limits.

## GitHub downloads

The prepared `.github/workflows/validate.yml` needs GitHub connection permission tae write workflow files afore it can be published. There is nae successful Actions DLL artifact yet.

Efter the workflow is published:

Open the repository's **Actions** page, chyse a successful **Validate Cocaine** rin fer the branch ye want, an download **Cocaine-Windows-Release** frae **Artifacts**. Extract that artifact, syne extract its `Cocaine-Windows-Release.zip`. GitHub may ask ye tae sign in fer artifact downloads. Debug builds are available in a sindry artifact.

The workflow builds the DLL an baith tools on Windows, rins the synthetic injector an lancher smoke checks, an packages the files. Packaging fails gin the DLL or ony tool is missin or haes the wrang architecture. Artifacts are published efter those checks pass.

## Build an package on yer Windows machine

Install Visual Studio 2022 wi Desktop development with C++, MSVC v143 an the Windows SDK, plus Python 3. Frae a Visual Studio Developer Command Prompt in the repository folder:

```bat
msbuild Cocaine.sln /m /p:Configuration=Release /p:Platform=x86
msbuild injector\CocaineInjector.vcxproj /m /p:Configuration=Release /p:Platform=x64
msbuild launcher\CocaineLauncher.vcxproj /m /p:Configuration=Release /p:Platform=x64
python tools\package_windows.py --configuration Release
```

The DLL is `output\Release\Cocaine.dll`; the complete package is `dist\Cocaine-Windows-Release.zip`. These full Windows build commands still need a successful MSVC rin; the local Linux checks cover portable code an cross-built EXE tools.
