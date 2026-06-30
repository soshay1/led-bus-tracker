#pragma once
#include "routes.h"
#include <unordered_set>
#include <vector>

enum State {
    START,
    IN_ERROR,
    PREDICTION_OR_ERROR_NEXT,
    PREDICTION_NEXT,
    TIMESTAMP_NEXT,
    IN_TIMESTAMP,
    ROUTE_NEXT,
    IN_ROUTE,
    ROUTE_DIRECTION_NEXT,
    IN_ROUTE_DIRECTION,
    DESTINATION_NEXT,
    IN_DESTINATION,
    DELAY_NEXT,
    IN_DELAY,
    MINUTES_NEXT,
    IN_MINUTES,
    DONE
};

class BusAPIStreamParser {
    public:
        BusAPIStreamParser();
        void processChunk(const char* chunk, size_t length);
        std::vector<BusArrivalInfo> getPayload();
    private:
        static bool tryParseRouteType(const char* route, const char* destination, RouteType &out, bool &isShortened);
        State state;
        char window[19];
        size_t windowSize = 0;
        size_t bufferPos = 0;

        char routeBuffer[2] = {0};
        size_t routeLength = 0;
        char routeDirectionBuffer[12] = {0};
        size_t routeDirectionLength = 0;
        char destinationBuffer[32] = {0};
        size_t destinationLength = 0;
        char delayBuffer[5] = {0};
        size_t delayLength = 0;
        char minutesBuffer[4] = {0};
        size_t minutesLength = 0;

        std::vector<BusArrivalInfo> parsedRoutes;
        size_t parsedRoutesCount = 0;
        std::unordered_set<RouteInfo, RouteInfoHash> parsedRouteTypes;
        RouteType currentRouteType;
        RouteInfo currentRouteInfo;
        BusArrivalInfo currentBusArrivalInfo;

};