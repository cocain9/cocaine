#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <tlhelp32.h>
#include <filesystem>
#include <fstream>
#include <iostream>
#include <limits>
#include <string>
#include <vector>
#include "dll_image.h"

namespace {
    class Handle {
        HANDLE value_;
    public:
        explicit Handle(HANDLE value) : value_(value) {}
        ~Handle() { if (value_ && value_ != INVALID_HANDLE_VALUE) CloseHandle(value_); }
        Handle(const Handle&) = delete;
        Handle& operator=(const Handle&) = delete;
        HANDLE get() const { return value_; }
    };

    [[noreturn]] void fail(const char* operation) {
        const auto error = GetLastError();
        throw std::runtime_error(std::string(operation) + " gaed wrang (Windows error " + std::to_string(error) + ")");
    }

    DWORD parse_pid(const wchar_t* text) {
        std::uint64_t value = 0;
        if (!*text) throw std::runtime_error("PID is empty");
        for (; *text; ++text) {
            if (*text < L'0' || *text > L'9') throw std::runtime_error("PID maun hae decimal digits anerly");
            value = value * 10 + (*text - L'0');
            if (value > std::numeric_limits<DWORD>::max()) throw std::runtime_error("PID is oot o range");
        }
        if (!value || value == GetCurrentProcessId()) throw std::runtime_error("Pick a target PID that isna zero or this injector");
        return static_cast<DWORD>(value);
    }

    std::vector<MODULEENTRY32W> modules(DWORD pid) {
        HANDLE raw = INVALID_HANDLE_VALUE;
        for (int attempt = 0; attempt < 8; ++attempt) {
            raw = CreateToolhelp32Snapshot(TH32CS_SNAPMODULE | TH32CS_SNAPMODULE32, pid);
            if (raw != INVALID_HANDLE_VALUE || GetLastError() != ERROR_BAD_LENGTH) break;
        }
        Handle snapshot(raw);
        if (raw == INVALID_HANDLE_VALUE) fail("Module snapshot");
        MODULEENTRY32W entry{};
        entry.dwSize = sizeof(entry);
        if (!Module32FirstW(raw, &entry)) fail("Module32FirstW");
        std::vector<MODULEENTRY32W> result;
        do { result.push_back(entry); } while (Module32NextW(raw, &entry));
        if (GetLastError() != ERROR_NO_MORE_FILES) fail("Module32NextW");
        return result;
    }

    bool loaded(DWORD pid, const std::filesystem::path& path) {
        for (const auto& entry : modules(pid)) {
            std::error_code error;
            if (std::filesystem::equivalent(path, entry.szExePath, error) && !error) return true;
        }
        return false;
    }

    LPTHREAD_START_ROUTINE remote_loader(DWORD pid) {
        const auto kernel = GetModuleHandleW(L"kernel32.dll");
        if (!kernel) fail("GetModuleHandleW");
        const auto loader = GetProcAddress(kernel, "LoadLibraryW");
        if (!loader) fail("GetProcAddress");
        HMODULE owner = nullptr;
        // Forwarded exports can live in KernelBase: locate the actual owner.
        if (!GetModuleHandleExW(GET_MODULE_HANDLE_EX_FLAG_FROM_ADDRESS | GET_MODULE_HANDLE_EX_FLAG_UNCHANGED_REFCOUNT,
            reinterpret_cast<LPCWSTR>(loader), &owner)) fail("GetModuleHandleExW");
        wchar_t filename[MAX_PATH]{};
        const DWORD count = GetModuleFileNameW(owner, filename, MAX_PATH);
        if (!count || count >= MAX_PATH) throw std::runtime_error("Canna find the local loader module");
        const auto name = std::filesystem::path(filename).filename().wstring();
        const auto rva = reinterpret_cast<std::uintptr_t>(loader) - reinterpret_cast<std::uintptr_t>(owner);
        for (const auto& module : modules(pid)) {
            if (_wcsicmp(name.c_str(), module.szModule) == 0 && rva < module.modBaseSize)
                return reinterpret_cast<LPTHREAD_START_ROUTINE>(reinterpret_cast<std::uintptr_t>(module.modBaseAddr) + rva);
        }
        throw std::runtime_error("Target haes nae compatible loader module");
    }

    class RemoteBuffer {
        HANDLE process_;
        void* address_;
    public:
        RemoteBuffer(HANDLE process, SIZE_T size) : process_(process),
            address_(VirtualAllocEx(process, nullptr, size, MEM_COMMIT | MEM_RESERVE, PAGE_READWRITE)) {
            if (!address_) fail("VirtualAllocEx");
        }
        ~RemoteBuffer() { if (address_) VirtualFreeEx(process_, address_, 0, MEM_RELEASE); }
        RemoteBuffer(const RemoteBuffer&) = delete;
        RemoteBuffer& operator=(const RemoteBuffer&) = delete;
        void* get() const { return address_; }
        void retain() { address_ = nullptr; } // A running loader may still read its argument.
    };
}

int wmain(int argc, wchar_t** argv) {
    try {
        if (argc != 3) {
            std::wcerr << L"Uise: CocaineInjector.exe <PID> <absolute DLL path>\n";
            return 2;
        }
        const DWORD pid = parse_pid(argv[1]);
        const std::filesystem::path supplied(argv[2]);
        if (!supplied.is_absolute()) throw std::runtime_error("Gie an absolute DLL path");
        const auto path = std::filesystem::canonical(supplied);
        if (!std::filesystem::is_regular_file(path)) throw std::runtime_error("DLL path isna a regular file");
        const auto filename = path.wstring();
        // Toolhelp module paths use MAX_PATH; reject paths that cannot be verified.
        if (filename.size() >= MAX_PATH) throw std::runtime_error("DLL path maun be shorter nor MAX_PATH");
        std::ifstream image(path, std::ios::binary);
        const auto machine = cocaine::dll_machine(image);
        constexpr USHORT self_machine = sizeof(void*) == 8 ? IMAGE_FILE_MACHINE_AMD64 : IMAGE_FILE_MACHINE_I386;
        if (machine != self_machine) throw std::runtime_error("Injector an DLL architectures differ; uise the matching injector build");
        Handle process(OpenProcess(PROCESS_CREATE_THREAD | PROCESS_QUERY_INFORMATION | PROCESS_VM_OPERATION |
            PROCESS_VM_WRITE | PROCESS_VM_READ, FALSE, pid));
        if (!process.get()) fail("OpenProcess");
        USHORT process_machine = 0, native_machine = 0;
        if (!IsWow64Process2(process.get(), &process_machine, &native_machine)) fail("IsWow64Process2");
        const auto target_machine = process_machine ? process_machine : native_machine;
        if (target_machine != machine) throw std::runtime_error("Target an DLL architectures differ; the auld Cocaine DLL canna lade intae CS2");
        if (loaded(pid, path)) {
            std::wcout << L"DLL is awready laded.\n";
            return 0;
        }
        const auto loader = remote_loader(pid);
        const SIZE_T size = (filename.size() + 1) * sizeof(wchar_t);
        RemoteBuffer buffer(process.get(), size);
        SIZE_T written = 0;
        if (!WriteProcessMemory(process.get(), buffer.get(), filename.c_str(), size, &written)) fail("WriteProcessMemory");
        if (written != size) throw std::runtime_error("DLL path write isna complete");
        Handle thread(CreateRemoteThread(process.get(), nullptr, 0, loader, buffer.get(), 0, nullptr));
        if (!thread.get()) fail("CreateRemoteThread");
        const DWORD wait = WaitForSingleObject(thread.get(), 15000);
        if (wait != WAIT_OBJECT_0) {
            buffer.retain();
            throw std::runtime_error("Loader didna finish in time. Its path buffer is haudit till the process exits; check the target afore trying again");
        }
        // HMODULE is pointer-sized; GetExitCodeThread truncates it on x64.
        if (!loaded(pid, path)) throw std::runtime_error("Loader feenished but the DLL wisna fund. Check dependencies an DLL stairtup");
        std::wcout << L"DLL laded. This disna confirm its asynchronous stairtup or game hooks wrocht.\n";
        return 0;
    } catch (const std::exception& error) {
        std::cerr << "Cocaine injector: " << error.what() << '\n';
        return 1;
    }
}
