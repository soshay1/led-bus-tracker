#pragma once
#include "display.h"
#include "routes.h"
#include <vector>
class RowController {
    public:
        RowController(DisplayRenderer* display);
        void update(std::vector<BusArrivalInfo> busArrivals);
        void refresh();
    private:
        DisplayRenderer* display;
        std::vector<BusArrivalInfo> currentArrivals;
        std::vector<BusArrivalInfo> displayByRow[4];
        int rowPos[4] = {0, 0, 0, 0}; // track position in each row for refreshing
};