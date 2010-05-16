/*The MutablePolygon class of the ImageEvolverQt program
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

#ifndef MUTABLEPOLYGON_H
#define MUTABLEPOLYGON_H

#include <QPainter>

#include "utils.h"
#include "linkedlist.h"
#include "constants.h"

#include "mutablecolorshape.h"
#include "mutablepoint.h"

namespace shapes
{
    class MutablePolygon;
}

class MutablePolygon : public MutableColorShape
{
    public:
        MutablePolygon(int xB, int yB);
        //MutablePolygon(int xB, int yB, QXMLStreamAttributes attr);
        MutablePolygon(int xB, int yB, QStringList &points, QColor c);
        ~MutablePolygon(){if (removedPoint != 0) delete removedPoint;}
        virtual bool mutate();
        virtual void draw(QPainter *painter);
        virtual QString toSVG();
        virtual void reset();

    private:
        QPolygonF ppolygon;
        QColor color;
        MutablePoint *removedPoint;
        LinkedList<MutablePoint> polygon;
        const int xBounds, yBounds;
		int prevRed, prevGreen, prevBlue, prevAlpha, addedIndex, removedIndex;
        bool changed;
};

#endif // MUTABLEPOLYGON_H
