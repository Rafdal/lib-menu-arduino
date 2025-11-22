#ifndef MENU_CONSOLE_H
#define MENU_CONSOLE_H


#include <algorithm>
#include <cctype>
#include <chrono>
#include <cstdlib>
#include <functional>
#include <iostream>
#include <string>
#include <cstdio>
#include <vector>
#include <thread>

#if defined(_WIN32)
#include <conio.h>
#else
#include <sys/select.h>
#include <termios.h>
#include <unistd.h>
#endif

#include "Menu.h"

#ifndef DEBUG_PRINT_BUFFER_SIZE
#define DEBUG_PRINT_BUFFER_SIZE 5
#endif

struct DebugMessage {
    unsigned long timestamp;
    unsigned long id;
    char* text;
};

class DebugBuffer {
    static constexpr size_t kCapacity = DEBUG_PRINT_BUFFER_SIZE;
    DebugMessage buffer[kCapacity];
    size_t head = 0;
    size_t count = 0;

    unsigned long nextId = 0;

public:
    void log(const char* msg) {
        buffer[head] = {millis(), nextId++, const_cast<char*>(msg)};
        head = (head + 1) % kCapacity;
        if (count < kCapacity) ++count;
    }

    void printAll(bool showIds = true, bool showTime = false) const {
        size_t start = (head + kCapacity - count) % kCapacity;
        for (size_t i = 0; i < count; ++i) {
            const auto& entry = buffer[(start + i) % kCapacity];
            if (showTime && showIds) {
                std::printf("[%10lu ms] (ID: %lu) %s\n", entry.timestamp, entry.id, entry.text);
            } else if (showTime) {
                std::printf("[%10lu ms] %s\n", entry.timestamp, entry.text);
            } else if (showIds) {
                std::printf("% 3lu %s\n", entry.id, entry.text);
            } else {
                std::printf("%s\n", entry.text);
            }
        }
    }
};

DebugBuffer debug;

#if !defined(_WIN32)
class TerminalRawMode {
public:
    TerminalRawMode() {
        tcgetattr(STDIN_FILENO, &original_);
        termios raw = original_;
        raw.c_lflag &= static_cast<unsigned long>(~(ICANON | ECHO));
        raw.c_cc[VMIN] = 0;
        raw.c_cc[VTIME] = 0;
        tcsetattr(STDIN_FILENO, TCSANOW, &raw);
    }

    ~TerminalRawMode() {
        tcsetattr(STDIN_FILENO, TCSANOW, &original_);
    }

private:
    termios original_{};
};
#endif

bool try_read_char(char &ch) {
#if defined(_WIN32)
    if (_kbhit()) {
        ch = static_cast<char>(_getch());
        return true;
    }
    return false;
#else
    static TerminalRawMode raw_guard;
    fd_set set;
    FD_ZERO(&set);
    FD_SET(STDIN_FILENO, &set);
    timeval timeout{};
    const int ready = select(STDIN_FILENO + 1, &set, nullptr, nullptr, &timeout);
    if (ready > 0 && FD_ISSET(STDIN_FILENO, &set)) {
        unsigned char byte = 0;
        const ssize_t count = read(STDIN_FILENO, &byte, 1);
        if (count == 1) {
            ch = static_cast<char>(byte);
            return true;
        }
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(5));
    return false;
#endif
}

void handle_arrow_key()
{
#if defined(_WIN32)
    const int extended = _getch();
    switch (extended) {
    case 72: // Up arrow
        menu_go_up();
        debug.log("[Debug] Up arrow key detected");
        break;
    case 80: // Down arrow
        menu_go_down();
        debug.log("[Debug] Down arrow key detected");
        break;
    case 75: // Left arrow
        menu_go_left();
        debug.log("[Debug] Left arrow key detected");
        break;
    case 77: // Right arrow
        menu_go_right();
        debug.log("[Debug] Right arrow key detected");
        break;
    default:
        break;
    }
#else
    // Non-Windows terminals already emit escape sequences that
    // will be handled by the default case of keyboard_listener.
#endif
}

void keyboard_listener()
{
    char ch = 0;
    if (!try_read_char(ch)) {
        std::this_thread::sleep_for(std::chrono::milliseconds(5));
        return;
    }

#if defined(_WIN32)
    if (ch == 0 || ch == static_cast<char>(224)) {
        handle_arrow_key();
        return;
    }
#endif

    switch (static_cast<unsigned char>(ch)) {
    case 'w':
    case 'W':
        menu_go_up();
        debug.log("[Debug] W key detected");
        break;
    case 's':
    case 'S':
        menu_go_down();
        debug.log("[Debug] S key detected");
        break;
    case 'a':
    case 'A':
        menu_go_left();
        debug.log("[Debug] A key detected");
        break;
    case 'd':
    case 'D':
        menu_go_right();
        debug.log("[Debug] D key detected");
        break;
    case '\r':
    case '\n':
    case ' ':
        menu_go_select();
        debug.log("[Debug] Select key detected");
        break;
    case '\b':
    case 27: // ESC key
    case 'q':
    case 'Q':
        menu_go_back();
        debug.log("[Debug] Back key detected");
        break;
    default:
        break;
    }
}

void clear_console()
{
#if defined(_WIN32)
    std::system("cls");
#else
    std::cout << "\x1B[2J\x1B[H";
#endif
}

#endif // MENU_CONSOLE_H