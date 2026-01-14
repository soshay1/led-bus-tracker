#include <cstdint>
#include <vector>
#include <Arduino.h>
#include "routes.h"
#include "busAPIClient.h"
#pragma once

#ifndef BUS_API_SERVICE_H
#define BUS_API_SERVICE_H
class BusAPIService {
    public:
        BusAPIService();
        std::vector<RouteInfo> parseStopData(); 
    private:
        BusAPIClient client;
        static bool tryParseRouteType(const String &route, const String &destination, RouteType &out);

};
#endif