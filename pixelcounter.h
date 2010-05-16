/*The Pixelcounter class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.0
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

#ifndef PIXELCOUNTER_H
#define PIXELCOUNTER_H

#include <QDebug>
#include <QThread>
#include <QRgb>
#include <QSemaphore>

#include <math.h>

;namespace core
{
    class PixelCounter;
}

class PixelCounter : public QThread
{
    Q_OBJECT

    public:
        PixelCounter();
        PixelCounter(int i, int t, QSemaphore *s);
        void setImages(QRgb *o, QRgb *c, int w, int h);

        void startCounting();
        long getDiff();

    private:
        QRgb *original;
        QRgb *current;
        long diff;
        //double orig;
        int threadNumber, pixels, totalThreads, startPixel, endPixel;
        QSemaphore *semaphore;

    private slots:
        void end();

    protected:
        void run();
};

#endif // PIXELCOUNTER_H
