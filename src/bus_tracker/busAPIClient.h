#include <cstdint>
#include <Arduino.h>
#include <vector>
#include "routes.h"
#pragma once

#ifndef BUS_API_CLIENT_H
#define BUS_API_CLIENT_H
class BusAPIClient {
    public:
    BusAPIClient();
    std::vector<BusArrivalInfo> getStopData();
};
#endif