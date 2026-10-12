#pragma once

#include <cmath>
#include <cstddef>

namespace feature_settings {
    struct AntiAimPreset {
        std::size_t pitch;
        std::size_t yaw;
        float jitter;
        float rotation;
        float speed;
    };

    // Presets configure the selected state; global enable/fake-yaw/lag stay user-controlled.
    inline AntiAimPreset anti_aim_preset( std::size_t index ) {
        switch( index ) {
        case 1: return { 1, 1, 45.f, 360.f, 10.f };
        case 2: return { 1, 2, 45.f, 360.f, 10.f };
        case 3: return { 1, 3, 45.f, 360.f, 10.f };
        default: return { 0, 0, 45.f, 360.f, 10.f };
        }
    }

    inline float rotation_phase( float time, float speed, float range ) {
        if( !std::isfinite( time ) || !std::isfinite( speed ) || !std::isfinite( range ) || range <= 0.f )
            return 0.f;
        const double phase = std::fmod( static_cast<double>( time ) * speed * 20.0, range );
        return static_cast<float>( phase < 0.0 ? phase + range : phase );
    }
}
