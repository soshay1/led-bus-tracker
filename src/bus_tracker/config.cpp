#include "config.h"

namespace Config {

    #if defined(_VARIANT_MATRIXPORTAL_M4_)
    const uint8_t rgbPins[6]  = {7, 8, 9, 10, 11, 12};
    const uint8_t addrPins[5] = {17, 18, 19, 20, 21};

    const uint8_t clockPin = 14;
    const uint8_t latchPin = 15;
    const uint8_t oePin    = 16;
    #else
    const uint8_t rgbPins[6]  = {42, 41, 40, 38, 39, 37};
    const uint8_t addrPins[5] = {45, 36, 48, 35, 21};

    const uint16_t clockPin = 2;
    const uint16_t latchPin = 47;
    const uint16_t oePin    = 14;
    #endif

    #if HEIGHT == 16
    const uint8_t numAddrPins = 3;
    #elif HEIGHT == 32
    const uint8_t numAddrPins = 4;
    #elif HEIGHT == 64
    const uint8_t numAddrPins = 4;
    #endif

    const char root_ca[] =
        "-----BEGIN CERTIFICATE-----\n"
        "MIIDjjCCAnagAwIBAgIQAzrx5qcRqaC7KGSxHQn65TANBgkqhkiG9w0BAQsFADBh\n"
        "MQswCQYDVQQGEwJVUzEVMBMGA1UEChMMRGlnaUNlcnQgSW5jMRkwFwYDVQQLExB3\n"
        "d3cuZGlnaWNlcnQuY29tMSAwHgYDVQQDExdEaWdpQ2VydCBHbG9iYWwgUm9vdCBH\n"
        "MjAeFw0xMzA4MDExMjAwMDBaFw0zODAxMTUxMjAwMDBaMGExCzAJBgNVBAYTAlVT\n"
        "MRUwEwYDVQQKEwxEaWdpQ2VydCBJbmMxGTAXBgNVBAsTEHd3dy5kaWdpY2VydC5j\n"
        "b20xIDAeBgNVBAMTF0RpZ2lDZXJ0IEdsb2JhbCBSb290IEcyMIIBIjANBgkqhkiG\n"
        "9w0BAQEFAAOCAQ8AMIIBCgKCAQEAuzfNNNx7a8myaJCtSnX/RrohCgiN9RlUyfuI\n"
        "...\n"
        "-----END CERTIFICATE-----\n";
}
