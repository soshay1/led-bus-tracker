#include <cstdint>
#include <Arduino.h>
#pragma once

#ifndef BUS_API_CLIENT_H
#define BUS_API_CLIENT_H
class BusAPIClient {
    public:
    BusAPIClient();
    String getStopData();
};
#endif