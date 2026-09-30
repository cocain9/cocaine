#pragma once

// Five-element-inspired accents on neutral surfaces; readable contrast takes priority.
namespace theme {
    inline Color canvas( int alpha = 255 ) { return { 19, 24, 26, alpha }; }
    inline Color panel( int alpha = 255 ) { return { 25, 32, 34, alpha }; }
    inline Color field( int alpha = 255 ) { return { 34, 43, 45, alpha }; }
    inline Color border( int alpha = 255 ) { return { 63, 76, 78, alpha }; }
    inline Color ink( int alpha = 255 ) { return { 234, 235, 227, alpha }; }
    inline Color muted( int alpha = 255 ) { return { 164, 179, 178, alpha }; }
    inline Color scarlet( int alpha = 255 ) { return { 195, 102, 97, alpha }; }
    inline Color blue( int alpha = 255 ) { return { 119, 154, 175, alpha }; }
    inline Color teal( int alpha = 255 ) { return { 159, 192, 167, alpha }; }
    inline Color gold( int alpha = 255 ) { return { 196, 174, 129, alpha }; }
    inline Color violet( int alpha = 255 ) { return { 175, 164, 181, alpha }; }

    constexpr int header_height = 104;
    constexpr int sidebar_width = 144;
    constexpr int tab_height = 32;
}
