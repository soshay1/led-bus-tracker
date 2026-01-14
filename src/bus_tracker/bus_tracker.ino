
#pragma once
#include "routes.h"
#include "busAPIService.h"
#include "display.h"
#include "config.h"
#include "WifiManager.h"

static const int rowY[] = { 0, 16, 32, 48 }; // row 0–3 y-coordinates

Adafruit_Protomatter matrix(
  WIDTH, 4, 1, (uint8_t*)Config::rgbPins, Config::numAddrPins, (uint8_t*)Config::addrPins,
  Config::clockPin, Config::latchPin, Config::oePin, true,-2); // This function does NOT need to modify the input pins so why it demands a non-const array is beyond me.

AdafruitDisplayAdapter adapter(matrix);
DisplayRenderer display(adapter, Colors::WHITE, Colors::BLACK);
int minutes = 0;

// SETUP - RUNS ONCE AT PROGRAM START --------------------------------------

void err(int x) {
  uint8_t i;
  pinMode(LED_BUILTIN, OUTPUT);       // Using onboard LED
  for(i=1;;i++) {                     // Loop forever...
    digitalWrite(LED_BUILTIN, i & 1); // LED on/off blink to alert user
    delay(x);
  }
}

void setup(void) {
  Serial.begin(115200);
  //while (!Serial) delay(10);

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);

  wifiInit();

}


void loop() {
  BusAPIService busService;
  std::vector<RouteInfo> routes = busService.parseStopData();
  uint16_t x, y;
  RouteInfo R1 = {RouteType::RouteA1, Direction::EAST, minutes};
  RouteInfo R2 = {RouteType::RouteB, Direction::WEST, 60-minutes};
  RouteInfo R3 = {RouteType::Route75, Direction::EAST, minutes};
  RouteInfo R4 = {RouteType::RouteD2, Direction::EAST, minutes};
  if(minutes>60)
  {
      minutes = 0;
  }
  else
  {
      minutes+=1;
  }
  matrix.fillScreen(0x0);
  display.drawRoute(R1,0);
  display.drawRoute(R2,1);
  display.drawRoute(R3,2);
  display.drawRoute(R4,3);
  matrix.show();
  delay(60000);

}