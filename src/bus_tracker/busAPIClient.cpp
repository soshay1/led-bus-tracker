#include <WiFi.h>
#include <HTTPClient.h>
#include <WiFiClientSecure.h>
#include "config.h"
#include "secrets.h"
#include "WifiManager.h"
#include "busAPIClient.h"
#include "BusAPIStreamParser.h"

BusAPIClient::BusAPIClient() {
    
}

static bool requestPending = false;

std::vector<BusArrivalInfo> BusAPIClient::getStopData() {
    int requestTime = millis();
    while(wifiConnected == false) {
        if(millis() - requestTime > 10000) {
            Serial.println("WiFi not connected yet");
            return {};
        }
    }
 
    HTTPClient http;
    http.setConnectTimeout(5000);
    http.setTimeout(5000);

    String url;
    url.reserve(256);
    url = "https://metromap.cityofmadison.com/bustime/api/v3/getpredictions?key=";
    url += API_TOKEN;
    url += "&stpid=";
    url += STOP_IDS;
    url += "&rt=A,B,C,D,75";
    WiFiClientSecure client;
    //client.setInsecure(); // TEMP ONLY
    http.begin(client, url);
    
    int httpCode = -1;

    for (int attempt = 0; attempt < 3 && httpCode <= 0; attempt++) {
        httpCode = http.GET();
        if (httpCode <= 0) {
            Serial.printf("Attempt %d failed: %d\n", attempt + 1, httpCode);
            delay(500);
        }
    }

    if (httpCode > 0) {

        WiFiClient* stream = http.getStreamPtr();
        char buf[128];
        BusAPIStreamParser parser = BusAPIStreamParser();
        while(http.connected()) {
            size_t size = stream->readBytes(buf, sizeof(buf));
            parser.processChunk(buf, size);
        }
        Serial.println("Successfully got payload")
        return parser.getPayload();
    }

    else {
      Serial.print("Error on HTTP request: ");
      Serial.println(httpCode);
    }

    http.end();

  return {};
}

