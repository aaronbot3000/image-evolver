/*The MutableLine class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.3
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

#include "mutableline.h"

MutableLine::MutableLine(int xB, int yB) :
       pointA(xB, yB, 0, 0), pointB(xB, yB, 0, 0),
       color(Utils::getRand(256), Utils::getRand(256),Utils::getRand(256),
Utils::getRand(60)+20),
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

       pointA.setX(initialX < otherX ? initialX : otherX);
       pointA.setY(initialY < otherY ? initialY : otherY);
       pointB.setX(initialX > otherX ? initialX : otherX);
       pointB.setY(initialY > otherY ? initialY : otherY);
}

MutableLine::MutableLine(int xB, int yB, float x1, float y1, float x2,
float y2, QColor c) :
       pointA(xB, yB, x1, y1), pointB(xB, yB, x2, y2),
       changed(false),
       prevRed(0), prevGreen(0), prevBlue(0), prevAlpha(0)
{
       color = c;
}

bool MutableLine::mutate()
{
       changed = false;
       prevRed = color.red();
       prevBlue = color.blue();
       prevGreen = color.green();
       prevAlpha = color.alpha();

       changed = Utils::mutateColor(color);
       changed = pointA.mutate() || changed;
       changed = pointB.mutate() || changed;

       /*if (pointB.x() < pointA.x())
       {
                       int temp = pointA.x();
                       pointA.setX(pointB.x());
                       pointB.setX(temp);
       }

       if (pointB.y() < pointA.y())
       {
                       int temp = pointA.y();
                       pointA.setY(pointB.y());
                       pointB.setY(temp);
       }*/

       return changed;
}

void MutableLine::draw(QPainter *painter)
{
       QPen pen(color, 3, Qt::SolidLine, Qt::RoundCap, Qt::RoundJoin);
       painter->setPen(pen);
       painter->setBrush(color);
       painter->drawLine(pointA.x(), pointA.y(), pointB.x(), pointB.y());
       painter->setPen(Qt::NoPen);
}

QString MutableLine::toSVG()
{
       QString svgString("<line x1=\"");
       svgString += QString::number(pointA.x()) + "\" y1=\"" +
QString::number(pointA.y()) + "\" x2=\"" + QString::number(pointB.x())
+ "\" y2=\"" + QString::number(pointB.y()) + "\" ";
       svgString += "style=\"stroke:rgb(";
       svgString += QString::number(color.red()) + ",";
       svgString += QString::number(color.green()) + ",";
       svgString += QString::number(color.blue()) + ");";
       svgString += "stroke-width:3\" ";
       svgString += "opacity=\"" +
QString::number((((double)color.alpha())/255.0)) + "\"";
       svgString += (" />\n");

       return svgString;
}

void MutableLine::reset()
{
       if (changed)
       {
               pointA.reset();
               pointB.reset();

               color.setRgb(prevRed, prevGreen, prevBlue, prevAlpha);
               changed = false;
       }
}
