/*The ImageMutationCore class of the ImageEvolverQt program
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

#ifndef IMAGEMUTATIONCORE_H
#define IMAGEMUTATIONCORE_H

#include <QColor>
#include <QDebug>
#include <QFile>
#include <QImage>
#include <QLinkedList>
#include <QMutex>
#include <QString>
#include <QStringRef>
#include <QTextStream>
#include <QThread>
#include <QVector>
#include <QPainter>
#include <QRegExp>
#include <QXmlStreamAttributes>
#include <QXmlStreamReader>

#include <math.h>

#include "mutablecolorshape.h"
#include "backgroundrect.h"
#include "mutablecolorshapefactory.h"

#include "constants.h"
#include "linkedlist.h"
#include "utils.h"
#include "pixelcounter.h"

namespace core
{
    class ImageMutationCore;
}

class ImageMutationCore : public QThread
{
    Q_OBJECT

    public:
        ImageMutationCore();
        ~ImageMutationCore();

        bool loadSVG(QString &path);
        bool loadImage(QString &path);
        bool saveBestImage(QString path);
        bool saveSVG(QString path);
       
        void startMutation();
        void stopMutation();
        void setBackground(QColor &color);
        double getCurrentScore();
        unsigned int getGoodMutationCount();
        unsigned int getTestedMutationCount();
        int getNumberOfPolygons();
        QImage & getCurrentImage();
        QImage & getOriginalImage();
        QColor getBackground();

        void enableShape(int type);
        bool isShapeEnabled(int type);
        void disableShape(int type);

        void waitForFinish();
        QMutex currentImageLock;
        volatile bool mutateIsOn;

        int countThreads();

    private:
        void mutate();
        void drawPolygons();
        void countScore();
        void seedPolygons();

        QImage original, current;
        QPainter painter;
        QMutex shapesLock;

        QVector<int> shapesToUse;
        LinkedList<MutableColorShape> shapes;
        LinkedList<PixelCounter> counters;

        unsigned int goodMutationCount, testedMutationCount, cores;
        unsigned long currentScore;
        double bestScore;

    protected:
        void run();
};

#endif // IMAGEMUTATIONCORE_H
