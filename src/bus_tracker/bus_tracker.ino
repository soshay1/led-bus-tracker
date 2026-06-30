#pragma once
#include "routes.h"
#include "RowController.h"
#include "busAPIClient.h"
#include "display.h"
#include "config.h"
#include "WifiManager.h"

static const int rowY[] = { 0, 16, 32, 48 }; // row 0–3 y-coordinates

Adafruit_Protomatter matrix(
  WIDTH, 4, 1, (uint8_t*)Config::rgbPins, Config::numAddrPins, (uint8_t*)Config::addrPins,
  Config::clockPin, Config::latchPin, Config::oePin, true,-2); // This function does NOT need to modify the input pins so why it demands a non-const array is beyond me.

AdafruitDisplayAdapter adapter(matrix);
DisplayRenderer display(adapter, Colors::WHITE, Colors::BLACK);
BusAPIClient BusAPIClient;
RowController rowController(&display);
int counter = 0;

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

  ProtomatterStatus status = matrix.begin();
  Serial.printf("Protomatter begin() status: %d\n", status);

  wifiInit();

}


void loop() {

  if(counter>=60) {
    counter = 0;
  }
  
  if(counter==0) {
    std::vector<BusArrivalInfo> routes = BusAPIClient.getStopData();
    Serial.print(routes.size());
    rowController.update(routes);
  }

  matrix.fillScreen(0x0);
  rowController.refresh();
  matrix.show();

  counter+=5;
  delay(5000);

}