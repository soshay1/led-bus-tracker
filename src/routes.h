#pragma once
#include <cstdint>

enum RouteType {
    RouteA1,
    RouteA2,
    RouteB,
    RouteC1,
    RouteC2,
    RouteD1,
    RouteD2,
    Route75
};

enum Direction {
    EAST,
    WEST // technically the B goes north/south but I'm lucky enough to live between two lakes so it's the same thing
};

typedef struct {
    RouteType type;
    Direction dir;
    int minutesUntilNextBus;
    
} RouteInfo;

uint16_t ColorFromRouteType(RouteType type);