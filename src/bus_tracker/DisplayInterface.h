#pragma once
#include <stdio.h>

class DisplayInterface {
    public:
        virtual ~DisplayInterface() = default;
        virtual void drawPixel(int16_t x, int16_t y, uint16_t color) = 0;
        virtual void drawVLine(int16_t x, int16_t y, int16_t length, uint16_t color) = 0;
        virtual void drawHLine(int16_t x, int16_t y, int16_t length, uint16_t color) = 0;
        virtual void drawLine(int16_t x0, int16_t x1, int16_t y0, int16_t y1, uint16_t color) = 0;
        virtual void fillScreen(uint16_t color) = 0;
        virtual void show() = 0;
};