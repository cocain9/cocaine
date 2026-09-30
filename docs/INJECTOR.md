# Native Windows injector

`launcher/CocaineLauncher.vcxproj` builds a native Windows lancher wi a process picker, DLL file picker, an status message. Rin `CocaineLauncher.exe`; haud `CocaineInjector.exe` in the same folder. The lancher starts the injector an shows its result. It needs nae browser or HTML runtime.

`injector/CocaineInjector.vcxproj` builds the command-line injector. Gie it an explicit process ID an absolute DLL path:

```bat
CocaineInjector.exe 1234 "C:\mods\Cocaine.dll"
```

Uise Windows 10 version 1709 or later. The injector, target process, an DLL maun aa hae matching architectures. Paths maun be shorter nor `MAX_PATH`. Unicode paths wirk. Nae browser or HTML runtime is involved.

## Build

The main solution builds the Win32 injector wi the auld DLL:

```bat
msbuild Cocaine.sln /m /p:Configuration=Release /p:Platform=x86
```

- DLL: `output\Release\Cocaine.dll`
- Lancher: `output\Win32\Release\CocaineLauncher.exe`
- Injector: `output\Win32\Release\CocaineInjector.exe`

Tae build the sindry x64 injector:

```bat
msbuild launcher\CocaineLauncher.vcxproj /m /p:Configuration=Release /p:Platform=x64
msbuild injector\CocaineInjector.vcxproj /m /p:Configuration=Release /p:Platform=x64
```

Their outputs are `output\x64\Release\CocaineLauncher.exe` an `CocaineInjector.exe`. **This disna mak an x64 Cocaine DLL or gie CS2 compatibility.**

## Behaviour an limits

- Refuses ill-formit PE heiders, files that arena DLL images, an architecture mismatches afore allocatin target memory.
- Finds the loader address frae its awn module an relative address, includin forwarded exports. It disna assume the same base address in ilka process. The target maun uise the same system loader binaries as the injector.
- Uises `LoadLibraryW`, reports Windows errors, closes process/thread handles, an frees the path allocation efter the loader thread feenishes.
- Gin the loader disna feenish within 15 seconds, it hauds the argument buffer sae memory still in uise isna freed. Check the target afore trying again; the OS reclaims the buffer whan the target exits.
- Checks module presence withoot cuttin aff a pointer-sized x64 module handle. A second rin kens gin the file is awready laded.
- A laded module disna prove that Cocaine's asynchronous stairtup or hooks wrocht. See its debug diagnostic.
- The DLL's hooks dinna support live unloadin. The injector disna dae unloadin, manual mapping, process-protection bypass, or anti-cheat bypass.

Portable checks cover PE parsing. The Windows CI smoke check lades a synthetic DLL intae a synthetic host; it disna check a game. Locally built MinGW executables are cross-compiled artifacts; they still need a Windows runtime check.
