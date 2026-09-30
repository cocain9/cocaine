// Compile the real Release-mode helpers, without the UI tests' XOR adapter.
#include <array>
#include <cassert>
#include <cstddef>
#include <cstdint>
#include <cstring>
#include <string>
#include <utility>
#ifndef _MSC_VER
#define __forceinline inline
#endif
#ifdef _DEBUG
#undef _DEBUG
#endif
#include "../hash.h"
#include "../xorstr.h"

static_assert(FNV1a::get("") == 0x811c9dc5u, "Empty strings hash to the FNV basis");
static_assert(FNV1a::get("hello") == 0x4f9f2cabu, "Known FNV-1a vector");

const char* persistent_name() { return XOR("Cocaine"); }
const char* different_name() { return XOR("another"); }
const wchar_t* persistent_wide() { return XOR(L"Scots \u00e9"); }

int main() {
    const auto* saved = persistent_name();
    const auto* other = different_name();
    const auto* wide = persistent_wide();
    for (int i = 0; i < 1000; ++i) {
        // Same-length strings must have distinct owners and must not toggle
        // back to encoded data on repeated calls.
        assert(persistent_name() == saved);
        assert(std::strcmp(saved, "Cocaine") == 0);
        assert(std::strcmp(other, "another") == 0);
        assert(persistent_wide() == wide);
        assert(std::wstring(wide) == L"Scots \u00e9");
    }
    assert(std::string(XOR("")) == "");
    assert(FNV1a::get(std::string()) == FNV1a::get(""));
}
