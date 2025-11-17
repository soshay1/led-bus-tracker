#pragma once
#include "routes.h"
#include "colors.h"

uint16_t ColorFromRouteType(RouteType type)
{
    switch(type) {
        case RouteType::RouteA1:
        case RouteType::RouteA2:
            return Colors::RED;
        case RouteType::RouteB:
            return Colors::GREEN;
        case RouteType::RouteC1:
        case RouteType::RouteC2:
        case RouteType::RouteD1:
        case RouteType::RouteD2:
            return Colors::BLUE;
        case RouteType::Route75:
            return Colors::CYAN;
        default:
            return Colors::CYAN; // choosing a random one
    }
}