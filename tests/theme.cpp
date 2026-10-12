#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstdint>
#include <filesystem>
#include <fstream>
#include <iomanip>
#include <iostream>
#include <memory>
#include <sstream>
#include <string>
#include <vector>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#define XOR(value) value
struct Point { int x, y; };
struct Rect { int x, y, w, h; };
namespace math { template<class T> void clamp(T& value, T low, T high) { value = std::clamp(value, low, high); } }
#include "../color.h"
#include "../theme.h"
#include "../scots_locale.h"
#include "../element.h"
#include "../checkbox.h"
#include "../dropdown.h"
#include "../slider.h"
#include "../button.h"
#include "../multidropdown.h"
#include "../edit.h"
#include "../keybind.h"
#include "../colorpicker.h"
#include "../tab.h"
#include "../form.h"

// SVG adapter records the production drawing calls for inspection without the game.
namespace render {
    std::ostringstream svg;
    int primitives = 0;
    enum StringFlags_t { ALIGN_LEFT, ALIGN_RIGHT, ALIGN_CENTER };
    struct FontSize_t { int m_width, m_height; };
    std::string escape(const std::string& text) {
        std::string out;
        for(char c : text) {
            if(c == '&') out += "&amp;";
            else if(c == '<') out += "&lt;";
            else if(c == '>') out += "&gt;";
            else if(c == '"') out += "&quot;";
            else out += c;
        }
        return out;
    }
    std::string paint(Color color) {
        return "rgb(" + std::to_string(color.r()) + "," + std::to_string(color.g()) + "," + std::to_string(color.b()) + ")";
    }
    void rect_filled(int x, int y, int w, int h, Color color) {
        assert(w >= 0 && h >= 0);
        svg << "<rect x='" << x << "' y='" << y << "' width='" << w << "' height='" << h << "' fill='" << paint(color) << "' fill-opacity='" << color.a()/255.f << "'/>\n";
        ++primitives;
    }
    void rect(int x, int y, int w, int h, Color color) {
        assert(w >= 0 && h >= 0);
        svg << "<rect x='" << x << "' y='" << y << "' width='" << w << "' height='" << h << "' fill='none' stroke='" << paint(color) << "' stroke-opacity='" << color.a()/255.f << "'/>\n";
        ++primitives;
    }
    void line(int x, int y, int end_x, int end_y, Color color) {
        svg << "<path d='M" << x << ',' << y << 'L' << end_x << ',' << end_y << "' stroke='" << paint(color) << "' stroke-opacity='" << color.a()/255.f << "'/>\n";
        ++primitives;
    }
    void circle(int x, int y, int radius, int, Color color) {
        svg << "<circle cx='" << x << "' cy='" << y << "' r='" << radius << "' fill='" << paint(color) << "' fill-opacity='" << color.a()/255.f << "'/>\n";
        ++primitives;
    }
    void gradient(int x, int y, int w, int h, Color top, Color bottom) {
        const auto id = "gradient" + std::to_string(primitives++);
        svg << "<defs><linearGradient id='" << id << "' x2='0' y2='1'><stop stop-color='" << paint(top) << "'/><stop offset='1' stop-color='" << paint(bottom) << "'/></linearGradient></defs>";
        svg << "<rect x='" << x << "' y='" << y << "' width='" << w << "' height='" << h << "' fill='url(#" << id << ")' opacity='" << top.a()/255.f << "'/>\n";
    }
    void rect_filled_fade(int x, int y, int w, int h, Color color, int first, int last) {
        Color a = color, b = color;
        a.a() = static_cast<uint8_t>(first); b.a() = static_cast<uint8_t>(last);
        // The production adapter performs a surface fade; preserve the overlay's end alpha.
        color.a() = static_cast<uint8_t>(last / 2);
        rect_filled(x, y, w, h, color);
    }
    struct Font {
        int pixels = 14;
        const char* family = "IBM Plex Sans, sans-serif";
        int weight = 400;
        void string(int x, int y, Color color, const std::string& text, StringFlags_t flags = ALIGN_LEFT) {
            svg << "<text x='" << x << "' y='" << y + pixels << "' font-family='" << family << "' font-size='" << pixels << "' font-weight='" << weight << "' text-anchor='" << (flags == ALIGN_RIGHT ? "end" : flags == ALIGN_CENTER ? "middle" : "start") << "' fill='" << paint(color) << "' fill-opacity='" << color.a()/255.f << "'>" << escape(text) << "</text>\n";
        }
        FontSize_t size(const std::string& text) { return {static_cast<int>(text.size()) * 7, pixels}; }
        FontSize_t wsize(const std::wstring& text) { return {static_cast<int>(text.size()) * 7, pixels}; }
        void wstring(int x, int y, Color color, const std::wstring& text) { string(x,y,color,std::string(text.begin(),text.end())); }
    } menu, menu_shade, brand{28, "IBM Plex Sans, sans-serif", 500};
}
#include "../theme_art.h"
struct Globals { float m_frametime = .5f; } globals;
struct Surface {
    int CreateNewTextureID(bool) { return 1; }
    void DrawSetTextureRGBA(int, Color*, int, int) {}
    void DrawSetColor(Color) {}
    void DrawSetTexture(int) {}
    void DrawTexturedRect(int, int, int, int) {}
} surface;
struct Game { Globals* m_globals = &globals; Surface* m_surface = &surface; } g_csgo;
struct Host { int GetTickCount() { return 0; } } g_winapi;
struct GUI { Color m_color = theme::teal(); } g_gui;
#define STUB_INPUT(type) void type::think() {} void type::click() {}
STUB_INPUT(Checkbox)
STUB_INPUT(Dropdown)
STUB_INPUT(Slider)
STUB_INPUT(Button)
STUB_INPUT(MultiDropdown)
STUB_INPUT(Edit)
STUB_INPUT(Keybind)
STUB_INPUT(Colorpicker)
#include "form_draw_under_test.cpp"
#include "checkbox_draw_under_test.cpp"
#include "dropdown_draw_under_test.cpp"
#include "slider_draw_under_test.cpp"
#include "button_draw_under_test.cpp"
#include "multidropdown_draw_under_test.cpp"
#include "edit_draw_under_test.cpp"
#include "keybind_draw_under_test.cpp"
#include "colorpicker_draw_under_test.cpp"

int main(int argc, char** argv) {
    assert(argc == 2);
    Form form;
    form.SetPosition(10, 10); form.SetSize(780, 720);
    Tab chams; chams.SetTitle("chams style"); form.RegisterTab(&chams);
    std::array<Tab,8> tabs;
    const char* names[] = {"aimbot", "anti-aim", "players", "visuals", "movement", "skins", "misc", "config"};
    for(size_t i=0;i<tabs.size();++i) { tabs[i].SetTitle(names[i]); form.RegisterTab(&tabs[i]); }
    Dropdown material;
    material.setup("material", "material", {"original", "flat", "wireframe"}); material.set(2);
    chams.RegisterElement(&material); material.SetPosition(20, 24);
    struct DemoSlider : Slider { void fill(int value) { m_fill = value; } } opacity;
    opacity.setup("opacity when scoped", "scoped_blend", 0, 100, true, 0, 50, 1, L"%");
    chams.RegisterElement(&opacity); opacity.SetPosition(20, 96); opacity.fill(90);
    Checkbox enabled;
    enabled.setup("blend when scoped", "scope", true, true);
    chams.RegisterElement(&enabled); enabled.SetPosition(20, 168);
    struct DemoButton : Button { DemoButton() { m_show = true; } } save;
    save.setup("save configuration"); chams.RegisterElement(&save); save.SetPosition(20, 216);

    const Rect body = form.GetElementsRect(), first = form.GetTabRect(0), second = form.GetTabRect(1);
    assert(second.y - first.y == theme::tab_height);
    assert(first.x >= form.GetTabsRect().x && first.x + first.w <= form.GetTabsRect().x + form.GetTabsRect().w);
    assert(body.x > first.x + first.w && body.y > form.GetFormRect().y + theme::header_height);
    form.draw();
    assert(form.m_alpha == 255 && render::primitives > 40);
    const auto picture = render::svg.str();
    assert(picture.find("COCAINE") != std::string::npos && picture.find("whan scoped") != std::string::npos && picture.find("Hain settin") != std::string::npos);
    std::filesystem::create_directories(argv[1]);
    std::ofstream out(std::filesystem::path(argv[1]) / "cocaine-theme.svg");
    out << "<svg xmlns='http://www.w3.org/2000/svg' width='800' height='740' viewBox='0 0 800 740'>" << picture << "</svg>";
    out.close(); assert(out.good());
    // Compile and exercise the remaining themed controls after capturing the sample.
    MultiDropdown multi; multi.setup("layers", "layers", {"visible", "hidden"}); multi.select(0);
    chams.RegisterElement(&multi); multi.SetPosition(268, 24);
    Edit edit; edit.setup("number", "number"); edit.set(42); chams.RegisterElement(&edit); edit.SetPosition(268, 96);
    Keybind key; key.setup("hotkey", "hotkey", 65); chams.RegisterElement(&key); key.SetPosition(268, 168);
    Colorpicker color; color.setup("accent", "accent", theme::gold()); chams.RegisterElement(&color); color.SetPosition(268, 216);
    form.draw();
    const int before = render::primitives;
    form.m_open = false; form.draw();
    assert(form.m_alpha == 0 && render::primitives == before);
    std::cout << "Theme rendering and layout checks passed.\n";
}
