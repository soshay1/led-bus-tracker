#pragma once
#include <cstdint>
#include <bitset>

enum RouteType {
    RouteA,
    RouteA1,
    RouteA2,
    RouteB,
    RouteC,
    RouteC1,
    RouteC2,
    RouteD,
    RouteD1,
    RouteD2,
    Route75
};

enum Direction {
    WEST, // technically the B goes north/south
    EAST
};

typedef struct {
    RouteType type;
    Direction dir;
} RouteInfo;

static bool operator==(const RouteInfo& lhs, const RouteInfo& rhs) {
    return lhs.type == rhs.type && lhs.dir == rhs.dir;
}

static bool operator<(const RouteInfo& lhs, const RouteInfo& rhs) {
    if (lhs.type != rhs.type) {
        return lhs.type < rhs.type;
    }
    return lhs.dir < rhs.dir;
}

struct RouteInfoHash {
    std::size_t operator()(const RouteInfo& routeInfo) const {
        return std::hash<int>()(static_cast<int>(routeInfo.type)) ^ (std::hash<int>()(static_cast<int>(routeInfo.dir)) << 1);
    }
};

typedef struct {
    RouteInfo routeInfo;
    bool isDelayed;
    bool isShortened;
    int minutes;
} BusArrivalInfo;

static bool operator<(const BusArrivalInfo& lhs, const BusArrivalInfo& rhs) {
    if (!(lhs.routeInfo == rhs.routeInfo)) {
        return lhs.routeInfo < rhs.routeInfo;
    }
    return lhs.minutes < rhs.minutes; // we shouldn't have equal routes with different minutes but just in case
}

uint16_t ColorFromRouteType(RouteType type);