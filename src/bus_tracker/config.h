#pragma once
#include <cstdint>

#define HEIGHT  32
#define WIDTH   64

namespace Config {

    // Pins
    extern const uint8_t rgbPins[6];
    extern const uint8_t addrPins[5];

    #if defined(_VARIANT_MATRIXPORTAL_M4_)
    extern const uint8_t  clockPin;
    extern const uint8_t  latchPin;
    extern const uint8_t  oePin;
    #else
    extern const uint16_t clockPin;
    extern const uint16_t latchPin;
    extern const uint16_t oePin;
    #endif

    extern const uint8_t numAddrPins;

    // TLS root certificate
    extern const char root_ca[];
}
