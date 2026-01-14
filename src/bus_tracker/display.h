#pragma once
#include <Wire.h>
#include <Adafruit_LIS3DH.h>
#include <Adafruit_Protomatter.h>
#include "routes.h"
#include "display.h"
#include "config.h"
#include "colors.h"
#include "AdafruitDisplayAdapter.h"
#include <cstdint>

#ifndef DISPLAY_H
#define DISPLAY_H

class DisplayRenderer {
    private:
        DisplayInterface &matrix;
        uint16_t textColor;
        uint16_t antiColor;
        void drawCircle(int x, int y, uint16_t color);
        void drawA(int x, int y, uint16_t color);
        void drawB(int x, int y, uint16_t color);
        void drawC(int x, int y, uint16_t color);
        void drawD(int x, int y, uint16_t color);
        void draw75(int x, int y, uint16_t color);
        void drawSmallOne(int x, int y, uint16_t color);
        void drawSmallTwo(int x, int y, uint16_t color);
        void drawLargeOne(int x, int y);
        void drawLargeTwo(int x, int y);
        void drawLargeThree(int x, int y);
        void drawLargeFour(int x, int y);
        void drawLargeFive(int x, int y);
        void drawLargeSix(int x, int y);
        void drawLargeSeven(int x, int y);
        void drawLargeEight(int x, int y);
        void drawLargeNine(int x, int y);
        void drawLargeZero(int x, int y);
        void drawNumber(int number, int x, int y);
        void drawMinText(int x, int y);
        void drawHourPlusText(int x, int y);
        void drawLeftArrow(int x, int y);
        void drawRightArrow(int x, int y);
        uint16_t color565(uint8_t red, uint8_t green, uint8_t blue); // Copied the matrix function here because it's a one-liner 

    public:
        void drawRoute(const RouteInfo& route, int row);
        DisplayRenderer(DisplayInterface &matrix, uint16_t textColor, uint16_t antiColor);
};
#endif
