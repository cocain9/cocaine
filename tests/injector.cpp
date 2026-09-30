#include "../injector/dll_image.h"
#include <cassert>
#include <iostream>
#include <sstream>
#include <string>

namespace {
    void word(std::string& image, std::size_t offset, unsigned value) {
        image[offset] = static_cast<char>(value & 255);
        image[offset + 1] = static_cast<char>((value >> 8) & 255);
    }
    void dword(std::string& image, std::size_t offset, unsigned value) {
        word(image, offset, value);
        word(image, offset + 2, value >> 16);
    }
    std::string fixture(bool x64 = false) {
        std::string image(1024, '\0');
        image[0] = 'M'; image[1] = 'Z'; image[60] = 64;
        image[64] = 'P'; image[65] = 'E';
        word(image, 68, x64 ? 0x8664 : 0x14c);
        word(image, 70, 1); word(image, 84, x64 ? 240 : 224);
        word(image, 86, 0x2002); word(image, 88, x64 ? 0x20b : 0x10b);
        dword(image, 88 + 16, 4096); // Entry point.
        dword(image, 88 + 56, 8192); // SizeOfImage.
        dword(image, 88 + 60, 512);  // SizeOfHeaders, including the section table.
        const auto section = 88 + (x64 ? 240 : 224);
        dword(image, section + 8, 256);
        dword(image, section + 12, 4096);
        dword(image, section + 16, 512);
        dword(image, section + 20, 512);
        return image;
    }
    unsigned machine(const std::string& data) {
        std::istringstream stream(data, std::ios::binary);
        return cocaine::dll_machine(stream);
    }
    void rejects(const std::string& data) {
        try { machine(data); assert(false && "Malformed DLL accepted"); }
        catch (const std::runtime_error&) {}
    }
}
int main() {
    assert(machine(fixture()) == 0x14c);
    assert(machine(fixture(true)) == 0x8664);
    rejects("");
    auto data = fixture(); data[0] = 'N'; rejects(data);
    data = fixture(); data[64] = 'X'; rejects(data);
    data = fixture(); word(data, 86, 2); rejects(data); // EXE, not DLL.
    data = fixture(); word(data, 86, 0x2000); rejects(data); // Not executable.
    data = fixture(); word(data, 68, 0xaa64); rejects(data); // Unsupported architecture.
    data = fixture(); word(data, 88, 0x20b); rejects(data); // Mismatched optional header.
    data = fixture(); word(data, 84, 2); rejects(data);
    data = fixture(); word(data, 70, 0); rejects(data);
    data = fixture(); word(data, 70, 97); rejects(data);
    data = fixture(); data[60] = 0; rejects(data);
    data = fixture(); for (int i = 60; i < 64; ++i) data[i] = '\xff'; rejects(data);
    for (bool x64 : {false, true}) {
        const auto section = 88 + (x64 ? 240 : 224);
        auto original = fixture(x64);
        for (std::size_t length = 0; length < original.size(); ++length) rejects(original.substr(0, length));
        data = original; dword(data, 88 + 56, 0); rejects(data);
        data = original; dword(data, 88 + 60, 0); rejects(data);
        data = original; dword(data, 88 + 60, 4096); rejects(data);
        data = original; dword(data, 88 + 16, 8192); rejects(data);
        data = original; dword(data, section + 20, 0); rejects(data);
        data = original; dword(data, section + 20, 0xfffffff0); rejects(data);
        data = original; dword(data, section + 16, 0xffffffff); rejects(data);
        data = original; dword(data, section + 12, 0xfffffff0); rejects(data);
        data = original; dword(data, section + 8, 0xffffffff); rejects(data);
        data = original; dword(data, section + 16, 0); dword(data, section + 20, 0);
        dword(data, 88 + 16, 0); // Zero-filled section and resource-only DLL are valid.
        assert(machine(data) == (x64 ? 0x8664u : 0x14cu));
    }
    std::istringstream bad(fixture()); bad.setstate(std::ios::badbit);
    try { cocaine::dll_machine(bad); assert(false); } catch (const std::runtime_error&) {}
    std::cout << "DLL architecture and malformed PE checks passed.\n";
}
