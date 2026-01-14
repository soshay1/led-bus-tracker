#pragma once
#include <cstdint>
#define HEIGHT  32 // Matrix height (pixels) - SET TO 64 FOR 64x64 MATRIX!
#define WIDTH   64 // Matrix width (pixels)
namespace Config {

    // pins
    #if defined(_VARIANT_MATRIXPORTAL_M4_) // MatrixPortal M4
    const uint8_t rgbPins[6]  = {7, 8, 9, 10, 11, 12};
    const uint8_t addrPins[5] = {17, 18, 19, 20, 21};
    constexpr uint8_t clockPin   = 14;
    constexpr uint8_t latchPin   = 15;
    constexpr uint8_t oePin      = 16;
    #else // MatrixPortal ESP32-S3
    const uint8_t rgbPins[6]  = {42, 41, 40, 38, 39, 37};
    const uint8_t addrPins[5] = {45, 36, 48, 35, 21};
    constexpr uint16_t clockPin   = 2;
    constexpr uint16_t latchPin   = 47;
    constexpr uint16_t oePin      = 14;
    #endif
    #if HEIGHT == 16
    constexpr uint8_t numAddrPins = 3;
    #elif HEIGHT == 32
    constexpr uint8_t numAddrPins = 4;
    #elif HEIGHT == 64
    constexpr uint8_t numAddrPins = 4;
    #endif
}

