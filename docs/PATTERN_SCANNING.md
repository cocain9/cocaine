# Pattern scanning an game updates

## Update mechanisms

Cocaine gets mony field offsets by walkin game receive tables an data maps (`netvars.h`, `entoffsets.h`). It finds ither addresses wi byte patterns in laded modules (`csgo.cpp`, `pattern.h`). A function can move atween builds an still be fund gin its signature bides the same. Chynged instructions, table names, structure layouts, an callin conventions still need compatible code.

## Scanner contract

- Tokens are sindert by whitespace. Ilka token is twa hexadecimal digits, `?`, or `??`; baith wildcard forms match ae hale byte.
- Uppercase an lowercase hexadecimal digits wirk. Nibble wildcards, `0x` prefixes, extra characters, empty patterns, an ill-formit tokens are refused.
- `find` gies the first hale match or a null address.
- `FindAll` parses aince an gies ilka match, includin overlaps.
- `find_unique` gies a match anerly whan juist ane candidate exists. `require` applies that rule tae a module an throws a diagnostic wi the module name an pattern on failure.
- Needit scans in `CSGO::init` uise `require`. `Client::init` catches stairtup exceptions, writes the diagnostic wi `OutputDebugStringA`, an reports failure withoot giein the walcome notice. Uise a Windows debugger tae capture it.
- Matches maun fit hale within the buffer. Null, zero-length, overflowin, an unrepresentable numeric ranges are refused afore memory is read.
- Callers maun gie a readable, steady buffer. The scanner disna check Windows page permissions, module lifetimes, or whit a matching instruction means. It disna cache module bytes.

## References gien durin development

The [u16rogue C# scanner](https://github.com/u16rogue/csgo_external_cs/blob/d78838a2b32fe866a87263e1da2be35fdeab58ed/Classes/CSGOModule.cs) shaws byte/mask scanning. Its inner loop can index by the buffer end, an its success check is skippit fer a trailin wildcard. A failed first read leaves an allocated cache that later calls reuise. Thir details werena copied. Cocaine's checks cover buffer ends, trailin/aa wildcards, an chynged buffer contents.

The [VDK45 updater](https://github.com/VDK45/CSGO-cheats-auto-update-offsets/blob/76bb5fb5e49a01953d0f3cb3b11cabe77de35931/offsets.py) scrapes a GitHub HTML page fer a hazedumper heider, converts the values, an keeps some hard-coded offsets. It haes nae build-identity check. Cocaine disna import that list: names, address kinds, an runtime interfaces maun first be matched tae the supportit game build.

## Afore addin a remote offset source

Record the target game's build an architecture, source revision, module identity, an whether ilka value is a field offset, relative address, or absolute address. Uise structured data wi type/range checks an refuse incomplete or mismatched sets afore chyngin active state. Haud a validated local set fer the same build; a stale set frae anither build isna a fallback. A provider's newest commit by itsel disna prove compatibility.

## Check status

Portable scanner checks rin against synthetic heap buffers wi AddressSanitizer an UndefinedBehaviorSanitizer. Stairtup checks exercise the actual exception handler wi a synthetic init result. The project check verifies that literal signatures uise supportit syntax an needit stairtup scans uise `require`.

Nae game offsets or signatures were replaced wi guessit values. Windows DLL compilation, page access, unique signatures on real modules, an compatibility wi ony particular game patch bide unverified till the target build an runtime are ready.
