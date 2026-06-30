#include "BusAPIStreamParser.h"
#include <cstring>
#include <algorithm>
#include <vector>

static const char* errorElement="<error>";
static const char* errorElementEnd="</error>";
static const char* predictionElement="<prd>";
static const char* predictionElementEnd="</prd>";
static const char* routeElement="<rt>";
static const char* routeDirectionElement="<rtdir>";
static const char* destinationElement="<des>";
static const char* delayElement="<dly>";
static const char* minutesElement="<prdctdn>";
static const char* fullResponseEnd="</bustime-response>";
static const char* noServiceError="No service scheduled";

static const char* northbound="NORTHBOUND";
static const char* eastbound="EASTBOUND";
static const char* southbound="SOUTHBOUND";
static const char* westbound="WESTBOUND";


BusAPIStreamParser::BusAPIStreamParser()
{
    this->state = State::START;
    std::fill(std::begin(this->window), std::end(this->window), 0);
}

std::vector<BusArrivalInfo> BusAPIStreamParser::getPayload() {
    return this->parsedRoutes;
}

void BusAPIStreamParser::processChunk(const char* chunk, size_t length)
{
    for (size_t i = 0; i < length; i++) {
        char c = chunk[i];

        if (this->windowSize < sizeof(this->window)) {
            this->window[this->windowSize] = c;
            this->windowSize++;
        }
        else
        {
            memmove(this->window, this->window + 1, sizeof(window) - 1); // move everything left by one
            window[sizeof(window) - 1] = c;
        }
        switch(state)  {
            case START:
                if (windowSize >= strlen(errorElement) && memcmp(window + windowSize - strlen(errorElement), errorElement, strlen(errorElement)) == 0) {
                    state = IN_ERROR;
                }
                break;
            case IN_ERROR:
                if (windowSize >= strlen(errorElementEnd) && memcmp(window + windowSize - strlen(errorElementEnd), errorElementEnd, strlen(errorElementEnd)) == 0) {
                    state = PREDICTION_OR_ERROR_NEXT;
                }
                break;
            case PREDICTION_OR_ERROR_NEXT: // Using this state while we don't know if errors are done yet
                if (windowSize >= strlen(predictionElement) && memcmp(window + windowSize - strlen(predictionElement), predictionElement, strlen(predictionElement)) == 0) {
                    state = ROUTE_NEXT;
                }
                else if (windowSize >= strlen(errorElement) && memcmp(window + windowSize - strlen(errorElement), errorElement, strlen(errorElement)) == 0) {
                    state = IN_ERROR;
                }
                break;
            case PREDICTION_NEXT:
                if(routeLength > 0) {
                    // save prediction
                    bool parsedRoute = false;
                    bool isShortened = false;
                    int minutes = 0;
                    if(destinationLength > 0 && routeDirectionLength > 0 && delayLength > 0 && minutesLength > 0) { // only attempt if buffers aren't empty
                        parsedRoute = tryParseRouteType(routeBuffer, destinationBuffer, currentRouteType, isShortened);
                    }

                    if(!parsedRoute) { // failed to parse = quit
                        break;
                    }

                    currentRouteInfo.type = currentRouteType;
                    currentRouteInfo.dir = (memcmp(routeDirectionBuffer, northbound, strlen(northbound)) == 0 || memcmp(routeDirectionBuffer, eastbound, strlen(eastbound)) == 0) ? Direction::EAST : Direction::WEST;
                        
                    if(parsedRouteTypes.find(currentRouteInfo) == parsedRouteTypes.end()) { // only add if route type is valid and we haven't already added a prediction for this route type
                        currentBusArrivalInfo.routeInfo = currentRouteInfo;
                        currentBusArrivalInfo.isDelayed = memcmp(delayBuffer, "true", 4) == 0;
                        for(size_t i = 0; i < minutesLength; i++) {
                            if (minutesBuffer[i] < '0' || minutesBuffer[i] > '9') {
                                parsedRoute = false;
                                break;
                            }
                            minutes = minutes * 10 + (minutesBuffer[i] - '0');
                        }
                        currentBusArrivalInfo.minutes = minutes;
                        currentBusArrivalInfo.isShortened = isShortened;
                        parsedRoutes.push_back(currentBusArrivalInfo); // add to array
                        parsedRouteTypes.insert(currentRouteInfo); // add to set to track unique routes
                    }
                    // cleanup
                    std::fill(std::begin(routeBuffer), std::end(routeBuffer), 0);
                    std::fill(std::begin(routeDirectionBuffer), std::end(routeDirectionBuffer), 0);
                    std::fill(std::begin(destinationBuffer), std::end(destinationBuffer), 0);
                    std::fill(std::begin(delayBuffer), std::end(delayBuffer), 0);
                    std::fill(std::begin(minutesBuffer), std::end(minutesBuffer), 0);
                    routeLength = 0;
                    routeDirectionLength = 0;
                    destinationLength = 0;
                    delayLength = 0;
                    minutesLength = 0;
                }
                if (windowSize >= strlen(predictionElement) && memcmp(window + windowSize - strlen(predictionElement), predictionElement, strlen(predictionElement)) == 0) {
                    state = ROUTE_NEXT;
                }
                else if (windowSize >= strlen(fullResponseEnd) && memcmp(window + windowSize - strlen(fullResponseEnd), fullResponseEnd, strlen(fullResponseEnd)) == 0) {
                    state = DONE;
                }
                break;
            case ROUTE_NEXT:
                if (windowSize >= strlen(routeElement) && memcmp(window + windowSize - strlen(routeElement), routeElement, strlen(routeElement)) == 0) {
                    bufferPos = 0;
                    state = IN_ROUTE;
                }
                break;
            case IN_ROUTE:
                if (windowSize > 0 && window[windowSize - 1] == '<') {
                    // end of route element
                    routeLength = bufferPos;
                    state = ROUTE_DIRECTION_NEXT;
                }
                else {
                    // accumulate route characters
                    if (bufferPos <= sizeof(routeBuffer) - 1) {
                        routeBuffer[bufferPos++] = c;
                    }
                }
                break;
            case ROUTE_DIRECTION_NEXT:
                bufferPos = 0;
                if (windowSize >= strlen(routeDirectionElement) && memcmp(window + windowSize - strlen(routeDirectionElement), routeDirectionElement, strlen(routeDirectionElement)) == 0) {
                    state = IN_ROUTE_DIRECTION;
                }
                break;
            case IN_ROUTE_DIRECTION:
                if (windowSize > 0 && window[windowSize - 1] == '<') {
                    routeDirectionLength = bufferPos;
                    state = DESTINATION_NEXT;
                }
                else {
                    if (bufferPos <= sizeof(routeDirectionBuffer) - 1) {
                        routeDirectionBuffer[bufferPos++] = c;
                    }
                }
                break;
            case DESTINATION_NEXT:
                bufferPos = 0;
                if (windowSize >= strlen(destinationElement) && memcmp(window + windowSize - strlen(destinationElement), destinationElement, strlen(destinationElement)) == 0) {
                    state = IN_DESTINATION;
                }
                break;
            case IN_DESTINATION:
                if (windowSize > 0 && window[windowSize - 1] == '<') {
                    destinationLength = bufferPos;
                    state = DELAY_NEXT;
                }
                else {
                    if (bufferPos <= sizeof(destinationBuffer) - 1) {
                        destinationBuffer[bufferPos++] = c;
                    }
                }
                break;
            case DELAY_NEXT:
                bufferPos = 0;
                if (windowSize >= strlen(delayElement) && memcmp(window + windowSize - strlen(delayElement), delayElement, strlen(delayElement)) == 0) {
                    state = IN_DELAY;
                }
                break;
            case IN_DELAY:
                if (windowSize > 0 && window[windowSize - 1] == '<') {
                    delayLength = bufferPos;
                    state = MINUTES_NEXT;
                }
                else {
                    if (bufferPos <= sizeof(delayBuffer) - 1) {
                        delayBuffer[bufferPos++] = c;
                    }
                }
                break;
            case MINUTES_NEXT:
                bufferPos = 0;
                if (windowSize >= strlen(minutesElement) && memcmp(window + windowSize - strlen(minutesElement), minutesElement, strlen(minutesElement)) == 0) {
                    state = IN_MINUTES;
                }
                break;
            case IN_MINUTES:
                if (windowSize > 0 && window[windowSize - 1] == '<') {
                    minutesLength = bufferPos;
                    state = PREDICTION_NEXT;
                }
                else {
                    if (bufferPos <= sizeof(minutesBuffer) - 1) {
                        minutesBuffer[bufferPos++] = c;
                    }
                }
                break;
            case DONE:
                break;
        }
    }
}
bool BusAPIStreamParser::tryParseRouteType(const char* route, const char* destination, RouteType &out, bool &isShortened)
{
        if (route[0] == 'A') {
            if (memcmp(destination, "JUNCTION", 8) == 0) { out = RouteType::RouteA; return true; }
            if (destination[0] == '1') { out = RouteType::RouteA1; return true; }
            if (destination[0] == '2') { out = RouteType::RouteA2; return true; }
            else { out = RouteType::RouteA; isShortened = true; return true; }
        } else if (route[0] == 'B') { out = RouteType::RouteB; return true;
        } else if (route[0] == 'C') {
            if (memcmp(destination, "U.W. HOSPITAL", 13) == 0) { out = RouteType::RouteC; return true; }
            else if (destination[0] == '1') { out = RouteType::RouteC1; return true; }
            else if (destination[0] == '2') { out = RouteType::RouteC2; return true; }
            else { out = RouteType::RouteC; isShortened = true; return true; }
        } else if (route[0] == 'D') {
            if (destination[0] == '1') { out = RouteType::RouteD1; return true; }
            if (destination[0] == '2') { out = RouteType::RouteD2; return true; }
            else { out = RouteType::RouteD; isShortened = true; return true; }
        } else if (memcmp(route, "75", 2) == 0) {
            out = RouteType::Route75; return true;
        }
        return false;
}