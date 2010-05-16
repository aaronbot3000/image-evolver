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
