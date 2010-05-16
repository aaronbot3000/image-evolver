/*The MutableRectangle class of the ImageEvolverQt program
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

#include "mutablerectangle.h"

MutableRectangle::MutableRectangle(int xB, int yB) :
	topLeft(xB, yB, 0, 0), bottomRight(xB, yB, 0, 0),
    color(Utils::getRand(256), Utils::getRand(256),Utils::getRand(256), Utils::getRand(60)+20),
	changed(false),
    prevRed(0), prevGreen(0), prevBlue(0), prevAlpha(0)
{
    int initialX = Utils::getRand(xB);
    int initialY = Utils::getRand(yB);

    int otherX = initialX + 3 - Utils::getRand(7);
    otherX = otherX < xB ? otherX : xB - 1;
    otherX = otherX >= 0 ? otherX : 0;

    int otherY = initialY + 3 - Utils::getRand(7);
    otherY = otherY < yB ? otherY : yB - 1;
    otherY = otherY >= 0 ? otherY : 0;

    topLeft.setX(initialX < otherX ? initialX : otherX);
    topLeft.setY(initialY < otherY ? initialY : otherY);
    bottomRight.setX(initialX > otherX ? initialX : otherX);
    bottomRight.setY(initialY > otherY ? initialY : otherY);
}

MutableRectangle::MutableRectangle(int xB, int yB, float x, float y, float w, float h, QColor c) :
    topLeft(xB, yB, x, y), bottomRight(xB, yB, x + w, y + h),
    rect(x, y, w, h), changed(false),
    prevRed(0), prevGreen(0), prevBlue(0), prevAlpha(0)
{
	color = c;
}

bool MutableRectangle::mutate()
{
    changed = false;
    prevRed = color.red();
    prevBlue = color.blue();
    prevGreen = color.green();
    prevAlpha = color.alpha();

    changed = Utils::mutateColor(color);
    changed = topLeft.mutate() || changed;
    changed = bottomRight.mutate() || changed;

    /*if (bottomRight.x() < topLeft.x())
    {
            int temp = topLeft.x();
            topLeft.setX(bottomRight.x());
            bottomRight.setX(temp);
    }

    if (bottomRight.y() < topLeft.y())
    {
            int temp = topLeft.y();
            topLeft.setY(bottomRight.y());
            bottomRight.setY(temp);
    }*/

    return changed;
}

void MutableRectangle::draw(QPainter *painter)
{
    rect.setLeft(topLeft.x());
    rect.setTop(topLeft.y());
    rect.setRight(bottomRight.x());
    rect.setBottom(bottomRight.y());
    painter->setBrush(color);
    painter->drawRect(rect);
}

QString MutableRectangle::toSVG()
{
    QString svgString("<rect x=\"");
    svgString += QString::number((topLeft.x() < bottomRight.x() ? topLeft.x() : bottomRight.x())) + "\" y=\"" + QString::number((topLeft.y() < bottomRight.y() ? topLeft.y() : bottomRight.y())) + "\" width=\"" + QString::number(fabs(topLeft.x() - bottomRight.x())) + "\" height=\"" + QString::number(fabs(topLeft.y() - bottomRight.y())) + "\" ";
    svgString += "fill=\"rgb(";
    svgString += QString::number(color.red()) + ",";
    svgString += QString::number(color.green()) + ",";
    svgString += QString::number(color.blue()) + ")\" opacity=\"";
    svgString += QString::number((((double)color.alpha())/255.0));
    svgString += ("\" />\n");

    return svgString;
}

void MutableRectangle::reset()
{
    if (changed)
    {
        topLeft.reset();
        bottomRight.reset();

        //rect.setTopLeft(topLeft);
        //rect.setBottomRight(bottomRight);

        color.setRgb(prevRed, prevGreen, prevBlue, prevAlpha);
        changed = false;
    }
}


