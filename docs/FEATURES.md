# Chams an anti-aim additions

## Chams

The new **chams style** tab sets material appearance fer the existing player passes:

| Settin | Whit it daes |
| --- | --- |
| Original | Keeps lit player materials an flat auld models. The default fer auld settins. |
| Flat | Uises the existing flat debug material fer the passes. |
| Wireframe | Uises the flat material wi its wireframe flag on. |
| Opacity whan scoped | 0–100%; default 50%. Uised fer local real/ghost passes whan “blend whan scoped” is on unner Players. |

The Players tab keeps its auld colour, visibility, an blend settins. Chyngin styles sets the wireframe flag explicitly. `SceneEnd` clears temporary material flags an forced override, syne restores default colour/blend. Gin material pointers are missin, ordinary model drawing bides on. A missin local player canna reach the player-distance comparison ony mair.

## Anti-aim presets

Pick **staund**, **wauk**, or **air** in the anti-aim state picker. Pick a preset, syne click **Set preset**:

| Preset | Pickit state |
| --- | --- |
| Aff | Turns real pitch an yaw aff. |
| Backlins | Dounward pitch an backlins direction. |
| Jitter | Dounward pitch, backlins base direction, an the existing 45-degree jitter settin. |
| Turn | Dounward pitch wi a 360-degree turn range an speed settin 10. |

Settin a preset resets the pickit state's base-angle override an applicable fake-body/direction-lock picks. Ither movin states, global enable, fake yaw, an fake lag bide as set. Presets gie convenient stairtin points fer existing algorithms; they dinna promise better gameplay.

A zero turn range nou gies zero phase in place o callin `fmod` wi a zero divisor. The helper refuses non-finite inputs an multiplies in double precision.

## Reference comparison

[NEPS chams](https://github.com/JamalHackCOM/NEPS/blob/7e41ae811cf4ae136203093a059e137c8a10796f/NEPS/Hacks/Chams.cpp) haes a broader shader system, includin animated/rainbow colours an wireframe picks. Its [README](https://github.com/JamalHackCOM/NEPS/blob/7e41ae811cf4ae136203093a059e137c8a10796f/README.md) describes anti-aim conditionals an drag-an-drop settins an aw. Cocaine's additions uise its ain controls an material interfaces; they arena a NEPS port or feature match.

## Checks an limits

`python3 tests/run.py --sanitize --suite features` exercises the actual menu setup, preset callback, material setup, end-o-pass cleanup, an settin methods wi synthetic drawing an host adapters. It checks state isolation, unchynged global toggles, material transitions, numeric bounds, an auld/new settin behaviour.

The checks dinna draw a game frame or verify anti-aim against a server. Windows compilation, the visible result, actual material availability, an game compatibility still need the matching game runtime.
