#pragma once
#include "DisplayInterface.h"

class AdafruitDisplayAdapter : public DisplayInterface {
    public:
        AdafruitDisplayAdapter(Adafruit_Protomatter &matrix): matrix(matrix){}

        void drawPixel(int16_t x, int16_t y, uint16_t color)
        {
            this->matrix.drawPixel(x,y,color);
        }
        
        void drawHLine(int16_t x, int16_t y, int16_t length, uint16_t color)
        {
            this->matrix.drawFastHLine(x,y,length,color);
        }

        void drawVLine(int16_t x, int16_t y, int16_t length, uint16_t color)
        {
            this->matrix.drawFastVLine(x,y,length,color);
        }

        void drawLine(int16_t x0, int16_t x1, int16_t y0, int16_t y1, uint16_t color)
        {
            this->matrix.drawLine(x0,x1,y0,y1,color);
        }

        void show()
        {
            this->matrix.show();
        }
        
    private:
        Adafruit_Protomatter &matrix;
};