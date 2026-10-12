#pragma once
// Minimal host/render adapters. Controls, serialization and Config methods are production code.
#include <algorithm>
#include <array>
#include <cassert>
#include <cmath>
#include <cstring>
#include <cstdarg>
#include <cstdio>
#include <functional>
#include <limits>
#include <memory>
#include <string>
#include <vector>
#include "../config_store.h"
#ifndef _MSC_VER
#define __forceinline inline
#endif
#define XOR(value) value
struct Point { int x, y; };
struct Rect { int x, y, w, h; };
namespace math {
    template<class T> void clamp( T& value, T min, T max ) { value = std::clamp( value, min, max ); }
}
#include "../color.h"
#include "../theme.h"
#include "../scots_locale.h"
#include "../element.h"
#include "../checkbox.h"
#include "../dropdown.h"
#include "../multidropdown.h"
#include "../slider.h"
#include "../keybind.h"
#include "../colorpicker.h"
#include "../edit.h"
#include "../button.h"
#include "../tab.h"
#include "../form.h"
#include "../config.h"
#define STUB_RENDER(type) void type::draw() {} void type::think() {} void type::click() {}
STUB_RENDER(Checkbox)
STUB_RENDER(Dropdown)
STUB_RENDER(MultiDropdown)
STUB_RENDER(Slider)
STUB_RENDER(Keybind)
STUB_RENDER(Colorpicker)
STUB_RENDER(Edit)
STUB_RENDER(Button)
#undef STUB_RENDER

#ifndef _WIN32
constexpr int MAX_PATH = 260;
#define FAILED(value) ((value) < 0)
#endif
constexpr int CSIDL_PERSONAL = 5;
constexpr int SHGFP_TYPE_CURRENT = 0;
inline std::string test_documents;
struct Host {
    int GetTickCount() { return 1; }
    int SHGetFolderPathA( void*, int, void*, int, char* path ) {
        if( test_documents.size() >= MAX_PATH ) return -1;
        std::strcpy( path, test_documents.c_str() );
        return 0;
    }
} inline g_winapi;
struct Client { std::string m_user = "user"; void print(const std::string& text); } inline g_cl;
struct TestKeys { Keybind key1, key2, key3, key4, key5, key6; };
#ifdef COCAINE_FEATURE_TESTS
#include "feature_menu_under_test.h"
struct TestMain : Form { TestKeys config; AntiAimTab antiaim; ChamsStyleTab chams_style; };
#else
struct TestMain { TestKeys config; };
#endif
struct TestMenu { TestMain main; } inline g_menu;

#include "../gui.h"
inline std::string printed;
struct TestConsole {
    void ConsoleColorPrintf(Color, const char* format, ...) {
        va_list args;
        va_start(args, format);
        char text[1024]{};
        std::vsnprintf(text, sizeof(text), format, args);
        va_end(args);
        printed += text;
    }
} inline test_console;
struct TestGame { TestConsole* m_cvar = &test_console; } inline g_csgo;
