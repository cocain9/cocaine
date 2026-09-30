#include <cassert>
#include <iostream>
#include <stdexcept>
#include <string>
#include "../tinyformat.h"

#ifndef _MSC_VER
#define __stdcall
#endif
#define XOR(value) value
using ulong_t = unsigned long;

std::string diagnostics;
void OutputDebugStringA(const char* text) { diagnostics += text; }
struct Client {
    std::string m_user;
    static ulong_t __stdcall init(void* arg);
} g_cl;
struct Game {
    enum Outcome { success, failure, signature_error } outcome = success;
    bool init() {
        if(outcome == signature_error) throw std::runtime_error("missing signature in fixture.dll");
        return outcome == success;
    }
} g_csgo;
struct Notifications {
    std::string text;
    void add(const std::string& value) { text += value; }
} g_notify;

#include "startup_under_test.cpp"

int main() {
    g_csgo.outcome = Game::failure;
    assert(Client::init(nullptr) == 0);
    assert(diagnostics.find("stairtup failed") != std::string::npos);
    assert(g_notify.text.empty());
    diagnostics.clear();
    g_csgo.outcome = Game::signature_error;
    assert(Client::init(nullptr) == 0);
    assert(diagnostics.find("fixture.dll") != std::string::npos);
    assert(g_notify.text.empty());
    diagnostics.clear();
    g_csgo.outcome = Game::success;
    assert(Client::init(nullptr) == 1);
    assert(diagnostics.empty());
    assert(g_notify.text.find("walcome") != std::string::npos);
    std::cout << "All startup failure checks passed.\n";
}
