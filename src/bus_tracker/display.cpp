#pragma once
#include "display.h"
#include "config.h"

DisplayRenderer::DisplayRenderer(DisplayInterface &matrix, uint16_t textColor, uint16_t antiColor) : matrix(matrix), textColor(textColor), antiColor(antiColor)
{
    this->matrix.fillScreen(0x0);
}

/*
* Draws a row given route information
* PARAMETERS:
*  route (required): the current route to display
*  row (required):   the row to display this on, valid input is from 0 to 3.
*/
void DisplayRenderer::drawRoute(const BusArrivalInfo& route, int row)
{

    uint16_t color = ColorFromRouteType(route.routeInfo.type);
    uint8_t rowOffset = 16*row;
    this->drawCircle(1, 1 + rowOffset,color);
    
    switch(route.routeInfo.type){
        case RouteType::RouteA:
            this->drawA(6, 5 + rowOffset, textColor);
            break;
        case RouteType::RouteA1:
            this->drawA(3, 5 + rowOffset, textColor);
            this->drawSmallOne(10, 7 + rowOffset, textColor);
            break;
        case RouteType::RouteA2:
            this->drawA(3, 5 + rowOffset, textColor);
            this->drawSmallTwo(10, 7 + rowOffset, textColor);
            break;
        case RouteType::RouteB:
            this->drawB(6, 5 + rowOffset, antiColor);
            break;
        case RouteType::RouteC:
            this->drawC(6, 5 + rowOffset, antiColor);
            break;
        case RouteType::RouteC1:
            this->drawC(3, 5 + rowOffset, antiColor);
            this->drawSmallOne(10, 7 + rowOffset, antiColor);
            break;
        case RouteType::RouteC2:
            this->drawC(3, 5 + rowOffset, antiColor);
            this->drawSmallTwo(10, 7 + rowOffset, antiColor);
            break;
        case RouteType::RouteD:
            this->drawD(4, 4 + rowOffset, antiColor);
            break;
        case RouteType::RouteD1:
            this->drawD(3, 5 + rowOffset, antiColor);
            this->drawSmallOne(10, 7 + rowOffset, antiColor);
            break;
        case RouteType::RouteD2:
            this->drawD(3, 5 + rowOffset, antiColor);
            this->drawSmallTwo(10, 7 + rowOffset, antiColor);
            break;
        case RouteType::Route75:
            this->draw75(3, 5 + rowOffset, antiColor);
            break;
    }

    switch(route.routeInfo.dir)
    {
        case Direction::EAST:
            this->drawRightArrow(17, 5 + rowOffset);
            break;
        case Direction::WEST:
            this->drawLeftArrow(17, 5 + rowOffset);
            break;
    }
    
    if(route.minutes >= 60)
    {
        this->drawNumber(1, 39, 3 + rowOffset);
        this->drawHourPlusText(48, 8 + rowOffset);
    }
    else
    {
        int firstDigit = route.minutes / 10;
        int secondDigit = route.minutes % 10;

        if(firstDigit>0)
        {
            this->drawNumber(firstDigit, 32, 3 + rowOffset);
        }
        this->drawNumber(secondDigit, 41, 3 + rowOffset);
        this->drawMinText(51, 7 + rowOffset);
    }

    this->matrix.show(); // TODO: put this in a row controller? 
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
void DisplayRenderer::drawA(int x, int y, uint16_t color)
{
    this->matrix.drawHLine(x + 1, y + 0, 4, color);
    this->matrix.drawHLine(x + 1, y + 3, 5, color);
    this->matrix.drawVLine(x + 5, y + 1, 6, color);
    this->matrix.drawVLine(x + 0, y + 1, 6, color);
}

/*
*       ***
*       *  *
*       *  *
*       ***
*       *  *
*       *  *
*       ***
*/
void DisplayRenderer::drawB(int x, int y, uint16_t color)
{
    this->matrix.drawVLine(x + 0, y + 0, 7, color);
    this->matrix.drawVLine(x + 3, y + 1, 2, color);
    this->matrix.drawVLine(x + 3, y + 4, 2, color);
    this->matrix.drawHLine(x + 0, y + 0, 3, color);
    this->matrix.drawHLine(x + 0, y + 3, 3, color);
    this->matrix.drawHLine(x + 0, y + 6, 3, color);
}

/*
*        ****
*       **   *
*       *
*       *
*       *
*       **   *
*        ****
*/
void DisplayRenderer::drawC(int x, int y, uint16_t color)
{
    this->matrix.drawVLine(x + 0, y + 1, 5, color);
    this->matrix.drawHLine(x + 1, y + 0, 4, color);
    this->matrix.drawHLine(x + 1, y + 6, 4, color);
    this->matrix.drawPixel(x + 1, y + 1, color);
    this->matrix.drawPixel(x + 5, y + 1, color);
    this->matrix.drawPixel(x + 5, y + 5, color);
    this->matrix.drawPixel(x + 1, y + 5, color);
}

/*
*       *****
*       *    *
*       *    *
*       *    *
*       *    *
*       *    *
*       *****
*/
void DisplayRenderer::drawD(int x, int y, uint16_t color)
{
    this->matrix.drawVLine(x + 0, y + 0, 7, color);
    this->matrix.drawVLine(x + 5, y + 1, 6, color);
    this->matrix.drawHLine(x + 0, y + 0, 5, color);
    this->matrix.drawHLine(x + 0, y + 7, 5, color);
}

/*
*       ****  ****
*          *  *
*         *   *
*        *    ****
*        *       *
*        *       *
*        *    ****
*/
void DisplayRenderer::draw75(int x, int y, uint16_t color)
{
    // 7
    this->matrix.drawHLine(x + 0, y + 0, 4, color);
    this->matrix.drawLine(x + 3, y + 1, x + 1, y + 3, color);
    this->matrix.drawVLine(x + 1, y + 3, 4, color);

    // 5
    this->matrix.drawHLine(x + 6, y + 0, 4, color);
    this->matrix.drawHLine(x + 6, y + 3, 4, color);
    this->matrix.drawHLine(x + 6, y + 6, 4, color);
    this->matrix.drawVLine(x + 6, y + 1, 2, color);
    this->matrix.drawVLine(x + 9, y + 4, 2, color);
}

/*
*        *
*       **
*        *
*        *
*       ***
*/
void DisplayRenderer::drawSmallOne(int x, int y, uint16_t color)
{
    this->matrix.drawHLine(x + 0, y + 1, 2, color);
    this->matrix.drawHLine(x + 0, y + 4, 3, color);
    this->matrix.drawVLine(x + 1, y + 0, 5, color);
}

/*
*       ***
*         *
*       ***
*       *
*       ***
*/
void DisplayRenderer::drawSmallTwo(int x, int y, uint16_t color)
{
    this->matrix.drawHLine(x + 0, y + 0, 3, color);
    this->matrix.drawHLine(x + 0, y + 2, 3, color);
    this->matrix.drawHLine(x + 0, y + 4, 3, color);
    this->matrix.drawPixel(x + 2, y + 1, color);
    this->matrix.drawPixel(x + 0, y + 3, color);
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
*          *
*         **
*        ***
*          *
*          *
*          *
*          *
*          * 
*          *
*          *
*       *******
*/
 void DisplayRenderer::drawLargeOne(int x, int y)
 {
    this->matrix.drawLine(x + 1, y + 2, x + 3, y + 0, textColor);
    this->matrix.drawPixel(x + 2, y + 2, textColor);
    this->matrix.drawVLine(x + 3, y + 0, 11, textColor);
    this->matrix.drawHLine(x + 0, y + 10, 7, textColor);
 }

/*
*        *****
*       *     *
*             *
*             *  
*             *
*          ***
*         *     
*        *
*       *
*       *
*       *******
*/
void DisplayRenderer::drawLargeTwo(int x, int y)
{
    this->matrix.drawPixel(x + 0, y + 1, textColor);
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawVLine(x + 6, y + 1, 4, textColor);
    this->matrix.drawHLine(x + 4, y + 5, 2, textColor);
    this->matrix.drawLine(x + 3, y + 5, x + 0, y + 8, textColor);
    this->matrix.drawPixel(x + 0, y + 9, textColor);
    this->matrix.drawHLine(x + 0, y + 10, 7, textColor);
}

/*
*        *****
*       *     *
*             *
*             *
*             *
*           ***
*             *
*             *
*             *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeThree(int x, int y)
{
    this->matrix.drawPixel(x + 0, y + 1, textColor);
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawVLine(x + 6, y + 1, 9, textColor);
    this->matrix.drawHLine(x + 3, y + 5, 3, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
    this->matrix.drawPixel(x + 0, y + 9, textColor);
}

/*
*       *     *
*       *     *
*       *     * 
*       *     *
*       *     *
*        ******
*             *
*             *
*             *
*             *
*             *
*/
void DisplayRenderer::drawLargeFour(int x, int y)
{
    this->matrix.drawVLine(x + 0, y + 0, 5, textColor);
    this->matrix.drawHLine(x + 1, y + 5, 6, textColor);
    this->matrix.drawVLine(x + 6, y + 0, 11, textColor);
}

/*
*       *******
*       *
*       *
*       *
*       *
*       ******
*             *
*             *
*             *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeFive(int x, int y)
{
    this->matrix.drawHLine(x + 0, y + 0, 7, textColor);
    this->matrix.drawVLine(x + 0, y + 0, 6, textColor);
    this->matrix.drawHLine(x + 0, y + 5, 6, textColor);
    this->matrix.drawVLine(x + 6, y + 6, 4, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
    this->matrix.drawPixel(x + 0, y + 9, textColor);
}

/*
*        *****
*       *     *
*       *
*       *
*       *
*       ******
*       *     *
*       *     *
*       *     *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeSix(int x, int y)
{
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawVLine(x + 0, y + 1, 9, textColor);
    this->matrix.drawHLine(x + 0, y + 5, 6, textColor);
    this->matrix.drawVLine(x + 6, y + 6, 4, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
}
/*
*       *******
*             *
*            *
*           *
*          *
*         *
*         *
*         *
*         *
*         *
*         *
*/
void DisplayRenderer::drawLargeSeven(int x, int y)
{
    this->matrix.drawHLine(x + 0, y + 0, 7, textColor);
    this->matrix.drawLine(x + 6, y + 1, x + 2, y + 5, textColor);
    this->matrix.drawVLine(x + 2, y + 6, 5, textColor);
}

/*
*        *****
*       *     *
*       *     *
*       *     *
*       *     *
*        *****
*       *     *
*       *     *
*       *     *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeEight(int x, int y)
{
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawHLine(x + 1, y + 5, 5, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
    this->matrix.drawVLine(x + 0, y + 1, 4, textColor);
    this->matrix.drawVLine(x + 6, y + 1, 4, textColor);
    this->matrix.drawVLine(x + 0, y + 6, 4, textColor);
    this->matrix.drawVLine(x + 6, y + 6, 4, textColor);
}

/*
*        *****
*       *     *
*       *     *
*       *     *
*       *     *
*        ******
*             *
*             *
*             *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeNine(int x, int y)
{
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawHLine(x + 1, y + 5, 5, textColor);
    this->matrix.drawVLine(x + 0, y + 1, 4, textColor);
    this->matrix.drawVLine(x + 6, y + 1, 9, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
    this->matrix.drawPixel(x + 0, y + 9, textColor);
}

/*
*        *****
*       *     *
*       *     *
*       *    **
*       *   * *
*       *  *  *
*       * *   *
*       **    *
*       *     *
*       *     *
*        *****
*/
void DisplayRenderer::drawLargeZero(int x, int y) 
{
    this->matrix.drawVLine(x + 0, y + 1, 9, textColor);
    this->matrix.drawVLine(x + 6, y + 1, 9, textColor);
    this->matrix.drawHLine(x + 1, y + 0, 5, textColor);
    this->matrix.drawHLine(x + 1, y + 10, 5, textColor);
    this->matrix.drawLine(x + 5, y + 3, x + 1, y + 7, textColor);
}

void DisplayRenderer::drawNumber(int number, int x, int y)
{
    switch(number)
    {
        case 0:
            this->drawLargeZero(x, y);
            break;
        case 1:
            this->drawLargeOne(x, y);
            break;
        case 2:
            this->drawLargeTwo(x, y);
            break;
        case 3:
            this->drawLargeThree(x, y);
            break;
        case 4:
            this->drawLargeFour(x, y);
            break;
        case 5: 
            this->drawLargeFive(x, y);
            break;
        case 6:
            this->drawLargeSix(x, y);
            break;
        case 7:
            this->drawLargeSeven(x, y);
            break;
        case 8:
            this->drawLargeEight(x, y);
            break;
        case 9:
            this->drawLargeNine(x, y);
            break;
        default:
            return;
    }
    return;
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
    // h
    this->matrix.drawVLine(x + 0, y + 0, 6, textColor);
    this->matrix.drawHLine(x + 1, y + 2, 2, textColor);
    this->matrix.drawVLine(x + 3, y + 3, 3, textColor);

    // r
    this->matrix.drawVLine(x + 5, y + 0, 6, textColor);
    this->matrix.drawPixel(x + 6, y + 1, textColor);
    this->matrix.drawHLine(x + 7, y + 0, 2, textColor);
    this->matrix.drawPixel(x + 7, y + 0, textColor);

    // +
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
void DisplayRenderer::drawMinText(int x, int y)
{
    // m
    this->matrix.drawVLine(x + 0, y + 2, 5, textColor);
    this->matrix.drawVLine(x + 2, y + 2, 5, textColor);
    this->matrix.drawVLine(x + 4, y + 3, 4, textColor);
    this->matrix.drawHLine(x + 0, y + 2, 4, textColor);

    // i
    this->matrix.drawPixel(x + 6, y + 0, textColor);
    this->matrix.drawVLine(x + 6, y + 2, 5, textColor);

    // n
    this->matrix.drawVLine(x + 8, y + 2, 5, textColor);
    this->matrix.drawVLine(x + 10,y + 3, 4, textColor);
    this->matrix.drawHLine(x + 8, y + 2, 3, textColor);
}
