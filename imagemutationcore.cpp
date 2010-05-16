/*The ImageMutationCore class of the ImageEvolverQt program
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

#include "imagemutationcore.h"

long ImageMutationCore::currentScore = 0;

ImageMutationCore::ImageMutationCore():
    original(100, 100, QImage::Format_RGB32),
    current(100, 100, QImage::Format_RGB32),
    mutateIsOn(false), bestScore(0)
{
    shapes.prepend(new BackgroundRect(100, 100, QColor(255, 255, 255, 255)));
    shapesToUse.append(0);

    cores = countThreads();
    semaphore.release(cores - 1);
    for (int x=1; x<cores; x++)
        counters.append(new PixelCounter(x, cores, &semaphore));
}

ImageMutationCore::~ImageMutationCore()
{
    if (painter.isActive())
        painter.end();
    counters.removeAll();
}

bool ImageMutationCore::loadSVG(QString &path)
{
    mutateIsOn = false;

    waitForFinish();

    QFile file(path);
    bool retValue = file.open(QIODevice::ReadOnly | QIODevice::Text);

    if (retValue)
    {
        shapes.removeAll();

        QXmlStreamReader reader(static_cast<QIODevice*>(&file));
        QXmlStreamAttributes attrs;

        QString colorString;
        QStringList colorList;

        QString opacityString;
        int opacity;

        while(!reader.atEnd() && !reader.hasError())
        {
            QXmlStreamReader::TokenType token = reader.readNext();

            if(token == QXmlStreamReader::StartElement)
            {
                if(reader.name() == "polygon")
                {
                    attrs = reader.attributes();

                    colorString = attrs.value("fill").toString();
                    colorList = colorString.split(QRegExp(QString("[rgb(), ]+")), QString::SkipEmptyParts);

                    opacityString = attrs.value("opacity").toString();
                    opacity = (int)floor(((opacityString.toFloat())*255.0) + 0.5);

                    QString points = attrs.value("points").toString();
                    QStringList pointsList = points.split(QRegExp(QString("[ ,]+")), QString::SkipEmptyParts);

                    if (shapes.size() == 0)
                    {
                        shapes.append(new BackgroundRect(original.width(), original.height(),
                            QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
                    }
                    else
                        shapes.append(new MutablePolygon(original.width(), original.height(), pointsList,
                            QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
                }

                else if (reader.name() == "rect")
                {
                    attrs = reader.attributes();

                    colorString = attrs.value("fill").toString();
                    colorList = colorString.split(QRegExp(QString("[rgb(), ]+")), QString::SkipEmptyParts);

                    opacityString = attrs.value("opacity").toString();
                    opacity = (int)floor(((opacityString.toFloat())*255.0) + 0.5);

                    if (shapes.size() == 0)
                    {
                        shapes.append(new BackgroundRect(original.width(), original.height(),
                            QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
                    }

                    else
                    {
                        QString xString = attrs.value("x").toString();
                        float x = xString.toFloat();

                        QString yString = attrs.value("y").toString();
                        float y = yString.toFloat();

                        QString wString = attrs.value("width").toString();
                        float w = wString.toFloat();

                        QString hString = attrs.value("height").toString();
                        float h = hString.toFloat();

                        shapes.append(new MutableRectangle(original.width(), original.height(), x, y, w, h,
                            QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
                    }
                }

                else if (reader.name() == "ellipse")
                {
                    attrs = reader.attributes();

                    colorString = attrs.value("fill").toString();
                    colorList = colorString.split(QRegExp(QString("[rgb(), ]+")), QString::SkipEmptyParts);

                    opacityString = attrs.value("opacity").toString();
                    opacity = (int)floor(((opacityString.toFloat())*255.0) + 0.5);

                    QString cxString = attrs.value("cx").toString();
                    float cx = cxString.toFloat();

                    QString cyString = attrs.value("cy").toString();
                    float cy = cyString.toFloat();

                    QString rxString = attrs.value("rx").toString();
                    float rx = rxString.toFloat();

                    QString ryString = attrs.value("ry").toString();
                    float ry = ryString.toFloat();

                    shapes.append(new MutableEllipse(original.width(), original.height(), cx - rx, cy - ry, cx + rx, cy + ry,
                        QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
                }
            }
        }

        countScore();
        bestScore = currentScore;
        drawPolygons();
        goodMutationCount = 0;
        testedMutationCount = 0;
    }

    return retValue;
}

bool ImageMutationCore::loadImage(QString &path)
{
    mutateIsOn = false;

    waitForFinish();

    if (painter.isActive())
        painter.end();

    bool loadSuccess = original.load(path);
    if (loadSuccess)
    {
        if (painter.isActive())
            painter.end();
        current = QImage(original.width(), original.height(), QImage::Format_RGB32);
        painter.begin(&current);

        shapes.removeAll();
        shapes.prepend(new BackgroundRect(original.width(), original.height(), QColor(255, 255, 255)));
        seedPolygons();
        drawPolygons();

        painter.setPen(Qt::NoPen);
        shapes.get(0)->draw(&painter);

        bestScore = 255*3*original.width()*original.height();
        goodMutationCount = 0;
        testedMutationCount = 0;
    }

    return loadSuccess;
}

bool ImageMutationCore::saveBestImage(QString path)
{
    bool hasStarted = mutateIsOn;
    mutateIsOn = false;

    QImage best(current.width(), current.height(), QImage::Format_RGB32);

    waitForFinish();

    if (painter.isActive())
            painter.end();

    painter.begin(&best);
    painter.setRenderHint(QPainter::Antialiasing);
    painter.setPen(Qt::NoPen);

    for (int x = 0; x < shapes.size(); x++)
    {
        shapes.get(x)->draw(&painter);
    }

    painter.end();
    painter.begin(&current);
    painter.setRenderHint(QPainter::Antialiasing, false);
    painter.setPen(Qt::NoPen);

    bool retValue = best.save(path, "PNG", 100);

    if (hasStarted)
        startMutation();

    return retValue;
}

bool ImageMutationCore::saveSVG(QString path)
{
    bool hasStarted = mutateIsOn;
    mutateIsOn = false;

    waitForFinish();

    QFile file(path);
    bool retValue = file.open(QIODevice::WriteOnly | QIODevice::Text);

    if (retValue)
    {
        QTextStream out(&file);

        out << QString("<?xml version=\"1.0\" encoding=\"utf-8\"?>\n<!DOCTYPE svg PUBLIC \"-//W3C//DTD SVG 1.1//EN\" \"http://www.w3.org/Graphics/SVG/1.1/DTD/svg11.dtd\">\n<svg xmlns=\"http://www.w3.org/2000/svg\"\nxmlns:xlink=\"http://www.w3.org/1999/xlink\" xmlns:ev=\"http://www.w3.org/2001/xml-events\"\nversion=\"1.1\" baseProfile=\"full\"\nwidth=\"");
        out << QString::number(original.width());
        out << QString("px\" height=\"");
        out << QString::number(original.height());
        out << QString("px\">\n");

        for (int i = 0; i < shapes.size(); ++i)
        {
            out << shapes.get(i)->toSVG();
        }
        out << QString("</svg>\n");
        file.close();
    }

    if (hasStarted)
        startMutation();

    return retValue;
}

void ImageMutationCore::setBackground(QColor &color)
{
    bool restart = mutateIsOn;
    mutateIsOn = false;
    waitForFinish();

    static_cast<BackgroundRect*>(shapes.first())->setColor(color);
    
    drawPolygons();
    countScore();
    bestScore = currentScore;

    if (restart)
    {
        mutateIsOn = true;
        start();
    }
}

void ImageMutationCore::enableShape(int type)
{
    if (!shapesToUse.contains(type))
        shapesToUse.append(type);
}

bool ImageMutationCore::isShapeEnabled(int type)
{
    return shapesToUse.contains(type);
}

void ImageMutationCore::disableShape(int type)
{
    if (shapesToUse.contains(type))
        shapesToUse.remove(shapesToUse.indexOf(type));
}

QColor ImageMutationCore::getBackground()
{
    return static_cast<BackgroundRect*>(shapes.first())->getColor();
}

void ImageMutationCore::startMutation()
{
    mutateIsOn = true;
    start();
}

void ImageMutationCore::stopMutation()
{
    mutateIsOn = false;
    waitForFinish();
}

void ImageMutationCore::run()
{
    mutate();
}

void ImageMutationCore::mutate()
{
    bool changed = false;

    bool added;
    int addedIndex;

    bool moved;
    int moveIndex1;
    int moveIndex2;

    bool removed;
    int removedIndex;
    MutableColorShape *removedShape;
    MutableColorShape *temp;

    shapesLock.lock();

    while (mutateIsOn)
    {
        added = false;
        moved = false;
        removed = false;
        removedShape = NULL;

        //Remove a polygon from the image, so long as there is at least 2
        //shapes
        if (Utils::getRand(Constants::C_REMOVE_SHAPE) == 1 && shapes.size() > 2)
        {
            removedIndex = Utils::getRand(shapes.size()-1);
            removedIndex++;
            removedShape = shapes.remove(removedIndex);

            removed=true;
            changed=true;
        }

        //Add a polygon if it is within limits
        if (Utils::getRand(Constants::C_ADD_SHAPE) == 1)
        {
            if (shapes.size() > 1)
            {
                    addedIndex = Utils::getRand(shapes.size()-1);
                    addedIndex++;
            }
            else
                    addedIndex = 1;

            int rand = Utils::getRand(shapesToUse.size());
            shapes.insert(addedIndex, MutableColorShapeFactory::createShape(shapesToUse.at(rand), original.width(), original.height()));

            added=true;
            changed=true;
        }
        //Move a polygon so its position in the order of painting changes
        if (Utils::getRand(Constants::C_MOVE_SHAPE) == 1 && shapes.size() > 1)
        {
            moveIndex1=Utils::getRand(shapes.size()-1);
            moveIndex2=Utils::getRand(shapes.size()-1);
            moveIndex1++;
            moveIndex2++;
            temp = shapes.remove(moveIndex1);
            shapes.insert(moveIndex2, temp);

            moved=true;
            changed=true;
        }

        for (int x = 0; x < shapes.size(); x++)
        {
            changed = shapes.get(x)->mutate() || changed;
        }
        if (!changed)
        {
            continue;
        }

        drawPolygons();
        countScore();
        //If this new generated picture is fitter than the old one, use it
        if (currentScore <= bestScore)
        {
            goodMutationCount++;
            bestScore = currentScore;

            if (removed)
            {
                delete removedShape;
                removedShape = NULL;
            }
        }

        //Reset the shape list
        else
        {
            if (moved)
            {
                temp = shapes.remove(moveIndex2);
                shapes.insert(moveIndex1, temp);
            }

            if (added)
                shapes.removeAndDelete(addedIndex);
            for (int x = 0; x < shapes.size(); x++)
            {
                shapes.get(x)->reset();
            }
            if (removed)
            {
                shapes.insert(removedIndex, removedShape);
                removedShape = NULL;
            }
        }
        testedMutationCount++;
    }
    
    shapesLock.unlock();
}

void ImageMutationCore::drawPolygons()
{
    currentImageLock.lock();

    for (int x = 0; x < shapes.size(); x++)
    {
        shapes.get(x)->draw(&painter);
    }

    currentImageLock.unlock();
}

void ImageMutationCore::countScore()
{
    for(int x=0; x<counters.size(); x++)
    {
        counters.get(x)->setImages((QRgb*)original.scanLine(0), (QRgb*)current.scanLine(0), current.width(), current.height());
        counters.get(x)->startCounting();
    }

    currentScore = 0;
    QRgb* o = (QRgb*)original.scanLine(0);
    QRgb* c = (QRgb*)current.scanLine(0);
    int pixels = current.width()*current.height();

    int startPixel = 0;
    int endPixel = (double)pixels/(double)cores;

    for (int x=startPixel; x<endPixel; x++)
    {
        currentScore += fabs((float)(qRed(c[x]) - qRed(o[x]))) +
            fabs((float)(qGreen(c[x]) - qGreen(o[x]))) +
            fabs((float)(qBlue(c[x]) - qBlue(o[x])));
    }
    semaphore.acquire(counters.size());
    semaphore.release(counters.size());

    for(int x=0; x<counters.size(); x++)
    {
        currentScore += counters.get(x)->getDiff();
    }
}

void ImageMutationCore::seedPolygons()
{
    for (int x = 0; x < 5; x++)
    {
        int rand = Utils::getRand(shapesToUse.size());
        shapes.append(MutableColorShapeFactory::createShape(shapesToUse.at(rand), original.width(), original.height()));
    }
}

float ImageMutationCore::getCurrentScore()
{
    return (1.0-((float)bestScore/(float)(255*3*original.width()*original.height())))*100.0;
}

int ImageMutationCore::getGoodMutationCount()
{
    return goodMutationCount;
}

int ImageMutationCore::getTestedMutationCount()
{
    return testedMutationCount;
}

int ImageMutationCore::getNumberOfPolygons()
{
    return shapes.size();
}

QImage & ImageMutationCore::getCurrentImage()
{
    return current;
}

QImage & ImageMutationCore::getOriginalImage()
{
    return original;
}

void ImageMutationCore::waitForFinish()
{
    //shapesLock.lock();
    //shapesLock.unlock();
    sleep(1);
    //while(!shapesLock.tryLock());
}

#if defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
#   include <unistd.h>
#elif defined(Q_OS_MACX)
#   include <mach/mach.h>
#   include <mach/machine.h>
#elif defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
#	include <Windows.h>
#endif

int ImageMutationCore::countThreads()
{
    int cpuCount = 1;

#if defined(Q_OS_WIN32) || defined(Q_OS_WIN64)
    {
        SYSTEM_INFO    si;
        GetSystemInfo(&si);
        cpuCount = si.dwNumberOfProcessors;
    }
#elif defined(Q_OS_UNIX) && !defined(Q_OS_MACX)
    cpuCount = sysconf(_SC_NPROCESSORS_ONLN);
#elif defined(Q_OS_MACX)
   kern_return_t		kr;
   struct host_basic_info	hostinfo;
   unsigned int			count;

   count = HOST_BASIC_INFO_COUNT;
   kr = host_info(mach_host_self(), HOST_BASIC_INFO, (host_info_t)&hostinfo, &count);
   if(kr == KERN_SUCCESS) {
        cpuCount = hostinfo.avail_cpus;
//	totalMemory = hostinfo.memory_size; 			 //in bytes
   }

#endif

    if( cpuCount < 1 )
        cpuCount = 1;

    return cpuCount;
}
