#include "busAPIService.h"
#include "busAPIClient.h"
#include <Arduino.h>
#include <set>

BusAPIService::BusAPIService()
{
    this->client = BusAPIClient();
}

std::vector<RouteInfo> BusAPIService::parseStopData()
{
    const String data = client.getStopData();
    if(data == "") {
        return std::vector<RouteInfo>();
    }
    std::vector<RouteInfo> routes;
    std::set<RouteType> fullRouteSet = {
        RouteType::RouteA,
        RouteType::RouteA1,
        RouteType::RouteA2,
        RouteType::RouteB,
        RouteType::RouteC,
        RouteType::RouteC1,
        RouteType::RouteC2,
        RouteType::RouteD1,
        RouteType::RouteD2,
        RouteType::Route75
    };
    String errorElement="<error>";
    String messageElement="<msg>";
    String timestampElement="<tmstmp>";
    String routeElement="<rt>";
    String routeDirectionElement="<rtdir>";
    String destinationElement="<des>";
    String delayElement="<dly>";
    String minutesElement="<prdctdn>";
    int pos = -1;
    std::vector<RouteType> excludedRouteTypes;

    while(pos < data.length()) // first find errors
    {
        pos = data.indexOf(errorElement, pos + 1);
        if(pos == -1) break;
        int routeStart = data.indexOf(routeElement, pos) + routeElement.length();
        String route = data.substring(routeStart, data.indexOf("</rt>", routeStart)-routeStart);
        int msgStart = data.indexOf(messageElement, pos) + messageElement.length();

        if(data.substring(msgStart,20).equals("No service scheduled")) {
            if (route.equals("A")) {
                excludedRouteTypes.push_back(RouteType::RouteA);
                excludedRouteTypes.push_back(RouteType::RouteA1);
                excludedRouteTypes.push_back(RouteType::RouteA2);
            }
            else if (route.equals("B")) {
                excludedRouteTypes.push_back(RouteType::RouteB);
            }
            else if (route.equals("C")) {
                excludedRouteTypes.push_back(RouteType::RouteC);
                excludedRouteTypes.push_back(RouteType::RouteC1);
                excludedRouteTypes.push_back(RouteType::RouteC2);
            }
            else if (route.equals("D")) {
                excludedRouteTypes.push_back(RouteType::RouteD1);
                excludedRouteTypes.push_back(RouteType::RouteD2);
            }
            else if (route.equals("75")) {
                excludedRouteTypes.push_back(RouteType::Route75);
            }
        }
    }

    std::set<RouteType> routeSet(excludedRouteTypes.begin(), excludedRouteTypes.end());

    pos = data.indexOf(timestampElement);
    while(pos != -1)
    {
        int routeStart = data.indexOf(routeElement, pos) + routeElement.length();
        String route = data.substring(routeStart, data.indexOf("</rt>", routeStart)-routeStart);
        int routeDirectionStart = data.indexOf(routeDirectionElement, pos) + routeDirectionElement.length();
        String routeDirection = data.substring(routeDirectionStart, data.indexOf("</rtdir>", routeDirectionStart)-routeDirectionStart);
        int destinationStart = data.indexOf(destinationElement, pos) + destinationElement.length();
        String destination = data.substring(destinationStart, data.indexOf("</des>", destinationStart)-destinationStart);
        int delayStart = data.indexOf(delayElement, pos) + delayElement.length();
        String delay = data.substring(delayStart, data.indexOf("</dly>", delayStart)-delayStart);
        int minutesStart = data.indexOf(minutesElement, pos) + minutesElement.length();
        String minutes = data.substring(minutesStart, data.indexOf("</prdctdn>", minutesStart)-minutesStart);

        RouteType routeType;
        if (!tryParseRouteType(route, destination, routeType)) {
            pos = data.indexOf(timestampElement, pos + 1);
            continue; // unrecognized route
        }

        RouteInfo info;
        info.type = routeType;
        info.dir = (routeDirection.equals("EASTBOUND") || routeDirection.equals("NORTHBOUND")) ? Direction::EAST : Direction::WEST;
        info.isDelayed = delay.equalsIgnoreCase("true");
        info.minutes = minutes.toInt();
        routes.push_back(info);

        routeSet.insert(info.type);
        if (routeSet == fullRouteSet) break;
    }
    // Implementation for parsing the data into RouteInfo objects
    return routes;
}
bool BusAPIService::tryParseRouteType(const String &route, const String &destination, RouteType &out)
        {
            if (route.equals("A")) {
                if (destination.equals("JUNCTION")) { out = RouteType::RouteA; return true; }
                if (destination.length() > 0) {
                    if (destination[0] == '1') { out = RouteType::RouteA1; return true; }
                    if (destination[0] == '2') { out = RouteType::RouteA2; return true; }
                }
            } else if (route.equals("B")) {
                out = RouteType::RouteB; return true;
            } else if (route.equals("C")) {
                if (destination.equals("U.W. HOSPITAL")) { out = RouteType::RouteC; return true; }
                if (destination.length() > 0) {
                    if (destination[0] == '1') { out = RouteType::RouteC1; return true; }
                    if (destination[0] == '2') { out = RouteType::RouteC2; return true; }
                }
            } else if (route.startsWith("D")) {
                if (route.length() > 1) {
                    if (route[1] == '1') { out = RouteType::RouteD1; return true; }
                    if (route[1] == '2') { out = RouteType::RouteD2; return true; }
                }
                if (destination.length() > 1) {
                    if (destination[1] == '1') { out = RouteType::RouteD1; return true; }
                    if (destination[1] == '2') { out = RouteType::RouteD2; return true; }
                }
            } else if (route.equals("75")) {
                out = RouteType::Route75; return true;
            }
            return false;
        }