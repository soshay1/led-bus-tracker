#include "RowController.h"
#include <algorithm>

RowController::RowController(DisplayRenderer* display) : display(display) { 
    this->currentArrivals = std::vector<BusArrivalInfo>(); 
    for (int i = 0; i < 4; i++) {
        this->displayByRow[i] = std::vector<BusArrivalInfo>();
    }
}

void RowController::update(std::vector<BusArrivalInfo> busArrivals) {
    this->currentArrivals = busArrivals;
    // four or fewer routes, put them all on display
    if(busArrivals.size() <= 4) {
        for (int i = 0; i < busArrivals.size(); i++) {
            displayByRow[i] = std::vector<BusArrivalInfo>(1, busArrivals[i]);
        }
        return;
    }

    for (int i = 0; i < busArrivals.size(); i++) {
        switch(busArrivals[i].routeInfo.type) {
            case RouteType::RouteA:
            case RouteType::RouteA1:
            case RouteType::RouteA2:
                displayByRow[0].push_back(busArrivals[i]);
                break;
            case RouteType::RouteB:
                displayByRow[1].push_back(busArrivals[i]);
                break;
            case RouteType::RouteC:
            case RouteType::RouteC1:
            case RouteType::RouteC2:
                displayByRow[2].push_back(busArrivals[i]);
                break;
            case RouteType::RouteD:
            case RouteType::RouteD1:
            case RouteType::RouteD2:
                displayByRow[3].push_back(busArrivals[i]);
                break;
            case RouteType::Route75:
                // drop it on row 1
                displayByRow[1].push_back(busArrivals[i]);
                break;
        }
    }
    for (int i = 0; i < 4; i++) {
        std::sort(displayByRow[i].begin(), displayByRow[i].end());
    }
    std::fill(std::begin(rowPos), std::end(rowPos), 0); // reset row positions
}

void RowController::refresh() { 
    for (int row = 0; row < 4; row++) {
        if (displayByRow[row].size() == 0) {
            continue; // nothing to display
        }
        display->drawRoute(displayByRow[row][rowPos[row]], row);
        rowPos[row] = (rowPos[row] + 1) % displayByRow[row].size();
    }
}