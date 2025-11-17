
#pragma once
#include "routes.h"
#include "display.h"
#include "config.h"

static const int rowY[] = { 0, 16, 32, 48 }; // row 0–3 y-coordinates

Adafruit_Protomatter matrix(
  WIDTH, 4, 1, (uint8_t*)Config::rgbPins, Config::numAddrPins, (uint8_t*)Config::addrPins,
  Config::clockPin, Config::latchPin, Config::oePin, true,-2); // This function does NOT need to modify the input pins so why it demands a non-const array is beyond me.

AdafruitDisplayAdapter adapter(matrix);
DisplayRenderer display(adapter, Colors::WHITE);

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

}

// MAIN LOOP - RUNS ONCE PER FRAME OF ANIMATION ----------------------------

void loop() {
  
  uint16_t x, y;
  matrix.fillScreen(0x0);
  RouteInfo R1 = {RouteType::RouteA1, Direction::EAST, 5};
  display.drawRoute(R1,0);
  
  matrix.show(); // TODO : put this in drawRoute
}