# Haudin Cocaine in order

## Source map

| Pairt | Main files |
| --- | --- |
| Build an stairtup | `Cocaine.sln`, `Cocaine.vcxproj`, `entry.cpp`, `csgo.cpp` |
| Settins an storage | `config.cpp`, `config_store.h`, `callbacks.cpp` |
| Menu an input | `menu.h`, `gui.*`, `form.*`, `tab.h`, control heiders an code |
| Drawing an notices | `render.*`, `visuals.*`, `notify.h` |
| Game integration | `hooks.*`, `entity.h`, `interfaces.h`, `entoffsets.h`, `sdk.h` |
| Windows lancher an injector | `launcher/main.cpp`, `injector/main.cpp`, `tests/windows_launcher.py`, `tests/windows_injector.py` |
| Gameplay modules | `aimbot.*`, `movement.*`, `prediction.*`, `lagcomp.*`, an related modules |
| Bundled third-party code | `json.h` (nlohmann JSON 2.1.1), `tinyformat.h` |

## Improvements in this revision

- Extractit the hale source archive sae code can be reviewed, checkit, an chynged normally.
- Renamed the solution, project, output DLL, HUD, console prefix, an clan label tae Cocaine. Kept historical credit in `UPSTREAM.md`.
- Mendit Debug settins an lowed C++ exceptions an thread-safe local static init. Baith configurations uise precise floatin-point semantics tae keep non-finite checks wirk in order.
- Moved stairtup wirk oot o `DllMain` tae a worker an closed the thread handle.
- Replaced writin intae `std::string::c_str()` buffers wi filesystem path composition.
- Added bounded binary settin reads, JSON type/range checks, checks afore live updates, an failure reports.
- Added temporary-file replacement whan hainin, an kept access tae auld settins.
- Haudit hotkey hain callbacks durin loadin, sae pairt-laded hotkey files arena written back.
- Guardit dropdown an key indices; kept multi-picks free o duplicates; refused non-finite slider values an needless slider callbacks.
- Mendit an invalid keyword uised as a checkbox argument an active union initialisers in `Color`.
- Added a virtual destructor an initialised the column field fer menu elements.
- Made numeric edit parsing non-throwin an guarded failed Unicode conversions.
- Made console output treat notices as literal text, includin percent signs.
- Refused ill-formit byte signatures, parsed multi-match scans aince, an added unique-match checks fer needit stairtup signatures wi debug diagnostics.
- Added original/flat/wireframe chams styles, scoped opacity, an pickit-state anti-aim presets wi existing controls.
- Kept zero-range anti-aim rotation frae giein an invalid angle; restored shared material flags efter the chams pass.
- Added repeatable regression checks an a Windows/Linux CI workflow.
- Added Scots display text fer the current menu labels, choices, notices, an injector diagnostics. Internal IDs an callbacks bide unchynged.
- Added a native Windows lancher EXE wi process an DLL pickers; it runs the matching command-line injector an shows its result. Windows CI checks the GUI opens an exposes its controls.

## Rules fer development

1. Uise the documented x86 toolchain; a guid build by itsel disna prove game compatibility.
2. Keep new platform-independent helpers self-contained an check edge cases.
3. Keep existing settins or gie an explicit migration path whan chyngin their layout.
4. Check return values afore sayin file operations wrocht.
5. Dinna rewrite binary layouts an callin conventions broadly withoot a matching runtime check.
6. Haud author credit an third-party notices. Check licensing afore redistribution; the archive had nae project licence.

## Offset update request

The target is the current public CS2 release, checkit on 2026-09-29. The source bides an auld x86 Source 1 implementation an needs a Source 2/x64 port afore offset updates can mean onything. `COMPATIBILITY.md` records the dated Valve version observation an port requirements. Nae offsets were chynged by guesswork.

## Checks an upkeep still needit

- **Windows build an smoke check:** neither the hale DLL nor game integration can rin in this Linux editin environment. Rin baith CI build configurations an check stairtup, menu input, settins, Unicode paths, an shutdown in matching Windows. The Documents lookup still uises the auld ANSI API, sae non-ANSI profile paths need follow-up wirk.
- **Game integration:** hard-coded offsets, signatures, an unchecked interface pointers bide version-sensitive. A systematic failure-path audit needs the exact supportit build an a repeatable runtime.
- **Lifecycle:** stairtup is asynchronous; unloadin while the worker or hooks are active is unsupported. Hale hook teardown an worker cancellation need tae be designed an checkit thegither.
- **Concurrency:** settin I/O assumes calls are serialised on the UI thread. Several processes writin the same settin file arena supportit.
- **Dependencies:** the bundled JSON library is auld. An upgrade needs a sindry compatibility an licence review, wi ill-formit input regression checks kept.
- **Speed:** nae game benchmark wis ready. We dinna claim gameplay code is optimised or that ilka auld defect is fund.

The current chynges gie a checkit upkeep baseline. Further improvements should follow reproducible bugs, measured slowdowns, or specific compatibility needs.
