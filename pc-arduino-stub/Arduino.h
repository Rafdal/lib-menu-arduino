#pragma once

#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

typedef uint8_t byte;

template <typename T>
inline T constrain(T value, T low, T high) {
    return (value < low) ? low : (value > high) ? high : value;
}

class HardwareSerial {
public:
    void begin(unsigned long) {}

    template <typename T>
    void print(const T &value) {
        std::cout << value;
    }

    template <typename T>
    void println(const T &value) {
        std::cout << value << std::endl;
    }

    void println() {
        std::cout << std::endl;
    }
};

extern HardwareSerial Serial;

inline constexpr int HIGH = 0x1;
inline constexpr int LOW = 0x0;
inline constexpr int INPUT = 0x0;
inline constexpr int OUTPUT = 0x1;

inline void pinMode(int, int) {}
inline void digitalWrite(int, int) {}
inline int digitalRead(int) { return LOW; }

inline void delay(unsigned long ms) {
    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline unsigned long millis() {
    static const auto start = std::chrono::steady_clock::now();
    const auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::milliseconds>(now - start).count());
}

inline unsigned long micros() {
    static const auto start = std::chrono::steady_clock::now();
    const auto now = std::chrono::steady_clock::now();
    return static_cast<unsigned long>(
        std::chrono::duration_cast<std::chrono::microseconds>(now - start).count());
}

inline long map(long x, long in_min, long in_max, long out_min, long out_max) {
    return (x - in_min) * (out_max - out_min) / (in_max - in_min) + out_min;
}

inline int analogRead(int) { return 0; }
inline void analogWrite(int, int) {}

inline long random(long max) {
    return max ? std::rand() % max : 0;
}

inline long random(long min, long max) {
    return (max - min) ? (std::rand() % (max - min)) + min : min;
}

inline void randomSeed(unsigned long seed) {
    std::srand(static_cast<unsigned int>(seed));
}

#define PROGMEM
#define F(str) (str)
