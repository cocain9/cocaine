#include <cassert>
#include <cstddef>
#include <cstdint>
#include <iostream>
#include <limits>
#include <memory>
#include <string>
#include <vector>

#ifdef _WIN32
#include <Windows.h>
#else
#define __forceinline inline
struct MEMORY_BASIC_INFORMATION { unsigned long State, Protect; };
constexpr unsigned long PAGE_NOACCESS = 1, PAGE_GUARD = 0x100;
#endif

// Address::valid is not used by the buffer scanner. Keep its host dependency isolated.
struct QueryAdapter {
    size_t VirtualQuery(const void*, MEMORY_BASIC_INFORMATION*, size_t) { return 0; }
} g_winapi;
#include "../address.h"

namespace PE {
    class Module {
        Address base;
        size_t size;
    public:
        Module(Address address = {}, size_t length = 0) : base(address), size(length) {}
        Address GetBase() const { return base; }
        size_t GetImageSize() const { return size; }
        std::string GetName() const { return "fixture.dll"; }
    };
}
#include "../pattern.h"

int main() {
    // Exact heap size lets ASan detect attempts to read beyond the scan buffer.
    auto data = std::make_unique<uint8_t[]>(5);
    data[0] = 0xAB; data[1] = 0xAB; data[2] = 0xAB; data[3] = 0x00; data[4] = 0xFF;
    Address base(data.get());
    const auto address = base.as<uintptr_t>();
    assert(pattern::find(base, 5, "ab AB").as<uintptr_t>() == address);
    assert(pattern::find(base, 5, "00 ff").as<uintptr_t>() == address + 3);
    assert(pattern::find(base, 5, "FF").as<uintptr_t>() == address + 4);
    assert(pattern::find(base, 5, "AB AB AB 00 FF").as<uintptr_t>() == address);
    assert(pattern::find(base, 5, "AB ? ?? 00 FF").as<uintptr_t>() == address);
    assert(pattern::find(base, 5, "00 ?").as<uintptr_t>() == address + 3);
    assert(pattern::find(base, 5, "? ??").as<uintptr_t>() == address);
    assert(pattern::find(base, 5, "\t00\n FF \r").as<uintptr_t>() == address + 3);
    for(const std::string text : {"FF 00", "FF ?", "?? ?? ?? ?? ?? ??", "CD"}) {
        assert(!pattern::find(base, 5, text).as<uintptr_t>());
        assert(pattern::FindAll(base, 5, text).empty());
    }
    for(const std::string text : {"", " \t\n", "garbage", "A", "AB invalid", "AB F",
        "AB 0xFF", "AB FF00", "AB ???", "AB ?F", "AB F?", "AB ?garbage", "AB GG"}) {
        assert(!pattern::find(base, 5, text).as<uintptr_t>());
        assert(pattern::FindAll(base, 5, text).empty());
    }
    assert(!pattern::find(base, 5, std::string("AB ") + char(0xFF) + char(0xFF)).as<uintptr_t>());
    assert(!pattern::find(base, 5, std::string("AB\0 FF", 6)).as<uintptr_t>());
    assert(!pattern::find(Address{}, 5, "AB").as<uintptr_t>());
    assert(!pattern::find(base, 0, "AB").as<uintptr_t>());
    assert(pattern::FindAll(Address{}, 5, "AB").empty());
    // Wrapped and unrepresentable numeric ranges are rejected before any dereference.
    Address near_end(std::numeric_limits<uintptr_t>::max() - 1);
    assert(!pattern::find(near_end, 3, "AB").as<uintptr_t>());
    assert(pattern::FindAll(near_end, 3, "AB").empty());
    assert(!pattern::find(base, std::numeric_limits<size_t>::max(), "AB").as<uintptr_t>());

    auto matches = pattern::FindAll(base, 5, "AB AB");
    assert(matches.size() == 2);
    assert(matches[0].as<uintptr_t>() == address && matches[1].as<uintptr_t>() == address + 1);
    matches = pattern::FindAll(base, 5, "??");
    assert(matches.size() == 5);
    for(size_t i = 0; i < matches.size(); ++i) assert(matches[i].as<uintptr_t>() == address + i);
    matches = pattern::FindAll(base, 5, "?? ??");
    assert(matches.size() == 4 && matches.back().as<uintptr_t>() == address + 3);
    PE::Module module(base, 5);
    assert(pattern::find(module, "FF").as<uintptr_t>() == address + 4);
    assert(pattern::FindAll(module, "AB").size() == 3);
    assert(!pattern::find(PE::Module{}, "AB").as<uintptr_t>());
    assert(pattern::FindAll(PE::Module{}, "AB").empty());
    assert(pattern::find_unique(base, 5, "00 FF").as<uintptr_t>() == address + 3);
    assert(!pattern::find_unique(base, 5, "AB AB").as<uintptr_t>()); // overlapping ambiguity
    assert(!pattern::find_unique(base, 5, "CD").as<uintptr_t>());
    assert(!pattern::find_unique(base, 5, "garbage").as<uintptr_t>());
    assert(!pattern::find_unique(base, 5, "?").as<uintptr_t>());
    assert(!pattern::find_unique(Address{}, 5, "AB").as<uintptr_t>());
    assert(!pattern::find_unique(near_end, 3, "AB").as<uintptr_t>());
    assert(pattern::require(module, "00 FF").as<uintptr_t>() == address + 3);
    for(const auto text : {"AB", "CD", "garbage"}) {
        bool rejected = false;
        try { pattern::require(module, text); }
        catch(const std::runtime_error& error) {
            const std::string message = error.what();
            rejected = message.find("fixture.dll") != std::string::npos
                && message.find(text) != std::string::npos;
        }
        assert(rejected);
    }
    // No module-byte cache: subsequent scans see changed contents.
    data[4] = 0xEE;
    assert(!pattern::find(module, "FF").as<uintptr_t>());
    assert(pattern::find(module, "EE").as<uintptr_t>() == address + 4);
    std::cout << "All pattern scanner checks passed.\n";
}
