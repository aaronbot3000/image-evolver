/*The MutableEllipse class of the ImageEvolverQt program
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

#ifndef MUTABLEELLIPSE_H
#define MUTABLEELLIPSE_H

#include <math.h>

#include "constants.h"
#include "utils.h"
#include "mutablecolorshape.h"
#include "mutablepoint.h"

namespace shapes
{
	class MutableEllipse;
}

class MutableEllipse : public MutableColorShape
{
    public:
        MutableEllipse():changed(false){}
        MutableEllipse(int xB, int yB);
        MutableEllipse(int xB, int yB, float x1, float y1, float x2, float y2, QColor c);
        virtual ~MutableEllipse(void){}
        virtual bool mutate();
        virtual void reset();
        virtual void draw(QPainter *painter);
        virtual QString toSVG();

    private:
        MutablePoint topLeft, bottomRight;
        QRectF boundingRect;
        QColor color;
        int prevRed, prevGreen, prevBlue, prevAlpha;
		bool changed;
};

#endif //MUTABLEELLIPSE_H
