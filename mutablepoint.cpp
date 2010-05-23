/*The MutablePoint class of the ImageEvolverQt program
 * Copyright (C) 2009-2010  Aaron Fan
 * Version 3.2
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

#include "mutablepoint.h"

MutablePoint::MutablePoint(int xB, int yB, float x, float y):
	QPointF(x, y), xBounds(xB), yBounds(yB),
	prevX(0), prevY(0),
	changed(false){}

bool MutablePoint::mutate()
{
    changed=false;

    prevX = x();
    prevY = y();
	
    if (Utils::getRand(Constants::C_MUTATE_MOVE_POINT) == 1)
    {
        rx() += Constants::C_DELTA_POINT - Utils::getRand(Constants::C_DELTA_POINT*2+1);
        rx() = rx() <= xBounds ? rx() : xBounds;
        rx() = rx() >= 0 ? rx() : 0;

        changed=true;
    }

    if (Utils::getRand(Constants::C_MUTATE_MOVE_POINT) == 1)
    {
        ry() += Constants::C_DELTA_POINT - Utils::getRand(Constants::C_DELTA_POINT*2+1);
        ry() = ry() <= yBounds ? ry() : yBounds;
        ry() = ry() >= 0 ? ry() : 0;
		
        changed=true;
    }

    return changed;
}

void MutablePoint::reset()
{
    if (changed)
    {
        setX(prevX);
        setY(prevY);

        changed=false;
    }
    return;
}

QString MutablePoint::toSVG()
{
    return QString(QString::number((int)x()) + "," + QString::number((int)y()) + " ");
}
