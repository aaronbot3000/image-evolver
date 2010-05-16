/*The BackgroundRect class of the ImageEvolverQt program
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

#include "backgroundrect.h"

BackgroundRect::BackgroundRect(int w, int h, QColor c):
	color(c), rect(0, 0, w, h){}

bool BackgroundRect::mutate()
{
    return false;
}

void BackgroundRect::draw(QPainter *painter)
{
	painter->setBrush(color);
	painter->drawRect(rect);
}

QString BackgroundRect::toSVG()
{
    QString svgString("<rect x=\"");
	svgString += "0\" y=\"0\" width=\"" + QString::number(rect.width()) + "\" height=\"" + QString::number(rect.height()) + "\" ";
	svgString += " fill=\"rgb(";
	svgString += QString::number(color.red()) + ",";
	svgString += QString::number(color.green()) + ",";
	svgString += QString::number(color.blue()) + ")\" opacity=\"";
	svgString += QString::number((((double)color.alpha())/255.0));
	svgString += ("\" />\n");
	
	return svgString;
}

void BackgroundRect::reset()
{
    return;
}

void BackgroundRect::setColor(QColor c)
{
    color = c;
}

QColor BackgroundRect::getColor()
{
    return color;
}
