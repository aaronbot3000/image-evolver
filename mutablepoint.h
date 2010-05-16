/*The MutablePoint class of the ImageEvolverQt program
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

#ifndef MUTABLEPOINT_H
#define MUTABLEPOINT_H

#include <QPointF>

#include "constants.h"
#include "utils.h"

namespace shapes
{
    class MutablePolygon;
}

class MutablePoint : public QPointF
{
    public:
        MutablePoint():xBounds(0), yBounds(0){}
        MutablePoint(int xB, int yB, float x, float y);
        QString toSVG();
        bool mutate();
        void reset();

    private: 
		const int xBounds, yBounds;
        float prevX, prevY;
        bool changed;
};

#endif // MUTABLEPOINT_H
