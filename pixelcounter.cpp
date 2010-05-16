#include "pixelcounter.h"

PixelCounter::PixelCounter(){}

PixelCounter::PixelCounter(int i, int t, QSemaphore *s):
        threadNumber(i), totalThreads(t)
{
    semaphore = s;
    QObject::connect(this, SIGNAL(finished()), this, SLOT(end()));
}

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
    semaphore->acquire(1);
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

void PixelCounter::end()
{
    semaphore->release(1);
}
