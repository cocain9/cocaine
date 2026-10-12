# Game compatibility

## Wantit target — checkit 2026-09-29 UTC

The wantit target is the current public **Counter-Strike 2** release, Steam app 730.

- [Valve's update check](https://api.steampowered.com/ISteamApps/UpToDateCheck/v1/?appid=730&version=0) reportit `required_version: 14186`, server version **1.41.8.6**.
- [Valve's official news feed](https://api.steampowered.com/ISteamNews/GetNewsForApp/v2/?appid=730&count=5&maxlength=1200) reportit an update publishit on **2026-09-28**.
- [Valve's store metadata](https://store.steampowered.com/api/appdetails?appids=730) kens the game as Counter-Strike 2, wi Source 2.

The server version is **nae a Steam depot BuildID**. It disna identify ilka installed client module. Thir values are a dated observation, nae an automatic update source fer this DLL.

## Current code

The DLL bides a **32-bit Source 1 implementation frae 2018 CS:GO**. It disna wirk wi current 64-bit CS2. The injector refuses this architecture mismatch. Nae current CS2 offsets were made up or put intae the auld structures.

A wirkin CS2 port needs:

1. A Windows x64 build, wi x86 assembly an callin-convention assumptions replaced.
2. Source 2 entity/schema discovery in place o Source 1 receive tables an layouts.
3. Interfaces, hooks, input, materials, an drawing integration built anew.
4. Client binaries identified by module hashes, syne signature an interface checks against those binaries.
5. Windows integration checks fer stairtup, drawing, settins, shutdown, an game updates.

The hardenit scanner checks syntax, memory bounds, an unique matches. It canna mak an auld ABI or object layout compatible. Automatic offset scrapin canna prove that either.
