#pragma once

#include <array>
#include <cstdint>
#include <istream>
#include <stdexcept>

namespace cocaine {
    // Validate the architecture before allocating anything in the target process.
    // This is a structural check, not an assertion that a DLL is trustworthy/loadable.
    inline std::uint16_t dll_machine(std::istream& input) {
        input.seekg(0, std::ios::end);
        const auto end = input.tellg();
        if (end < 64) throw std::runtime_error("DLL haes a cut-short DOS heider");
        const auto length = static_cast<std::uint64_t>(end);
        auto read = [&](std::uint64_t offset, unsigned char* data, std::size_t count) {
            if (offset > length || count > length - offset)
                throw std::runtime_error("DLL heider gangs by the end o the file");
            input.seekg(static_cast<std::streamoff>(offset));
            if (!input.read(reinterpret_cast<char*>(data), static_cast<std::streamsize>(count)))
                throw std::runtime_error("Canna read DLL heider");
        };
        auto u16 = [](const unsigned char* p) -> std::uint16_t { return p[0] | (p[1] << 8); };
        auto u32 = [](const unsigned char* p) -> std::uint32_t {
            return std::uint32_t(p[0]) | (std::uint32_t(p[1]) << 8) |
                (std::uint32_t(p[2]) << 16) | (std::uint32_t(p[3]) << 24);
        };
        std::array<unsigned char, 64> dos{};
        read(0, dos.data(), dos.size());
        if (dos[0] != 'M' || dos[1] != 'Z') throw std::runtime_error("DLL haes nae MZ heider");
        const std::uint64_t pe = u32(dos.data() + 60);
        if (pe < dos.size()) throw std::runtime_error("PE heider position isna valid");
        std::array<unsigned char, 26> coff{};
        read(pe, coff.data(), coff.size());
        if (u32(coff.data()) != 0x4550) throw std::runtime_error("DLL haes nae PE signature");
        const auto machine = u16(coff.data() + 4);
        const auto sections = u16(coff.data() + 6);
        const auto optional_size = u16(coff.data() + 20);
        const auto flags = u16(coff.data() + 22);
        const auto magic = u16(coff.data() + 24);
        if (!(flags & 0x2000) || !(flags & 0x0002)) throw std::runtime_error("File isna an executable DLL image");
        if (!((machine == 0x014c && magic == 0x010b && optional_size >= 224) ||
              (machine == 0x8664 && magic == 0x020b && optional_size >= 240)))
            throw std::runtime_error("Anly matching x86/x64 PE DLL heiders are supportit");
        if (sections == 0 || sections > 96 || pe + 24 + optional_size + std::uint64_t(sections) * 40 > length)
            throw std::runtime_error("DLL section table isna valid or is cut short");
        return machine;
    }
}
