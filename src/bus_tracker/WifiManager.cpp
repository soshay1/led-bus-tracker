#include <WiFi.h>
#include "secrets.h"

static bool wifiStarted = false;
volatile bool wifiConnected = false;

void wifiInit() {
  if (wifiStarted) return;
  wifiStarted = true;

  WiFi.mode(WIFI_STA);

  WiFi.onEvent([](WiFiEvent_t event) {
    switch (event) {
      case ARDUINO_EVENT_WIFI_STA_DISCONNECTED:
        wifiConnected = false;
        Serial.println("WiFi dropped, reconnecting");
        WiFi.begin(WIFI_SSID, WIFI_PASS);
        break;

      case ARDUINO_EVENT_WIFI_STA_GOT_IP:
        wifiConnected = true;
        Serial.print("WiFi up: ");
        Serial.println(WiFi.localIP());
        break;

      default:
        break;
    }
  });

  WiFi.begin(WIFI_SSID, WIFI_PASS);
}
