/*The MutablePolygon class of the ImageEvolverQt program
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

#include "mutablepolygon.h"

MutablePolygon::MutablePolygon(int xB, int yB): 
    xBounds(xB), yBounds(yB), addedIndex(-1), removedIndex(-1), changed(false),
    color(Utils::getRand(256), Utils::getRand(256),Utils::getRand(256), Utils::getRand(60)+20)
{
    int initialX = Utils::getRand(xBounds);
    int initialY = Utils::getRand(yBounds);

    polygon.append(new MutablePoint(xBounds, yBounds, initialX, initialY));

    removedPoint = 0;

    for (int i = 1; i < 4; i++)
    {
        int xToAppend = initialX + 3 - Utils::getRand(7);
        xToAppend = xToAppend < xBounds ? xToAppend : xBounds - 1;
        xToAppend = xToAppend >= 0 ? xToAppend : 0;

        int yToAppend = initialY + 3 - Utils::getRand(7);
        yToAppend = yToAppend < yBounds ? yToAppend : yBounds - 1;
        yToAppend = yToAppend >= 0 ? yToAppend : 0;

        polygon.append(new MutablePoint(xBounds, yBounds, xToAppend, yToAppend));
    }
}

/*MutablePolygon::MutablePolygon(int xB, int yB, QXmlStreamAttributes attr):
    xBounds(xB), yBounds(yB)
{
    QString colorString = attrs.value("fill").toString();
    QStringList colorList = colorString.split(QRegExp(QString("[rgb(), ]+")), QString::SkipEmptyParts);

    opacityString = attrs.value("opacity").toString();
    opacity = (int)floor(((opacityString.toFloat())*255.0) + 0.5);

    color =

    QString points = attrs.value("points").toString();
    QStringList pointsList = points.split(QRegExp(QString("[ ,]+")), QString::SkipEmptyParts);
    shapes.append(new MutablePolygon(original.width(), original.height(), pointsList,
        QColor(colorList.at(0).toInt(), colorList.at(1).toInt(), colorList.at(2).toInt(), opacity)));
}*/

MutablePolygon::MutablePolygon(int xB, int yB, QStringList &points, QColor c):
    xBounds(xB), yBounds(yB), addedIndex(-1), removedIndex(-1), changed(false)
{
    for (int x = 0; x < points.length(); x += 2)
        polygon.append(new MutablePoint(xBounds, yBounds, points.at(x).toFloat(), points.at(x+1).toFloat()));

    removedPoint = 0;

    color = c;
}

bool MutablePolygon::mutate()
{
    changed = false;
    prevRed = color.red();
    prevBlue = color.blue();
    prevGreen = color.green();
    prevAlpha = color.alpha();

    if (removedPoint != 0)
         delete removedPoint;

    removedIndex = -1;
    removedPoint = 0;

    addedIndex = -1;

    changed = Utils::mutateColor(color);

    //Add a point into a random position in this polygon. The point is
    //placed at the midway between the points before and after it.
    if (Utils::getRand(Constants::C_MUTATE_ADD_POINT)==1)
    {
        addedIndex = Utils::getRand(polygon.size());
        int xToAdd = ((polygon.get(addedIndex)->x() + polygon.get((addedIndex + 1) % polygon.size())->x()) / 2);
        int yToAdd = ((polygon.get(addedIndex)->y() + polygon.get((addedIndex + 1) % polygon.size())->y()) / 2);

        polygon.insert(addedIndex, new MutablePoint(xBounds, yBounds, xToAdd, yToAdd));

        changed=true;
    }

    //Removes a random point. The minimum number of points in a polygon is
    //three, so we don't have lines
    if (Utils::getRand(Constants::C_MUTATE_DEL_POINT)==1&&polygon.size()>3)
    {
        removedIndex = Utils::getRand(polygon.size());

        removedPoint = polygon.remove(removedIndex);

        changed=true;
    }

    //Iterate through each point and test to move the points within a set
    //C_DELTA
    for (int x = 0; x < polygon.size(); x++)
    {
        changed = polygon.get(x)->mutate() || changed;
    }

    return changed;
}

void MutablePolygon::draw(QPainter *painter)
{
    ppolygon.remove(0, ppolygon.size());
    for (int x = 0; x < polygon.size(); x++)
    {
        ppolygon << (*polygon.get(x));
    }

    painter->setBrush(color);
    painter->drawPolygon(ppolygon);
}

QString MutablePolygon::toSVG()
{
    QString svgString("<polygon points=\"");
    for (int i = 0; i < polygon.size(); i++)
    {
        svgString += polygon.get(i)->toSVG();
    }
    svgString += "\" fill=\"rgb(";
    svgString += QString::number(color.red()) + ",";
    svgString += QString::number(color.green()) + ",";
    svgString += QString::number(color.blue()) + ")\" opacity=\"";
    svgString += QString::number((((double)color.alpha())/255.0));
    svgString += ("\" />\n");

    return svgString;
}

void MutablePolygon::reset()
{
    if (changed)
    {
        for (int x = 0; x < polygon.size(); x++)
        {
            polygon.get(x)->reset();
        }

        if (removedIndex != -1)
            polygon.insert(removedIndex, removedPoint);

        if (addedIndex != -1)
        {
            polygon.removeAndDelete(addedIndex);
        }

        removedPoint = 0;

        color.setRgb(prevRed, prevGreen, prevBlue, prevAlpha);
        changed = false;
    }
}
