# Ticklist o ilka feature

Baseline: aa 366 controls registert in `menu.h` afore the spatial-layout restyle. Ilka tick maks siccar the source control an its settin contract are still there; it disna claim post-migration runtime checks.

Conditional controls bide conditional. Their original visibility predicates are listit sae ilka path can be checkit. Unlabelt controls are kenned by their settin key. Dropdown an multi-pick choices are includit.

## Aimbot (28)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Turn on · `enable` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | quate aimbot · `silent` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | Mairk pick · `selection` | Dropdown | Aimbot / col 1 | Ayeweys ready; picks: farness, crosshair, skaith, healt, lag, hicht |
| ✓ | angle limit · `fov` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | (nae label: fov_amount) · `fov_amount` | Slider | Aimbot / col 1 | AddShowCallback(callbacks::IsFovOn) |
| ✓ | hitbox · `hitbox` | MultiDropdown | Aimbot / col 1 | Ayeweys ready; picks: heid, breist, body, airms, legs |
| ✓ | hitbox past · `hitbox_history` | MultiDropdown | Aimbot / col 1 | Ayeweys ready; picks: heid, breist, body, airms, legs |
| ✓ | multi-point · `multipoint` | MultiDropdown | Aimbot / col 1 | Ayeweys ready; picks: heid, breist, body, legs |
| ✓ | (nae label: hitbox_scale) · `scale` | Slider | Aimbot / col 1 | AddShowCallback(callbacks::IsMultipointOn) |
| ✓ | body hitbox scale · `body_scale` | Slider | Aimbot / col 1 | AddShowCallback(callbacks::IsMultipointBodyOn) |
| ✓ | least skaith · `minimal_damage` | Slider | Aimbot / col 1 | Ayeweys ready |
| ✓ | scale skaith on hp · `minimal_damage_hp` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | Shoot throu waas · `penetrate` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | (nae label: penetrate_minimal_damage) · `penetrate_minimal_damage` | Slider | Aimbot / col 1 | AddShowCallback(callbacks::IsPenetrationOn) |
| ✓ | scale penetration skaith on hp · `penetrate_minimal_damage_hp` | Checkbox | Aimbot / col 1 | AddShowCallback(callbacks::IsPenetrationOn) |
| ✓ | aimbot wi knife · `knifebot` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | aimbot wi taser · `zeusbot` | Checkbox | Aimbot / col 1 | Ayeweys ready |
| ✓ | auto scope · `zoom` | Dropdown | Aimbot / col 2 | Ayeweys ready; picks: Aff, Aye, hitchance fail |
| ✓ | Mend spread · `nospread` | Checkbox | Aimbot / col 2 | AddShowCallback(callbacks::IsConfigNS) |
| ✓ | Mend recoil · `norecoil` | Checkbox | Aimbot / col 2 | Ayeweys ready |
| ✓ | hitchance · `hitchance` | Checkbox | Aimbot / col 2 | AddShowCallback(callbacks::IsConfigMM) |
| ✓ | (nae label: hitchance_amount) · `hitchance_amount` | Slider | Aimbot / col 2 | AddShowCallback(callbacks::IsConfigMM); AddShowCallback(callbacks::IsHitchanceOn) |
| ✓ | Forsee fake-lag · `lagfix` | Checkbox | Aimbot / col 2 | Ayeweys ready |
| ✓ | Mend anti-aim · `correct` | Checkbox | Aimbot / col 2 | Ayeweys ready |
| ✓ | favour body aim · `baim1` | MultiDropdown | Aimbot / col 2 | Ayeweys ready; picks: Aye, deidly, lethal x2, fake, in air |
| ✓ | anerly body aim · `baim2` | MultiDropdown | Aimbot / col 2 | Ayeweys ready; picks: Aye, healt, fake, in air |
| ✓ | (nae label: baim_hp) · `baim_hp` | Slider | Aimbot / col 2 | AddShowCallback(callbacks::IsBaimHealth) |
| ✓ | Body aim on key · `baim_key` | Keybind | Aimbot / col 2 | Ayeweys ready |

## Anti-aim (46)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Turn on · `enable` | Checkbox | Anti-aim / col 1 | Ayeweys ready |
| ✓ | edge · `edge` | Checkbox | Anti-aim / col 1 | Ayeweys ready |
| ✓ | (nae label: mode) · `mode` | Dropdown | Anti-aim / col 1 | Ayeweys ready; picks: staund, wauk, air |
| ✓ | pitch · `pitch_stand` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); picks: Aff, doun, up, random, ideal |
| ✓ | yaw · `yaw_stand` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); picks: Aff, direction, jitter, turn, random |
| ✓ | (nae label: jitter_range_stnd) · `jitter_range_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::IsStandYawJitter) |
| ✓ | (nae label: rot_range_stnd) · `rot_range_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::IsStandYawRotate) |
| ✓ | (nae label: rot_speed_stnd) · `rot_speed_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::IsStandYawRotate) |
| ✓ | (nae label: rand_update_stnd) · `rand_update_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::IsStandYawRnadom) |
| ✓ | direction · `dir_stand` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw); picks: auto, backlins, left, richt, custom |
| ✓ | (nae label: dir_time_stnd) · `dir_time_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw); AddShowCallback(callbacks::IsStandDirAuto) |
| ✓ | (nae label: dir_custom_stnd) · `dir_custom_stand` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw); AddShowCallback(callbacks::IsStandDirCustom) |
| ✓ | base angle · `base_angle_stand` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw); picks: Aff, still, awa crosshair, awa farness |
| ✓ | haud direction · `dir_lock` | Checkbox | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw) |
| ✓ | Fake body · `body_fake_stand` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeStand); AddShowCallback(callbacks::HasStandYaw); picks: Aff, left, richt, opposite, z |
| ✓ | pitch · `pitch_walk` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); picks: Aff, doun, up, random, ideal |
| ✓ | yaw · `yaw_walk` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); picks: Aff, direction, jitter, turn, random |
| ✓ | (nae label: jitter_range_walk) · `jitter_range_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::IsWalkYawJitter) |
| ✓ | (nae label: rot_range_walk) · `rot_range_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::IsWalkYawRotate) |
| ✓ | (nae label: rot_speed_walk) · `rot_speed_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::IsWalkYawRotate) |
| ✓ | (nae label: rand_update_walk) · `rand_update_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::IsWalkYawRandom) |
| ✓ | direction · `dir_walk` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::WalkHasYaw); picks: auto, backlins, left, richt, custom |
| ✓ | (nae label: dir_time_walk) · `dir_time_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::WalkHasYaw); AddShowCallback(callbacks::IsWalkDirAuto) |
| ✓ | (nae label: dir_custom_walk) · `dir_custom_walk` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::WalkHasYaw); AddShowCallback(callbacks::IsWalkDirCustom) |
| ✓ | base angle · `base_angle_walk` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeWalk); AddShowCallback(callbacks::WalkHasYaw); picks: Aff, still, awa crosshair, awa farness |
| ✓ | pitch · `pitch_air` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); picks: Aff, doun, up, random, ideal |
| ✓ | yaw · `yaw_air` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); picks: Aff, direction, jitter, turn, random |
| ✓ | (nae label: jitter_range_air) · `jitter_range_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::IsAirYawJitter) |
| ✓ | (nae label: rot_range_air) · `rot_range_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::IsAirYawRotate) |
| ✓ | (nae label: rot_speed_air) · `rot_speed_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::IsAirYawRotate) |
| ✓ | (nae label: rand_update_air) · `rand_update_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::IsAirYawRandom) |
| ✓ | direction · `dir_air` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::AirHasYaw); picks: auto, backlins, left, richt, custom |
| ✓ | (nae label: dir_time_air) · `dir_time_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::AirHasYaw); AddShowCallback(callbacks::IsAirDirAuto) |
| ✓ | (nae label: dir_custom_air) · `dir_custom_air` | Slider | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::AirHasYaw); AddShowCallback(callbacks::IsAirDirCustom) |
| ✓ | base angle · `base_angle_air` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::AirHasYaw); picks: Aff, still, awa crosshair, awa farness |
| ✓ | Fake body · `body_fake_air` | Dropdown | Anti-aim / col 1 | AddShowCallback(callbacks::IsAntiAimModeAir); AddShowCallback(callbacks::AirHasYaw); picks: Aff, left, richt, opposite |
| ✓ | Fake yaw · `fake_yaw` | Dropdown | Anti-aim / col 2 | Ayeweys ready; picks: Aff, ordinar, relative, jitter, turn, random, local sicht |
| ✓ | (nae label: fake_relative) · `fake_relative` | Slider | Anti-aim / col 2 | AddShowCallback(callbacks::IsFakeAntiAimRelative) |
| ✓ | (nae label: fake_jitter_range) · `fake_jitter_range` | Slider | Anti-aim / col 2 | AddShowCallback(callbacks::IsFakeAntiAimJitter) |
| ✓ | fake-lag · `lag_enable` | Checkbox | Anti-aim / col 2 | Ayeweys ready |
| ✓ | (nae label: lag_active) · `lag_active` | MultiDropdown | Anti-aim / col 2 | Ayeweys ready; picks: move, air, cower |
| ✓ | (nae label: lag_mode) · `lag_mode` | Dropdown | Anti-aim / col 2 | Ayeweys ready; picks: maist, Breik, random, break step |
| ✓ | limit · `lag_limit` | Slider | Anti-aim / col 2 | Ayeweys ready |
| ✓ | on grund · `lag_land` | Checkbox | Anti-aim / col 2 | Ayeweys ready |
| ✓ | Preset fer pickit state · `preset` | Dropdown | Anti-aim / col 2 | Ayeweys ready; picks: Aff, backlins, jitter, turn |
| ✓ | Set preset · `apply_preset` | Button | Anti-aim / col 2 | Ayeweys ready |

## Players (41)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | boxes · `box` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: enemy, freendly |
| ✓ | box enemy colour · `box_enemy` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | box freendly colour · `box_friendly` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | dormant enemies · `dormant` | Checkbox | Players / col 1 | Ayeweys ready |
| ✓ | enemy affscreen esp · `offscreen` | Checkbox | Players / col 1 | Ayeweys ready |
| ✓ | affscreen esp colour · `offscreen_color` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | name · `name` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: enemy, freendly |
| ✓ | name colour · `name_color` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | healt · `health` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: enemy, freendly |
| ✓ | flags enemy · `flags_enemy` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: siller, armour, scoped, blindet, reladin, bomb |
| ✓ | flags freendly · `flags_friendly` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: siller, armour, scoped, blindet, reladin, bomb |
| ✓ | weapon · `weapon` | MultiDropdown | Players / col 1 | Ayeweys ready; picks: enemy, freendly |
| ✓ | (nae label: weapon_mode) · `weapon_mode` | Dropdown | Players / col 1 | Ayeweys ready; picks: text, icon |
| ✓ | ammo · `ammo` | Checkbox | Players / col 1 | Ayeweys ready |
| ✓ | colour · `ammo_color` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | lby update · `lby_update` | Checkbox | Players / col 1 | Ayeweys ready |
| ✓ | colour · `lby_update_color` | Colorpicker | Players / col 1 | Ayeweys ready |
| ✓ | skeleton · `skeleton` | MultiDropdown | Players / col 2 | Ayeweys ready; picks: enemy, freendly |
| ✓ | enemy colour · `skeleton_enemy` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | freendly colour · `skeleton_friendly` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | glow · `glow` | MultiDropdown | Players / col 2 | Ayeweys ready; picks: enemy, freendly |
| ✓ | enemy colour · `glow_enemy` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | freendly colour · `glow_friendly` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | (nae label: glow_blend) · `glow_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | chams enemy · `chams_enemy` | MultiDropdown | Players / col 2 | Ayeweys ready; picks: seen, unseen |
| ✓ | colour seen · `chams_enemy_vis` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | colour unseen · `chams_enemy_invis` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | (nae label: chams_enemy_blend) · `chams_enemy_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | chams past · `chams_enemy_history` | Checkbox | Players / col 2 | Ayeweys ready |
| ✓ | colour · `chams_enemy_history_col` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | (nae label: chams_history_blend) · `chams_enemy_history_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | chams freendly · `chams_friendly` | MultiDropdown | Players / col 2 | Ayeweys ready; picks: seen, unseen |
| ✓ | colour seen · `chams_friendly_vis` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | colour unseen · `chams_friendly_invis` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | chams local · `chams_local` | MultiDropdown | Players / col 2 | Ayeweys ready; picks: real, fake |
| ✓ | (nae label: chams_friendly_blend) · `chams_friendly_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | colour · `chams_local_col` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | (nae label: chams_local_blend) · `chams_local_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | fake colour · `chams_local_ghost_col` | Colorpicker | Players / col 2 | Ayeweys ready |
| ✓ | (nae label: chams_local_ghost_blend) · `chams_local_ghost_blend` | Slider | Players / col 2 | Ayeweys ready |
| ✓ | blend whan scoped · `chams_local_scope` | Checkbox | Players / col 2 | Ayeweys ready |

## Chams style (2)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | material · `material` | Dropdown | Chams style / col 1 | Ayeweys ready; picks: original, flat, wireframe |
| ✓ | Opacity whan scoped · `scoped_blend` | Slider | Chams style / col 1 | Ayeweys ready |

## Sichtins (36)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Drapt weapons · `items` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | Drapt weapon ammo · `ammo` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | colour · `item_color` | Colorpicker | Sichtins / col 1 | Ayeweys ready |
| ✓ | projectiles · `proj` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | colour · `proj_color` | Colorpicker | Sichtins / col 1 | Ayeweys ready |
| ✓ | projectile range · `proj_range` | MultiDropdown | Sichtins / col 1 | Ayeweys ready; picks: frag, molly |
| ✓ | colour · `proj_range_color` | Colorpicker | Sichtins / col 1 | Ayeweys ready |
| ✓ | planted c4 · `planted_c4` | MultiDropdown | Sichtins / col 1 | Ayeweys ready; picks: On screen (2D), On bomb (3D) |
| ✓ | Dinna shaw team-mates · `disableteam` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | warld · `world` | Dropdown | Sichtins / col 1 | Ayeweys ready; picks: Aff, nicht, fu bricht |
| ✓ | sheer props · `transparent_props` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | Shaw enemies on radar · `enemy_radar` | Checkbox | Sichtins / col 1 | Ayeweys ready |
| ✓ | Tak awa sicht recoil · `novisrecoil` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Tak awa smoke grenades · `nosmoke` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Tak awa fog · `nofog` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Tak awa flashbangs · `noflash` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Tak awa scope · `noscope` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Owerwrite FOV · `fov` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | (nae label: fov_amt) · `fov_amt` | Slider | Sichtins / col 2 | Ayeweys ready |
| ✓ | Owerwrite FOV whan scoped · `fov_scoped` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | Owerwrite viewmodel FOV · `viewmodel_fov` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | (nae label: viewmodel_fov_amt) · `viewmodel_fov_amt` | Slider | Sichtins / col 2 | Ayeweys ready |
| ✓ | Shaw spectator list · `spectators` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | force crosshair · `force_xhair` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | shaw spread · `spread_xhair` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | shaw spread colour · `spread_xhair_col` | Colorpicker | Sichtins / col 2 | Ayeweys ready |
| ✓ | (nae label: spread_xhair_blend) · `spread_xhair_blend` | Slider | Sichtins / col 2 | Ayeweys ready |
| ✓ | penetration crosshair · `pen_crosshair` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | signs · `indicators` | MultiDropdown | Sichtins / col 2 | Ayeweys ready; picks: lby, lag compensation, Fake latency |
| ✓ | grenade simulation · `tracers` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | impact beams · `impact_beams` | Checkbox | Sichtins / col 2 | Ayeweys ready |
| ✓ | impact beams colour · `impact_beams_color` | Colorpicker | Sichtins / col 2 | Ayeweys ready |
| ✓ | impact beams hurt colour · `impact_beams_hurt_color` | Colorpicker | Sichtins / col 2 | Ayeweys ready |
| ✓ | impact beams time · `impact_beams_time` | Slider | Sichtins / col 2 | Ayeweys ready |
| ✓ | third-person · `thirdperson` | Keybind | Sichtins / col 2 | Ayeweys ready |
| ✓ |   · `thirdperson_distance` | Slider | Sichtins / col 2 | Ayeweys ready |

## Movin (12)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Automatic lowp · `bhop` | Checkbox | Movin / col 1 | Ayeweys ready |
| ✓ | duck in air · `airduck` | Checkbox | Movin / col 1 | Ayeweys ready |
| ✓ | automatic strafe · `autostrafe` | Checkbox | Movin / col 1 | Ayeweys ready |
| ✓ | c-strafe · `cstrafe` | Keybind | Movin / col 1 | Ayeweys ready |
| ✓ | a-strafe · `astrafe` | Keybind | Movin / col 1 | Ayeweys ready |
| ✓ | z-strafe · `zstrafe` | Keybind | Movin / col 1 | Ayeweys ready |
| ✓ | (nae label: z_freq) · `z_freq` | Slider | Movin / col 1 | Ayeweys ready |
| ✓ | (nae label: z_dist) · `z_dist` | Slider | Movin / col 1 | Ayeweys ready |
| ✓ | fake-wauk · `fakewalk` | Keybind | Movin / col 2 | Ayeweys ready |
| ✓ | Automatic keek · `autopeek` | Keybind | Movin / col 2 | Ayeweys ready |
| ✓ | automatic stop aye on · `autostop_always_on` | Checkbox | Movin / col 2 | Ayeweys ready |
| ✓ | Automatic stap · `autostop` | Keybind | Movin / col 2 | AddShowCallback(callbacks::AUTO_STOP) |

## Skins (176)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Turn on · `enable` | Checkbox | Skins / col 1 | Ayeweys ready |
| ✓ | paintkit ID · `id_deagle` | Edit | Skins / col 1 | AddShowCallback(callbacks::DEAGLE) |
| ✓ | stattrak · `stattrak_deagle` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::DEAGLE) |
| ✓ | quality · `quality_deagle` | Slider | Skins / col 1 | AddShowCallback(callbacks::DEAGLE) |
| ✓ | seed · `seed_deagle` | Slider | Skins / col 1 | AddShowCallback(callbacks::DEAGLE) |
| ✓ | paintkit ID · `id_elite` | Edit | Skins / col 1 | AddShowCallback(callbacks::ELITE) |
| ✓ | stattrak · `stattrak_elite` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::ELITE) |
| ✓ | quality · `quality_elite` | Slider | Skins / col 1 | AddShowCallback(callbacks::ELITE) |
| ✓ | seed · `seed_elite` | Slider | Skins / col 1 | AddShowCallback(callbacks::ELITE) |
| ✓ | paintkit ID · `id_fiveseven` | Edit | Skins / col 1 | AddShowCallback(callbacks::FIVESEVEN) |
| ✓ | stattrak · `stattrak_fiveseven` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::FIVESEVEN) |
| ✓ | quality · `quality_fiveseven` | Slider | Skins / col 1 | AddShowCallback(callbacks::FIVESEVEN) |
| ✓ | seed · `seed_fiveseven` | Slider | Skins / col 1 | AddShowCallback(callbacks::FIVESEVEN) |
| ✓ | paintkit ID · `id_glock` | Edit | Skins / col 1 | AddShowCallback(callbacks::GLOCK) |
| ✓ | stattrak · `stattrak_glock` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::GLOCK) |
| ✓ | quality · `quality_glock` | Slider | Skins / col 1 | AddShowCallback(callbacks::GLOCK) |
| ✓ | seed · `seed_glock` | Slider | Skins / col 1 | AddShowCallback(callbacks::GLOCK) |
| ✓ | paintkit ID · `id_ak47` | Edit | Skins / col 1 | AddShowCallback(callbacks::AK47) |
| ✓ | stattrak · `stattrak_ak47` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::AK47) |
| ✓ | quality · `quality_ak47` | Slider | Skins / col 1 | AddShowCallback(callbacks::AK47) |
| ✓ | seed · `seed_ak47` | Slider | Skins / col 1 | AddShowCallback(callbacks::AK47) |
| ✓ | paintkit ID · `id_aug` | Edit | Skins / col 1 | AddShowCallback(callbacks::AUG) |
| ✓ | stattrak · `stattrak_aug` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::AUG) |
| ✓ | quality · `quality_aug` | Slider | Skins / col 1 | AddShowCallback(callbacks::AUG) |
| ✓ | seed · `seed_aug` | Slider | Skins / col 1 | AddShowCallback(callbacks::AUG) |
| ✓ | paintkit ID · `id_awp` | Edit | Skins / col 1 | AddShowCallback(callbacks::AWP) |
| ✓ | stattrak · `stattrak_awp` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::AWP) |
| ✓ | quality · `quality_awp` | Slider | Skins / col 1 | AddShowCallback(callbacks::AWP) |
| ✓ | seed · `seed_awp` | Slider | Skins / col 1 | AddShowCallback(callbacks::AWP) |
| ✓ | paintkit ID · `id_famas` | Edit | Skins / col 1 | AddShowCallback(callbacks::FAMAS) |
| ✓ | stattrak · `stattrak_famas` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::FAMAS) |
| ✓ | quality · `quality_famas` | Slider | Skins / col 1 | AddShowCallback(callbacks::FAMAS) |
| ✓ | seed · `seed_famas` | Slider | Skins / col 1 | AddShowCallback(callbacks::FAMAS) |
| ✓ | paintkit ID · `id_g3sg1` | Edit | Skins / col 1 | AddShowCallback(callbacks::G3SG1) |
| ✓ | stattrak · `stattrak_g3sg1` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::G3SG1) |
| ✓ | quality · `quality_g3sg1` | Slider | Skins / col 1 | AddShowCallback(callbacks::G3SG1) |
| ✓ | seed · `seed_g3sg1` | Slider | Skins / col 1 | AddShowCallback(callbacks::G3SG1) |
| ✓ | paintkit ID · `id_galil` | Edit | Skins / col 1 | AddShowCallback(callbacks::GALIL) |
| ✓ | stattrak · `stattrak_galil` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::GALIL) |
| ✓ | quality · `quality_galil` | Slider | Skins / col 1 | AddShowCallback(callbacks::GALIL) |
| ✓ | seed · `seed_galil` | Slider | Skins / col 1 | AddShowCallback(callbacks::GALIL) |
| ✓ | paintkit ID · `id_m249` | Edit | Skins / col 1 | AddShowCallback(callbacks::M249) |
| ✓ | stattrak · `stattrak_m249` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::M249) |
| ✓ | quality · `quality_m249` | Slider | Skins / col 1 | AddShowCallback(callbacks::M249) |
| ✓ | seed · `seed_m249` | Slider | Skins / col 1 | AddShowCallback(callbacks::M249) |
| ✓ | paintkit ID · `id_m4a4` | Edit | Skins / col 1 | AddShowCallback(callbacks::M4A4) |
| ✓ | stattrak · `stattrak_m4a4` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::M4A4) |
| ✓ | quality · `quality_m4a4` | Slider | Skins / col 1 | AddShowCallback(callbacks::M4A4) |
| ✓ | seed · `seed_m4a4` | Slider | Skins / col 1 | AddShowCallback(callbacks::M4A4) |
| ✓ | paintkit ID · `id_mac10` | Edit | Skins / col 1 | AddShowCallback(callbacks::MAC10) |
| ✓ | stattrak · `stattrak_mac10` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::MAC10) |
| ✓ | quality · `quality_mac10` | Slider | Skins / col 1 | AddShowCallback(callbacks::MAC10) |
| ✓ | seed · `seed_mac10` | Slider | Skins / col 1 | AddShowCallback(callbacks::MAC10) |
| ✓ | paintkit ID · `id_p90` | Edit | Skins / col 1 | AddShowCallback(callbacks::P90) |
| ✓ | stattrak · `stattrak_p90` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::P90) |
| ✓ | quality · `quality_p90` | Slider | Skins / col 1 | AddShowCallback(callbacks::P90) |
| ✓ | seed · `seed_p90` | Slider | Skins / col 1 | AddShowCallback(callbacks::P90) |
| ✓ | paintkit ID · `id_ump45` | Edit | Skins / col 1 | AddShowCallback(callbacks::UMP45) |
| ✓ | stattrak · `stattrak_ump45` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::UMP45) |
| ✓ | quality · `quality_ump45` | Slider | Skins / col 1 | AddShowCallback(callbacks::UMP45) |
| ✓ | seed · `seed_ump45` | Slider | Skins / col 1 | AddShowCallback(callbacks::UMP45) |
| ✓ | paintkit ID · `id_xm1014` | Edit | Skins / col 1 | AddShowCallback(callbacks::XM1014) |
| ✓ | stattrak · `stattrak_xm1014` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::XM1014) |
| ✓ | quality · `quality_xm1014` | Slider | Skins / col 1 | AddShowCallback(callbacks::XM1014) |
| ✓ | seed · `seed_xm1014` | Slider | Skins / col 1 | AddShowCallback(callbacks::XM1014) |
| ✓ | paintkit ID · `id_bizon` | Edit | Skins / col 1 | AddShowCallback(callbacks::BIZON) |
| ✓ | stattrak · `stattrak_bizon` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::BIZON) |
| ✓ | quality · `quality_bizon` | Slider | Skins / col 1 | AddShowCallback(callbacks::BIZON) |
| ✓ | seed · `seed_bizon` | Slider | Skins / col 1 | AddShowCallback(callbacks::BIZON) |
| ✓ | paintkit ID · `id_mag7` | Edit | Skins / col 1 | AddShowCallback(callbacks::MAG7) |
| ✓ | stattrak · `stattrak_mag7` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::MAG7) |
| ✓ | quality · `quality_mag7` | Slider | Skins / col 1 | AddShowCallback(callbacks::MAG7) |
| ✓ | seed · `seed_mag7` | Slider | Skins / col 1 | AddShowCallback(callbacks::MAG7) |
| ✓ | paintkit ID · `id_negev` | Edit | Skins / col 1 | AddShowCallback(callbacks::NEGEV) |
| ✓ | stattrak · `stattrak_negev` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::NEGEV) |
| ✓ | quality · `quality_negev` | Slider | Skins / col 1 | AddShowCallback(callbacks::NEGEV) |
| ✓ | seed · `seed_negev` | Slider | Skins / col 1 | AddShowCallback(callbacks::NEGEV) |
| ✓ | paintkit ID · `id_sawedoff` | Edit | Skins / col 1 | AddShowCallback(callbacks::SAWEDOFF) |
| ✓ | stattrak · `stattrak_sawedoff` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::SAWEDOFF) |
| ✓ | quality · `quality_sawedoff` | Slider | Skins / col 1 | AddShowCallback(callbacks::SAWEDOFF) |
| ✓ | seed · `seed_sawedoff` | Slider | Skins / col 1 | AddShowCallback(callbacks::SAWEDOFF) |
| ✓ | paintkit ID · `id_tec9` | Edit | Skins / col 1 | AddShowCallback(callbacks::TEC9) |
| ✓ | stattrak · `stattrak_tec9` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::TEC9) |
| ✓ | quality · `quality_tec9` | Slider | Skins / col 1 | AddShowCallback(callbacks::TEC9) |
| ✓ | seed · `seed_tec9` | Slider | Skins / col 1 | AddShowCallback(callbacks::TEC9) |
| ✓ | paintkit ID · `id_p2000` | Edit | Skins / col 1 | AddShowCallback(callbacks::P2000) |
| ✓ | stattrak · `stattrak_p2000` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::P2000) |
| ✓ | quality · `quality_p2000` | Slider | Skins / col 1 | AddShowCallback(callbacks::P2000) |
| ✓ | seed · `seed_p2000` | Slider | Skins / col 1 | AddShowCallback(callbacks::P2000) |
| ✓ | paintkit ID · `id_mp7` | Edit | Skins / col 1 | AddShowCallback(callbacks::MP7) |
| ✓ | stattrak · `stattrak_mp7` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::MP7) |
| ✓ | quality · `quality_mp7` | Slider | Skins / col 1 | AddShowCallback(callbacks::MP7) |
| ✓ | seed · `seed_mp7` | Slider | Skins / col 1 | AddShowCallback(callbacks::MP7) |
| ✓ | paintkit ID · `id_mp9` | Edit | Skins / col 1 | AddShowCallback(callbacks::MP9) |
| ✓ | stattrak · `stattrak_mp9` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::MP9) |
| ✓ | quality · `quality_mp9` | Slider | Skins / col 1 | AddShowCallback(callbacks::MP9) |
| ✓ | seed · `seed_mp9` | Slider | Skins / col 1 | AddShowCallback(callbacks::MP9) |
| ✓ | paintkit ID · `id_nova` | Edit | Skins / col 1 | AddShowCallback(callbacks::NOVA) |
| ✓ | stattrak · `stattrak_nova` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::NOVA) |
| ✓ | quality · `quality_nova` | Slider | Skins / col 1 | AddShowCallback(callbacks::NOVA) |
| ✓ | seed · `seed_nova` | Slider | Skins / col 1 | AddShowCallback(callbacks::NOVA) |
| ✓ | paintkit ID · `id_p250` | Edit | Skins / col 1 | AddShowCallback(callbacks::P250) |
| ✓ | stattrak · `stattrak_p250` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::P250) |
| ✓ | quality · `quality_p250` | Slider | Skins / col 1 | AddShowCallback(callbacks::P250) |
| ✓ | seed · `seed_p250` | Slider | Skins / col 1 | AddShowCallback(callbacks::P250) |
| ✓ | paintkit ID · `id_scar20` | Edit | Skins / col 1 | AddShowCallback(callbacks::SCAR20) |
| ✓ | stattrak · `stattrak_scar20` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::SCAR20) |
| ✓ | quality · `quality_scar20` | Slider | Skins / col 1 | AddShowCallback(callbacks::SCAR20) |
| ✓ | seed · `seed_scar20` | Slider | Skins / col 1 | AddShowCallback(callbacks::SCAR20) |
| ✓ | paintkit ID · `id_sg553` | Edit | Skins / col 1 | AddShowCallback(callbacks::SG553) |
| ✓ | stattrak · `stattrak_sg553` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::SG553) |
| ✓ | quality · `quality_sg553` | Slider | Skins / col 1 | AddShowCallback(callbacks::SG553) |
| ✓ | seed · `seed_sg553` | Slider | Skins / col 1 | AddShowCallback(callbacks::SG553) |
| ✓ | paintkit ID · `id_ssg08` | Edit | Skins / col 1 | AddShowCallback(callbacks::SSG08) |
| ✓ | stattrak · `stattrak_ssg08` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::SSG08) |
| ✓ | quality · `quality_ssg08` | Slider | Skins / col 1 | AddShowCallback(callbacks::SSG08) |
| ✓ | seed · `seed_ssg08` | Slider | Skins / col 1 | AddShowCallback(callbacks::SSG08) |
| ✓ | paintkit ID · `id_m4a1s` | Edit | Skins / col 1 | AddShowCallback(callbacks::M4A1S) |
| ✓ | stattrak · `stattrak_m4a1s` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::M4A1S) |
| ✓ | quality · `quality_m4a1s` | Slider | Skins / col 1 | AddShowCallback(callbacks::M4A1S) |
| ✓ | seed · `seed_m4a1s` | Slider | Skins / col 1 | AddShowCallback(callbacks::M4A1S) |
| ✓ | paintkit ID · `id_usps` | Edit | Skins / col 1 | AddShowCallback(callbacks::USPS) |
| ✓ | stattrak · `stattrak_usps` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::USPS) |
| ✓ | quality · `quality_usps` | Slider | Skins / col 1 | AddShowCallback(callbacks::USPS) |
| ✓ | seed · `seed_usps` | Slider | Skins / col 1 | AddShowCallback(callbacks::USPS) |
| ✓ | paintkit ID · `id_cz75a` | Edit | Skins / col 1 | AddShowCallback(callbacks::CZ75A) |
| ✓ | stattrak · `stattrak_cz75a` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::CZ75A) |
| ✓ | quality · `quality_cz75a` | Slider | Skins / col 1 | AddShowCallback(callbacks::CZ75A) |
| ✓ | seed · `seed_cz75a` | Slider | Skins / col 1 | AddShowCallback(callbacks::CZ75A) |
| ✓ | paintkit ID · `id_revolver` | Edit | Skins / col 1 | AddShowCallback(callbacks::REVOLVER) |
| ✓ | stattrak · `stattrak_revolver` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::REVOLVER) |
| ✓ | quality · `quality_revolver` | Slider | Skins / col 1 | AddShowCallback(callbacks::REVOLVER) |
| ✓ | seed · `seed_revolver` | Slider | Skins / col 1 | AddShowCallback(callbacks::REVOLVER) |
| ✓ | paintkit ID · `id_bayonet` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BAYONET) |
| ✓ | stattrak · `stattrak_bayonet` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BAYONET) |
| ✓ | quality · `quality_bayonet` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BAYONET) |
| ✓ | seed · `seed_bayonet` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BAYONET) |
| ✓ | paintkit ID · `id_flip` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FLIP) |
| ✓ | stattrak · `stattrak_flip` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FLIP) |
| ✓ | quality · `quality_flip` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FLIP) |
| ✓ | seed · `seed_flip` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FLIP) |
| ✓ | paintkit ID · `id_gut` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_GUT) |
| ✓ | stattrak · `stattrak_gut` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_GUT) |
| ✓ | quality · `quality_gut` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_GUT) |
| ✓ | seed · `seed_gut` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_GUT) |
| ✓ | paintkit ID · `id_karambit` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_KARAMBIT) |
| ✓ | stattrak · `stattrak_karambit` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_KARAMBIT) |
| ✓ | quality · `quality_karambit` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_KARAMBIT) |
| ✓ | seed · `seed_karambit` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_KARAMBIT) |
| ✓ | paintkit ID · `id_m9` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_M9_BAYONET) |
| ✓ | stattrak · `stattrak_m9` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_M9_BAYONET) |
| ✓ | quality · `quality_m9` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_M9_BAYONET) |
| ✓ | seed · `seed_m9` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_M9_BAYONET) |
| ✓ | paintkit ID · `id_huntsman` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_HUNTSMAN) |
| ✓ | stattrak · `stattrak_huntsman` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_HUNTSMAN) |
| ✓ | quality · `quality_huntsman` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_HUNTSMAN) |
| ✓ | seed · `seed_huntsman` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_HUNTSMAN) |
| ✓ | paintkit ID · `id_falchion` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FALCHION) |
| ✓ | stattrak · `stattrak_falchion` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FALCHION) |
| ✓ | quality · `quality_falchion` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FALCHION) |
| ✓ | seed · `seed_falchion` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_FALCHION) |
| ✓ | paintkit ID · `id_bowie` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BOWIE) |
| ✓ | stattrak · `stattrak_bowie` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BOWIE) |
| ✓ | quality · `quality_bowie` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BOWIE) |
| ✓ | seed · `seed_bowie` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BOWIE) |
| ✓ | paintkit ID · `id_butterfly` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BUTTERFLY) |
| ✓ | stattrak · `stattrak_butterfly` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BUTTERFLY) |
| ✓ | quality · `quality_butterfly` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BUTTERFLY) |
| ✓ | seed · `seed_butterfly` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_BUTTERFLY) |
| ✓ | paintkit ID · `id_daggers` | Edit | Skins / col 1 | AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS) |
| ✓ | stattrak · `stattrak_daggers` | Checkbox | Skins / col 1 | AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS) |
| ✓ | quality · `quality_daggers` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS) |
| ✓ | seed · `seed_daggers` | Slider | Skins / col 1 | AddShowCallback(callbacks::KNIFE_SHADOW_DAGGERS) |
| ✓ | knife model · `knife` | Dropdown | Skins / col 2 | Ayeweys ready; picks: Aff, bayonet, bowie, butterfly, falchion, flip, gut, huntsman, karambit, m9 bayonet, daggers |
| ✓ | glove model · `glove` | Dropdown | Skins / col 2 | Ayeweys ready; picks: Aff, bloodhound, sport, driver, handwraps, moto, specialist |
| ✓ | glove paintkit ID · `glove_id` | Edit | Skins / col 2 | Ayeweys ready |

## Ither (14)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Auto-buy things · `buy1` | MultiDropdown | Ither / col 1 | Ayeweys ready; picks: galilar, famas, ak47, m4a1, m4a1_silencer, ssg08, aug, sg556, awp, scar20, g3sg1, nova, xm1014, mag7, m249, negev, mac10, mp9, mp7, ump45, p90, bizon |
| ✓ | (nae label: auto_buy2) · `buy2` | MultiDropdown | Ither / col 1 | Ayeweys ready; picks: glock, hkp2000, usp_silencer, elite, p250, tec9, fn57, deagle |
| ✓ | (nae label: auto_buy3) · `buy3` | MultiDropdown | Ither / col 1 | Ayeweys ready; picks: vest, vesthelm, taser, defuser, heavyarmor, molotov, incgrenade, decoy, flashbang, hegrenade, smokegrenade |
| ✓ | notices · `notifications` | MultiDropdown | Ither / col 1 | Ayeweys ready; picks: matchmaking, skaith, buys, bomb, defuse |
| ✓ | Last tick defuse · `last_tick_defuse` | Keybind | Ither / col 1 | Ayeweys ready |
| ✓ | Fake latency · `fake_latency` | Keybind | Ither / col 1 | Ayeweys ready |
| ✓ | (nae label: fake_latency_amt) · `fake_latency_amt` | Slider | Ither / col 1 | Ayeweys ready |
| ✓ | Auto-accept matchmaking · `autoaccept` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | Lowse inventory in-game · `unlock` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | hitmarker · `hitmarker` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | ragdoll force · `ragdoll_force` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | Shaw matchmaking ranks · `ranks` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | Keep killfeed · `killfeed` | Checkbox | Ither / col 2 | Ayeweys ready |
| ✓ | clantag · `clantag` | Checkbox | Ither / col 2 | Ayeweys ready |

## Settins (11)

| Here | Option / member | Kind | Current place | Existing conditions / picks |
|---|---|---|---|---|
| ✓ | Menu colour · `menu_color` | Colorpicker | Settins / col 1 | Ayeweys ready |
| ✓ | safe mode · `mode` | Dropdown | Settins / col 2 | Ayeweys ready; picks: matchmaking, no-spread |
| ✓ | Settin · `config` | Dropdown | Settins / col 2 | Ayeweys ready; picks: 1, 2, 3, 4, 5, 6 |
| ✓ | Settin key 1 · `key1` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig1) |
| ✓ | Settin key 2 · `key2` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig2) |
| ✓ | Settin key 3 · `key3` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig3) |
| ✓ | Settin key 4 · `key4` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig4) |
| ✓ | Settin key 5 · `key5` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig5) |
| ✓ | Settin key 6 · `key6` | Keybind | Settins / col 2 | AddShowCallback(callbacks::IsConfig6) |
| ✓ | Hain · `save` | Button | Settins / col 2 | Ayeweys ready |
| ✓ | Lade · `load` | Button | Settins / col 2 | Ayeweys ready |

