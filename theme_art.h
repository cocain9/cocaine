#pragma once

#include <cmath>

namespace theme {
    inline void accent_line( int x, int y, int width, int alpha ) {
        render::rect_filled(x, y, width, 1, border(alpha));
        render::rect_filled(x, y, width / 8, 1, teal(alpha));
    }

    inline void rose( int x, int y, int alpha ) {
        render::line( x, y, x, y + 24, teal(alpha) );
        render::line( x, y + 16, x - 9, y + 10, teal(alpha) );
        render::line( x, y + 20, x + 10, y + 14, teal(alpha) );
        for( int i = 0; i < 5; ++i ) {
            const float angle = i * 6.2831853f / 5.f;
            render::circle( x + static_cast<int>(std::cos(angle) * 6),
                y + static_cast<int>(std::sin(angle) * 6), 6, 16, scarlet(alpha) );
        }
        render::circle( x, y, 5, 16, gold(alpha) );
        render::circle( x, y, 2, 12, canvas(alpha) );
    }

    inline void header( int x, int y, int width, int alpha ) {
        render::rect_filled(x, y, width, header_height, panel(alpha));
        render::brand.string(x + 28, y + 20, ink(alpha), "COCAINE");
        render::menu.string(x + 29, y + 59, muted(alpha), scots::display("CONTROL ROOM"));
        // Quiet elemental accents: wood, fire, earth, metal and water.
        const Color colors[] = { teal(alpha), scarlet(alpha), gold(alpha), ink(alpha), blue(alpha) };
        for (int i = 0; i < 5; ++i)
            render::circle(x + width - 112 + i * 18, y + 44, 3, 12, colors[i]);
        accent_line(x + 28, y + header_height - 1, width - 56, alpha);
    }
}
