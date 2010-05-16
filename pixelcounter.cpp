/*The Pixelcounter class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.1
 *
 *This program is free software: you can redistribute it and/or modify
 *it under the terms of the GNU General Public License as published by
 *the Free Software Foundation, either version 3 of the License, or
 *(at your option) any later version.
 *
 *This program is distributed in the hope that it will be useful,
 *but WITHOUT ANY WARRANTY; without even the implied warranty of
 *MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *GNU General Public License for more details.
 *
 *You should have received a copy of the GNU General Public License
 *along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

#include "pixelcounter.h"

PixelCounter::PixelCounter(){}

PixelCounter::PixelCounter(int i, int t):
        threadNumber(i), totalThreads(t)
{}

void PixelCounter::setImages(QRgb *o, QRgb *c, int w, int h)
{
    original = o;
    current = c;
    pixels = w*h;

    startPixel = ((double)pixels/(double)totalThreads)*threadNumber;
    endPixel = ((double)pixels/(double)totalThreads)*threadNumber+((double)pixels/(double)totalThreads);
}

void PixelCounter::startCounting()
{
    this->start();
}

void PixelCounter::run()
{
    diff = 0;

    /*
    // root-mean-square diffing
    for (int x=startPixel; x<endPixel; x++)
    {
        temp =
            (fabs((float)(qRed(current[x]) - qRed(original[x]))) +
            fabs((float)(qGreen(current[x]) - qGreen(original[x]))) +
            fabs((float)(qBlue(current[x]) - qBlue(original[x]))));
        diff += temp * temp;
        temp =
            (fabs((float)qRed(original[x])) +
            fabs((float)qGreen(original[x])) +
            fabs((float)qBlue(original[x])));
        orig += temp * temp;
    }
    diff = diff / orig;
    */

    for (int x=startPixel; x<endPixel; x++)
    {
        diff +=
            (fabs((float)(qRed(current[x]) - qRed(original[x]))) +
            fabs((float)(qGreen(current[x]) - qGreen(original[x]))) +
            fabs((float)(qBlue(current[x]) - qBlue(original[x]))));
    }
}

long PixelCounter::getDiff()
{
    return diff;
}
