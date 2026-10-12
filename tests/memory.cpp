// Host adapter around the actual Player::GetBonePosition method.
#include <cassert>
#include <type_traits>
#include <vector>
#ifndef _MSC_VER
#define __forceinline inline
#endif
struct vec3_t {
    float x, y, z;
    vec3_t(float a, float b, float c) : x(a), y(b), z(c) {}
};
using matrix3x4a_t = float[3][4];
unsigned long long GetTickCount64() { return 1000; }
class Player {
public:
    bool succeeds = true;
    int calls = 0;
    bool SetupBones(matrix3x4a_t* matrices, int count, int, unsigned long long) {
        ++calls;
        if (!succeeds) return false;
        for (int i = 0; i < count; ++i) {
            matrices[i][0][3] = static_cast<float>(i);
            matrices[i][1][3] = 2;
            matrices[i][2][3] = 3;
        }
        return true;
    }
#include "bone_position_under_test.h"
};

struct Impact { int m_tickbase; };
struct CheckedImpacts {
    std::vector<Impact> values;
    std::size_t size() const { return values.size(); }
    Impact& operator[](std::size_t index) { return values.at(index); }
};
bool final_impact(CheckedImpacts& values, std::size_t i, std::size_t impact_count) {
    auto* vis_impacts = &values;
    auto* impact = &values[i];
    bool is_final_impact = false;
    (void)impact_count; // Represents the cached count before expired entries were erased.
#include "impact_boundary_under_test.cpp"
    return is_final_impact;
}

int main() {
    Player player;
    static_assert(!std::is_reference<decltype(player.GetBonePosition(0))>::value, "Return owned data");
    const auto first = player.GetBonePosition(0);
    const auto last = player.GetBonePosition(127);
    assert(first.x == 0 && first.y == 2 && first.z == 3);
    assert(last.x == 127 && last.y == 2 && last.z == 3);
    assert(player.GetBonePosition(-1).x == 0);
    assert(player.GetBonePosition(128).x == 0);
    assert(player.calls == 2); // Invalid indices must be rejected before calling the engine.
    player.succeeds = false;
    const auto missing = player.GetBonePosition(4);
    assert(missing.x == 0 && missing.y == 0 && missing.z == 0);

    CheckedImpacts impacts{{{10}, {10}, {20}}};
    assert(!final_impact(impacts, 0, 3));
    assert(final_impact(impacts, 1, 3));
    assert(final_impact(impacts, 2, 3));
    impacts.values.erase(impacts.values.begin() + 1, impacts.values.end());
    // An expired trailing impact must not leave a stale neighbor index.
    assert(final_impact(impacts, 0, 3));
}
