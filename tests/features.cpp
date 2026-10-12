#define COCAINE_FEATURE_TESTS
#include "support.h"
#include "../feature_settings.h"
#include "config_under_test.cpp"
#include <iostream>
#include <map>

enum AntiAimMode : size_t { STAND, WALK, AIR };
struct Notifications { void add(const std::string&) {} } g_notify;
#include "preset_under_test.cpp"

// Record calls to the rendering boundary while executing the production material setup.
enum { MATERIAL_VAR_WIREFRAME, MATERIAL_VAR_ZNEARER, MATERIAL_VAR_NOFOG, MATERIAL_VAR_IGNOREZ };
struct IMaterial {
    std::map<int, bool> flags;
    void SetFlag(int flag, bool value) { flags[flag] = value; }
};
struct StudioRender {
    IMaterial* forced = nullptr;
    void ForcedMaterialOverride(IMaterial* material) { forced = material; }
} studio;
struct RenderView {
    Color color;
    float blend = 0.f;
    void SetColorModulation(Color value) { color = value; }
    void SetBlend(float value) { blend = value; }
} render_view;
struct RenderHost {
    StudioRender* m_studio_render = &studio;
    RenderView* m_render_view = &render_view;
} render_host;
struct Chams {
    IMaterial *debugdrawflat = nullptr, *debugambientcube = nullptr;
    std::vector<int> m_players;
    bool SortPlayers() { return false; }
    void RenderPlayer(int) {}
    void SceneEnd();
    Color color;
    void SetColor(Color value) { color = value; }
    void SetupMaterial(IMaterial* material, Color color, bool z_flag);
};
// Only the host object is adapted; the function body is copied verbatim by the runner.
#define g_csgo render_host
#include "material_under_test.cpp"
#include "scene_end_under_test.cpp"
#undef g_csgo

int main(int argc, char** argv) {
    assert(argc == 2);
    auto& menu = g_menu.main;
    menu.RegisterTab(&menu.antiaim);
    menu.antiaim.init();
    menu.RegisterTab(&menu.chams_style);
    menu.chams_style.init();
    auto& aa = menu.antiaim;
    assert(menu.chams_style.material.get() == 0);
    assert(menu.chams_style.scoped_blend.get() == 50);
    aa.enable.set(true);
    aa.fake_yaw.set(3);
    aa.lag_enable.set(true);
    aa.mode.set(0);
    aa.preset.set(2);
    callbacks::ApplyAntiAimPreset();
    assert(aa.pitch_stand.get() == 1 && aa.yaw_stand.get() == 2);
    assert(aa.jitter_range_stand.get() == 45 && aa.dir_stand.get() == 1);
    assert(aa.yaw_walk.get() == 0 && aa.yaw_air.get() == 0);
    aa.mode.set(1); aa.preset.set(3); callbacks::ApplyAntiAimPreset();
    assert(aa.yaw_walk.get() == 3 && aa.rot_range_walk.get() == 360);
    aa.mode.set(2); aa.preset.set(1); callbacks::ApplyAntiAimPreset();
    assert(aa.yaw_air.get() == 1);
    aa.mode.set(0); aa.preset.set(0); callbacks::ApplyAntiAimPreset();
    assert(aa.pitch_stand.get() == 0 && aa.yaw_stand.get() == 0);
    assert(aa.yaw_walk.get() == 3 && aa.yaw_air.get() == 1);
    assert(aa.enable.get() && aa.fake_yaw.get() == 3 && aa.lag_enable.get());

    for(float range : {0.f, -1.f, std::numeric_limits<float>::quiet_NaN()})
        assert(feature_settings::rotation_phase(1, 10, range) == 0);
    assert(feature_settings::rotation_phase(1, 10, 360) == 200);
    assert(feature_settings::rotation_phase(-1, 10, 360) == 160);
    assert(feature_settings::rotation_phase(std::numeric_limits<float>::infinity(), 10, 360) == 0);
    assert(std::isfinite(feature_settings::rotation_phase(std::numeric_limits<float>::max(), 100, 360)));

    IMaterial lit, flat;
    Chams chams;
    chams.debugdrawflat = &flat;
    chams.debugambientcube = &lit;
    menu.chams_style.material.set(0);
    chams.SetupMaterial(&lit, Color(1, 2, 3), false);
    assert(studio.forced == &lit && !lit.flags[MATERIAL_VAR_WIREFRAME]);
    menu.chams_style.material.set(1);
    chams.SetupMaterial(&lit, Color(4, 5, 6), true);
    assert(studio.forced == &flat && !flat.flags[MATERIAL_VAR_WIREFRAME] && flat.flags[MATERIAL_VAR_IGNOREZ]);
    menu.chams_style.material.set(2);
    chams.SetupMaterial(&lit, Color(4, 5, 6), false);
    assert(studio.forced == &flat && flat.flags[MATERIAL_VAR_WIREFRAME] && !flat.flags[MATERIAL_VAR_IGNOREZ]);
    chams.SceneEnd();
    assert(!flat.flags[MATERIAL_VAR_WIREFRAME] && !flat.flags[MATERIAL_VAR_IGNOREZ]);
    assert(studio.forced == nullptr && render_view.blend == 1.f && render_view.color.r() == 255);
    menu.chams_style.material.set(1);
    chams.SetupMaterial(&lit, Color(7, 8, 9), false);
    assert(!flat.flags[MATERIAL_VAR_WIREFRAME] && chams.color.r() == 7);
    studio.forced = nullptr; chams.debugdrawflat = nullptr;
    chams.SetupMaterial(&lit, Color(), false);
    assert(studio.forced == nullptr);

    test_documents = argv[1];
    g_config.init();
    menu.chams_style.material.set(2);
    menu.chams_style.scoped_blend.set(23);
    assert(g_config.save(&menu, "1.cfg"));
    menu.chams_style.material.set(0);
    menu.chams_style.scoped_blend.set(50);
    aa.yaw_air.set(0);
    assert(g_config.load(&menu, "1.cfg"));
    assert(menu.chams_style.material.get() == 2 && menu.chams_style.scoped_blend.get() == 23);
    assert(aa.yaw_air.get() == 1);
    // Old files without the new tab keep default/new values intact.
    assert(config_store::write(std::filesystem::path(argv[1]) / "Cocaine" / "2.cfg",
        nlohmann::json{{"anti-aim", {{"yaw_air", 2}}}}));
    assert(g_config.load(&menu, "2.cfg"));
    assert(aa.yaw_air.get() == 2 && menu.chams_style.scoped_blend.get() == 23);
    std::cout << "All chams and anti-aim feature checks passed.\n";
}
