# Validation status — 2026-09-30

## Mendit in this pass

- `xorstr.h`: the Release helper returned a pointer intae a destroyed temporary. Ilka call site noo owns its string buffer fer the process lifetime. The reserved `xor` namespace wis renamed.
- `hash.h`: empty strings noo start the length check at index zero, withoot readin past the terminator.
- `entity.h`: bone positions are returned by value; indices ootwith 0–127 are refused afore the engine call.
- `visuals.cpp`: beam-impact neighbor checks uise the current list size efter expired entries hae been erased.
- `tests/run.py`: quoted-heider forwarders keep the project's `math.h` oot o MSVC's system include search. New fixtures check Release string lifetime, empty hashes, bone bounds an expired-impact removal.
- `tests/menu_inventory.py`: explicit UTF-8 keeps the Scots parity ticklist independent o the Windows system locale.

## Results

The eight portable regression suites passed wi AddressSanitizer an UndefinedBehaviorSanitizer. The final impact-boundary fixture passed efter its addition. Source/project checks, Scots coverage an aa 366 menu contracts passed. These checks uise synthetic host/render adapters; they dinna verify engine offsets or live game hooks.

Cppcheck 2.7.4 scanned aa 52 DLL translation units in Release/Win32 configuration. It reported **85 diagnostics: 10 error-severity diagnostics, 65 warnings an 10 performance suggestions**. The error-severity diagnostics are in the bundled `json.h`; they include exception-path, iterator-wrapper an byte-initialisation reports that need library-specific triage. They arena a demonstrated Windows compiler failure. Remaining warnings include SDK object initialisation, duplicate menu-label fields an resource-owning class copy semantics. They hae nae blanket suppression; the project isna bein declared warning-free. The affected visuals unit wis rescanned efter the final fix.

Efter the [Potato-Injector comparison](LOADER_COMPARISON.md), the loader gained section/image validation an the launcher gained safer stdin/timer setup. The fresh launcher/injector static scan returned nae diagnostics; all four EXEs rebuilt wi strict MinGW warnings. Updated PE sanitizer tests passed. CodeRabbit review wis attempted, but it is disabled fer this task; nae completed CodeRabbit review is claimed.

## Windows build blocker

`validate.yml` wis uploaded tae the repository root. GitHub requires `.github/workflows/validate.yml`. The connected GitHub App still refuses writes tae workflow paths, sae the move couldna be published. Nae Actions run, MSVC DLL build or complete DLL/EXE artifact has been verified.

Move the uploaded file tae `.github/workflows/validate.yml` on the `coderabbit/rename-program-cocaine/3058715c` branch, which haes the source, or grant the connection workflow-write permission. The correct-path copy is prepared locally. A successful Windows build remains required afore claiming a complete package.

The game DLL still targets archived 2018 CS:GO; a current CS2 port remains separate unfinished work.
