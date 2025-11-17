#pragma once
#include "display.h"
#include "config.h"

DisplayRenderer::DisplayRenderer(DisplayInterface &matrix, uint16_t textColor) : matrix(matrix), textColor(textColor)
{}

/*
* Draws a row given route information
* PARAMETERS:
*  route (required): the current route to display
*  row (required):   the row to display this on, valid input is from 0 to 3.
*/
void DisplayRenderer::drawRoute(const RouteInfo& route, int row)
{
    uint16_t color = ColorFromRouteType(route.type);
    uint8_t rowOffset = 16*row;
    this->drawCircle(1, 1 + rowOffset,color);
    
    switch(route.type){
        case RouteType::RouteA1:
            this->drawA(3, 5 + rowOffset);
            this->drawSmallOne(10, 7 + rowOffset);
            break;
        case RouteType::RouteA2:
            this->drawA(3, 5 + rowOffset);
            this->drawSmallTwo(10, 7 + rowOffset);
            break;
    }

    switch(route.dir)
    {
        case Direction::EAST:
            this->drawRightArrow(17, 5 + rowOffset);
            break;
        case Direction::WEST:
            this->drawLeftArrow(17, 5 + rowOffset);
            break;
    }
 
    if(route.minutesUntilNextBus >= 60)
    {
        ; // pass, TODO
    }

    return;
}

/*
* Draws a circle of radius 7 pixels given a position and color
* PARAMETERS:
*  x (required): the x coordinate of the upper left corner of the circle
*  y (required): the y coordinate of the upper left corner of the circle
*  color (required): the rgb565 color 
* NOTES:
* There is an Adafruit function to draw a circle but it's ugly
*/
void DisplayRenderer::drawCircle(int x, int y, uint16_t color)
{
    int circleLines[4][2] = {
        {6, 1}, // 1 line of 6, etc
        {10, 1},
        {12, 2},
        {14, 3}
    };

    int currPart = 0; // index of array being drawn
    int offsetY = 0;
    int offsetX;
    int dir = 1;

    do
    {
        int numLines = circleLines[currPart][1];
        int width = circleLines[currPart][0];
        offsetX = (14 - width) / 2;
        for (int i = 0; i < numLines; i++)
        {
            this->matrix.drawHLine(x + offsetX, y + offsetY, width, color);
            offsetY += 1;
        }

        currPart += dir;

        if (currPart >= 4)
        {
            dir = -1;
            currPart+=dir;
        }

    } while (currPart >= 0);
}

/*
*        ****
*       *    *
*       *    *
*       ******
*       *    *
*       *    *
*       *    *
*/
void DisplayRenderer::drawA(int x, int y)
{
    this->matrix.drawHLine(x + 1, y + 0, 4, textColor);
    this->matrix.drawHLine(x + 1, y + 3, 5, textColor);
    this->matrix.drawVLine(x + 5, y + 1, 6, textColor);
    this->matrix.drawVLine(x + 0, y + 1, 6, textColor);
}

/*
*        *
*       **
*        *
*        *
*       ***
*/
void DisplayRenderer::drawSmallOne(int x, int y)
{
    this->matrix.drawHLine(x + 0, y + 1, 2, textColor);
    this->matrix.drawHLine(x + 0, y + 4, 3, textColor);
    this->matrix.drawVLine(x + 1, y + 0, 5, textColor);
}

/*
*       ***
*         *
*       ***
*       *
*       ***
*/
void DisplayRenderer::drawSmallTwo(int x, int y)
{
    this->matrix.drawHLine(x + 0, y + 0, 3, textColor);
    this->matrix.drawHLine(x + 0, y + 2, 3, textColor);
    this->matrix.drawHLine(x + 1, y + 4, 3, textColor);
    this->matrix.drawPixel(x + 2, y + 1, textColor);
    this->matrix.drawPixel(x + 0, y + 3, textColor);
}

/*
*         *
*        **
*       ***
*       ***       
*        **
*         *
*/
void DisplayRenderer::drawLeftArrow(int x, int y)
{
    this->matrix.drawVLine(x + 0, y + 2, 2, textColor);
    this->matrix.drawVLine(x + 1, y + 1, 4, textColor);
    this->matrix.drawVLine(x + 2, y + 0, 6, textColor);
    
}

/*
*       *
*       **
*       ***
*       ***     
*       **
*       *
*/
void DisplayRenderer::drawRightArrow(int x, int y)
{
    this->matrix.drawVLine(x + 0, y + 0, 6, textColor);
    this->matrix.drawVLine(x + 1, y + 1, 4, textColor);
    this->matrix.drawVLine(x + 2, y + 2, 2, textColor);
}

/*
*       *    * **
*       *    **  *   *
*       ***  *       *
*       *  * *     *****
*       *  * *       *
*       *  * *       *
*/
void DisplayRenderer::drawHourPlusText(int x, int y)
{
    this->matrix.drawVLine(x + 0, y + 0, 6, textColor);
    this->matrix.drawHLine(x + 1, y + 2, 2, textColor);
    this->matrix.drawVLine(x + 3, y + 3, 3, textColor);
    this->matrix.drawVLine(x + 5, y + 0, 6, textColor);
    this->matrix.drawPixel(x + 6, y + 1, textColor);
    this->matrix.drawHLine(x + 7, y + 0, 2, textColor);
    this->matrix.drawPixel(x + 7, y + 0, textColor);
    this->matrix.drawHLine(x + 9, y + 3, 5, textColor);
    this->matrix.drawVLine(x + 11, y + 1, 5, textColor);
}

/*
*              *
*
*       ****   *  ***
*       * * *  *  *  *
*       * * *  *  *  *
*       * * *  *  *  *
*       * * *  *  *  *
*/
/*void DisplayRenderer::drawHourPlusText(int x, int y)
{
    this->matrix.drawVLine(x + 0, y + 0, 6, textColor);
    this->matrix.drawHLine(x + 1, y + 2, 2, textColor);
    this->matrix.drawVLine(x + 3, y + 3, 3, textColor);
    this->matrix.drawVLine(x + 5, y + 0, 6, textColor);
    this->matrix.drawPixel(x + 6, y + 1, textColor);
    this->matrix.drawHLine(x + 7, y + 0, 2, textColor);
    this->matrix.drawPixel(x + 7, y + 0, textColor);
    this->matrix.drawHLine(x + 9, y + 3, 5, textColor);
    this->matrix.drawVLine(x + 11, y + 1, 5, textColor);
}*/
