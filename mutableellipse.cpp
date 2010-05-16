/*The MutableEllipse class of the ImageEvolverQt program
 * Copyright (C) 2009  Aaron Fan
 * Version 2.1.0
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

#include "mutableellipse.h"

MutableEllipse::MutableEllipse(int xB, int yB) :
    color(Utils::getRand(256), Utils::getRand(256),Utils::getRand(256), Utils::getRand(60)+20),
    changed(false), topLeft(xB, yB, 0, 0), bottomRight(xB, yB, 0, 0),
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
}

MutableEllipse::MutableEllipse(int xB, int yB, float x1, float y1, float x2, float y2, QColor c) :
    topLeft(xB, yB, x1, y1), bottomRight(xB, yB, x2, y2),
    changed(false),
    prevRed(0), prevGreen(0), prevBlue(0), prevAlpha(0)
{
    color = c;
}

bool MutableEllipse::mutate()
{
    changed = false;
    prevRed = color.red();
    prevBlue = color.blue();
    prevGreen = color.green();
    prevAlpha = color.alpha();

    changed = Utils::mutateColor(color);
    changed = topLeft.mutate() || changed;
    changed = bottomRight.mutate() || changed;

    return changed;
}

void MutableEllipse::draw(QPainter *painter)
{
    boundingRect.setLeft(topLeft.x());
    boundingRect.setTop(topLeft.y());
    boundingRect.setRight(bottomRight.x());
    boundingRect.setBottom(bottomRight.y());

    painter->setBrush(color);
    painter->drawEllipse(boundingRect);
}

QString MutableEllipse::toSVG()
{
    QString svgString("<ellipse cx=\"");
    svgString += QString::number((topLeft.x() + bottomRight.x()) / 2.0) + "\" cy=\"" + QString::number((topLeft.y() + bottomRight.y()) / 2.0) +
            "\" rx=\"" + QString::number(fabs(topLeft.x() - bottomRight.x()) / 2.0) + "\" ry=\"" + QString::number(fabs(topLeft.y() - bottomRight.y()) / 2.0) + "\" ";
    svgString += "fill=\"rgb(";
    svgString += QString::number(color.red()) + ",";
    svgString += QString::number(color.green()) + ",";
    svgString += QString::number(color.blue()) + ")\" opacity=\"";
    svgString += QString::number((((double)color.alpha())/255.0));
    svgString += ("\" />\n");

    return svgString;
}

void MutableEllipse::reset()
{
    if (changed)
    {
        topLeft.reset();
        bottomRight.reset();

        color.setRgb(prevRed, prevGreen, prevBlue, prevAlpha);
        changed = false;
    }
}


