#define WIN32_LEAN_AND_MEAN
#define NOMINMAX
#include <windows.h>
#include <commdlg.h>
#include <tlhelp32.h>
#include <algorithm>
#include <string>
#include <vector>

namespace {
constexpr int process_id = 101;
constexpr int refresh_id = 102;
constexpr int path_id = 103;
constexpr int browse_id = 104;
constexpr int load_id = 105;
constexpr UINT poll_id = 1;

HWND title_label{}, target_label{}, dll_label{}, process_box{}, path_box{}, refresh_button{}, browse_button{}, load_button{}, status_text{};
HANDLE child_process{}, child_output{};
HFONT ui_font{};

void status(const std::wstring& message) { SetWindowTextW(status_text, message.c_str()); }

std::wstring windows_error(DWORD code) {
    return L"Windows error " + std::to_wstring(code);
}

std::wstring local_text(const std::string& bytes) {
    if (bytes.empty()) return {};
    const int count = MultiByteToWideChar(CP_ACP, 0, bytes.data(), static_cast<int>(bytes.size()), nullptr, 0);
    if (!count) return L"Canna read injector message.";
    std::wstring result(static_cast<size_t>(count), L'\0');
    MultiByteToWideChar(CP_ACP, 0, bytes.data(), static_cast<int>(bytes.size()), result.data(), count);
    while (!result.empty() && (result.back() == L'\n' || result.back() == L'\r')) result.pop_back();
    return result;
}

std::wstring own_directory() {
    wchar_t filename[MAX_PATH]{};
    const DWORD size = GetModuleFileNameW(nullptr, filename, MAX_PATH);
    if (!size || size >= MAX_PATH) return {};
    std::wstring path(filename, size);
    return path.substr(0, path.find_last_of(L"\\/"));
}

void refresh_processes() {
    SendMessageW(process_box, CB_RESETCONTENT, 0, 0);
    HANDLE snapshot = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
    if (snapshot == INVALID_HANDLE_VALUE) {
        status(L"Canna list processes: " + windows_error(GetLastError()));
        return;
    }
    PROCESSENTRY32W entry{};
    entry.dwSize = sizeof(entry);
    std::vector<std::pair<std::wstring, DWORD>> entries;
    if (Process32FirstW(snapshot, &entry)) {
        do {
            if (entry.th32ProcessID && entry.th32ProcessID != GetCurrentProcessId())
                entries.emplace_back(entry.szExeFile, entry.th32ProcessID);
        } while (Process32NextW(snapshot, &entry));
    }
    CloseHandle(snapshot);
    std::sort(entries.begin(), entries.end(), [](const auto& left, const auto& right) {
        return _wcsicmp(left.first.c_str(), right.first.c_str()) < 0;
    });
    for (const auto& entry : entries) {
        const auto label = entry.first + L"  (PID " + std::to_wstring(entry.second) + L")";
        const LRESULT index = SendMessageW(process_box, CB_ADDSTRING, 0, reinterpret_cast<LPARAM>(label.c_str()));
        if (index >= 0) SendMessageW(process_box, CB_SETITEMDATA, static_cast<WPARAM>(index), entry.second);
    }
    status(L"Chyse a process an a DLL, syne lade it.");
}

void browse_dll(HWND window) {
    wchar_t filename[MAX_PATH]{};
    GetWindowTextW(path_box, filename, MAX_PATH);
    OPENFILENAMEW dialog{};
    dialog.lStructSize = sizeof(dialog);
    dialog.hwndOwner = window;
    dialog.lpstrFilter = L"DLL files (*.dll)\0*.dll\0Aw files (*.*)\0*.*\0";
    dialog.lpstrFile = filename;
    dialog.nMaxFile = MAX_PATH;
    dialog.Flags = OFN_EXPLORER | OFN_FILEMUSTEXIST | OFN_PATHMUSTEXIST;
    if (GetOpenFileNameW(&dialog)) SetWindowTextW(path_box, filename);
}

void begin_load(HWND window) {
    const LRESULT selected = SendMessageW(process_box, CB_GETCURSEL, 0, 0);
    if (selected == CB_ERR) { status(L"Chyse a process first."); return; }
    const DWORD pid = static_cast<DWORD>(SendMessageW(process_box, CB_GETITEMDATA, selected, 0));
    if (!pid || pid == static_cast<DWORD>(CB_ERR)) { status(L"Refresh an pick a valid process."); return; }
    wchar_t dll[MAX_PATH]{};
    if (GetWindowTextLengthW(path_box) >= MAX_PATH) { status(L"DLL path is ower lang."); return; }
    if (!GetWindowTextW(path_box, dll, MAX_PATH)) { status(L"Chyse a DLL first."); return; }
    const DWORD attributes = GetFileAttributesW(dll);
    if (attributes == INVALID_FILE_ATTRIBUTES || (attributes & FILE_ATTRIBUTE_DIRECTORY)) {
        status(L"The DLL path maun name an existin file.");
        return;
    }
    const std::wstring executable = own_directory() + L"\\CocaineInjector.exe";
    if (GetFileAttributesW(executable.c_str()) == INVALID_FILE_ATTRIBUTES) {
        status(L"CocaineInjector.exe maun bide beside this launcher.");
        return;
    }
    // lpApplicationName fixes the executable; quotes keep spaces in the DLL path intact.
    std::wstring command = L"\"" + executable + L"\" " + std::to_wstring(pid) + L" \"" + dll + L"\"";
    SECURITY_ATTRIBUTES security{sizeof(security), nullptr, TRUE};
    HANDLE read_end{}, write_end{};
    if (!CreatePipe(&read_end, &write_end, &security, 0)) {
        status(L"Canna collect injector output: " + windows_error(GetLastError())); return;
    }
    if (!SetHandleInformation(read_end, HANDLE_FLAG_INHERIT, 0)) {
        const DWORD error = GetLastError();
        CloseHandle(read_end); CloseHandle(write_end);
        status(L"Canna prepare injector output: " + windows_error(error)); return;
    }
    STARTUPINFOW startup{};
    startup.cb = sizeof(startup);
    startup.dwFlags = STARTF_USESTDHANDLES | STARTF_USESHOWWINDOW;
    startup.wShowWindow = SW_HIDE;
    startup.hStdInput = GetStdHandle(STD_INPUT_HANDLE);
    startup.hStdOutput = write_end;
    startup.hStdError = write_end;
    PROCESS_INFORMATION process{};
    const BOOL started = CreateProcessW(executable.c_str(), command.data(), nullptr, nullptr, TRUE,
        CREATE_NO_WINDOW, nullptr, own_directory().c_str(), &startup, &process);
    const DWORD error = started ? ERROR_SUCCESS : GetLastError();
    CloseHandle(write_end);
    if (!started) {
        CloseHandle(read_end);
        status(L"Canna stairt injector: " + windows_error(error)); return;
    }
    CloseHandle(process.hThread);
    child_process = process.hProcess;
    child_output = read_end;
    EnableWindow(load_button, FALSE);
    status(L"Ladin DLL... this can tak up tae 15 seconds.");
    SetTimer(window, poll_id, 100, nullptr);
}

void finish_load(HWND window) {
    if (!child_process || WaitForSingleObject(child_process, 0) != WAIT_OBJECT_0) return;
    KillTimer(window, poll_id);
    DWORD code = 1;
    GetExitCodeProcess(child_process, &code);
    CloseHandle(child_process);
    child_process = nullptr;
    std::string output;
    char buffer[1024];
    DWORD size{};
    while (ReadFile(child_output, buffer, sizeof(buffer), &size, nullptr) && size && output.size() < 16384)
        output.append(buffer, size);
    CloseHandle(child_output);
    child_output = nullptr;
    EnableWindow(load_button, TRUE);
    const auto message = local_text(output);
    status(message.empty() ? (code ? L"Injector failed wi nae message." : L"DLL laded.") : message);
}

HWND control(HWND window, const wchar_t* kind, const wchar_t* label, DWORD style, int id) {
    HWND handle = CreateWindowExW(0, kind, label, WS_CHILD | WS_VISIBLE | style, 0, 0, 0, 0,
        window, reinterpret_cast<HMENU>(static_cast<INT_PTR>(id)), GetModuleHandleW(nullptr), nullptr);
    SendMessageW(handle, WM_SETFONT, reinterpret_cast<WPARAM>(ui_font), TRUE);
    return handle;
}

LRESULT CALLBACK window_proc(HWND window, UINT message, WPARAM wparam, LPARAM lparam) {
    switch (message) {
    case WM_CREATE: {
        ui_font = CreateFontW(-16, 0, 0, 0, FW_NORMAL, FALSE, FALSE, FALSE, DEFAULT_CHARSET,
            OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, CLEARTYPE_QUALITY, DEFAULT_PITCH, L"Segoe UI");
        title_label = control(window, L"STATIC", L"Cocaine  /  Windows lancher", 0, 0);
        target_label = control(window, L"STATIC", L"Process tae target", 0, 0);
        process_box = control(window, L"COMBOBOX", L"", CBS_DROPDOWNLIST | WS_VSCROLL, process_id);
        refresh_button = control(window, L"BUTTON", L"Re-list", BS_PUSHBUTTON, refresh_id);
        dll_label = control(window, L"STATIC", L"DLL file", 0, 0);
        path_box = control(window, L"EDIT", L"", WS_BORDER | ES_AUTOHSCROLL, path_id);
        browse_button = control(window, L"BUTTON", L"Chyse...", BS_PUSHBUTTON, browse_id);
        load_button = control(window, L"BUTTON", L"Lade DLL", BS_DEFPUSHBUTTON, load_id);
        status_text = control(window, L"STATIC", L"", SS_LEFT, 0);
        refresh_processes();
        return 0;
    }
    case WM_SIZE: {
        const int width = LOWORD(lparam);
        const int padding = 20;
        const int content = width - 2 * padding;
        const int button = 100;
        MoveWindow(title_label, padding, 16, content, 28, TRUE);
        MoveWindow(target_label, padding, 60, content, 20, TRUE);
        MoveWindow(process_box, padding, 85, content - button - 8, 300, TRUE);
        MoveWindow(refresh_button, width - padding - button, 85, button, 28, TRUE);
        MoveWindow(dll_label, padding, 130, content, 20, TRUE);
        MoveWindow(path_box, padding, 155, content - button - 8, 28, TRUE);
        MoveWindow(browse_button, width - padding - button, 155, button, 28, TRUE);
        MoveWindow(load_button, padding, 204, 130, 34, TRUE);
        MoveWindow(status_text, padding, 251, content, 48, TRUE);
        return 0;
    }
    case WM_GETMINMAXINFO: {
        auto* limits = reinterpret_cast<MINMAXINFO*>(lparam);
        limits->ptMinTrackSize = POINT{580, 345};
        return 0;
    }
    case WM_COMMAND:
        if (HIWORD(wparam) != BN_CLICKED) break;
        switch (LOWORD(wparam)) {
        case refresh_id: refresh_processes(); return 0;
        case browse_id: browse_dll(window); return 0;
        case load_id: if (!child_process) begin_load(window); return 0;
        }
        break;
    case WM_TIMER:
        if (wparam == poll_id) { finish_load(window); return 0; }
        break;
    case WM_CLOSE:
        if (child_process) { status(L"Wait till the injector feenishes afore closin."); return 0; }
        DestroyWindow(window);
        return 0;
    case WM_DESTROY:
        if (child_process) CloseHandle(child_process);
        if (child_output) CloseHandle(child_output);
        if (ui_font) DeleteObject(ui_font);
        PostQuitMessage(0);
        return 0;
    }
    return DefWindowProcW(window, message, wparam, lparam);
}
}

int WINAPI wWinMain(HINSTANCE instance, HINSTANCE, PWSTR, int show) {
    WNDCLASSW type{};
    type.lpfnWndProc = window_proc;
    type.hInstance = instance;
    type.lpszClassName = L"CocaineWindowsLauncher";
    type.hCursor = LoadCursorW(nullptr, IDC_ARROW);
    type.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 1);
    if (!RegisterClassW(&type)) return 1;
    HWND window = CreateWindowExW(0, type.lpszClassName, L"Cocaine", WS_OVERLAPPEDWINDOW,
        CW_USEDEFAULT, CW_USEDEFAULT, 680, 345, nullptr, nullptr, instance, nullptr);
    if (!window) return 1;
    ShowWindow(window, show);
    UpdateWindow(window);
    MSG message{};
    while (GetMessageW(&message, nullptr, 0, 0) > 0) {
        TranslateMessage(&message);
        DispatchMessageW(&message);
    }
    return static_cast<int>(message.wParam);
}
