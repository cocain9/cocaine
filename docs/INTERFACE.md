# Native menu

The C++ DLL draws the menu through the existing game surface. HTML is uised anerly tae shaw a development preview o its draw calls.

## The look

- **IBM Plex Sans Regular an Medium** are bundled unner the SIL Open Font License. Font registration is private tae the process; Segoe UI is the fallback gin the resource canna load.
- Charcoal panels, aff-white text, an muted sage fer picks.
- Even sidebar rows, the same drawing/click geometry, aligned columns, an a sindry draggable heider.
- Five quaet accents inspired by wuid, fire, yird, metal, an watter. The wee rose hauds a link tae the wantit concert-art feel. Aa motifs are original code; nae third-party band art is bundled.

Feng Shui is an aesthetic reference here. Nae scientific or healt benefit is claimit. The practical choices are readable contrast, steady controls, room tae breathe, an a clear visual order. Nae user study or game benchmark haes been run.

The preview uses actual frame/control drawing code wi an SVG adapter an synthetic sample controls. It embeds the same fonts, but text measures, drawing, an input still need checks in the Windows game runtime. The hale menu isna simulated.

## Scots text

`assets/locale/scots.json` lists ilka current menu label an pick. `scots_locale.h` is built frae it by `tools/generate_scots.py`. Translation happens whan text is drawn; original IDs, choice indices, an callbacks bide the same. Game jargon an model names keep their kent spellins. `python3 tests/check_locale.py` checks coverage an that the header matches the term list. This isna a native-speaker language review.
