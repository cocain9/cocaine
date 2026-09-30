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
    std::string fixture(bool x64 = false) {
        std::string image(512, '\0');
        image[0] = 'M'; image[1] = 'Z'; image[60] = 64;
        image[64] = 'P'; image[65] = 'E';
        word(image, 68, x64 ? 0x8664 : 0x14c);
        word(image, 70, 1); word(image, 84, x64 ? 240 : 224);
        word(image, 86, 0x2002); word(image, 88, x64 ? 0x20b : 0x10b);
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
    for (std::size_t length = 0; length < 352; ++length) rejects(fixture().substr(0, length));
    for (std::size_t length = 0; length < 368; ++length) rejects(fixture(true).substr(0, length));
    std::istringstream bad(fixture()); bad.setstate(std::ios::badbit);
    try { cocaine::dll_machine(bad); assert(false); } catch (const std::runtime_error&) {}
    std::cout << "DLL architecture and malformed PE checks passed.\n";
}
