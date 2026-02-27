#pragma once

#include <cctype>
#include <chrono>
#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <iostream>
#include <thread>

typedef uint8_t byte;

class Servo
{
public:
  Servo() {}
  uint8_t attach(int pin) {}           // attach the given pin to the next free channel, sets pinMode, returns channel number or INVALID_SERVO if failure
  uint8_t attach(int pin, int min, int max) {} // as above but also sets min and max values for writes.
  void detach() {}
  void write(int value) {}             // if value is < 200 it's treated as an angle, otherwise as pulse width in microseconds
  void writeMicroseconds(int value) {} // Write pulse width in microseconds
//   int read();                        // returns current pulse width as an angle between 0 and 180 degrees
//   int readMicroseconds();            // returns current pulse width in microseconds for this servo (was read_us() in first release)
//   bool attached();                   // return true if this servo is attached, otherwise false
};