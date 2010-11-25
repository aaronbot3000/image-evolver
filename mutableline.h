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

#ifndef MUTABLELINE_H
#define MUTABLELINE_H

#include <math.h>

#include "constants.h"
#include "utils.h"
#include "mutablecolorshape.h"
#include "mutablepoint.h"

namespace shapes
{
       class MutableLine;
}

class MutableLine : public MutableColorShape
{
       public:
               MutableLine():changed(false){}
               MutableLine(int xB, int yB);
               MutableLine(int xB, int yB, float x1, float y1, float x2, float y2, QColor c);
               virtual ~MutableLine(void){}
               virtual bool mutate();
               virtual void reset();
               virtual void draw(QPainter *painter);
               virtual QString toSVG();

       private:
               MutablePoint pointA, pointB;
               QColor color;
               bool changed;
               int prevRed, prevGreen, prevBlue, prevAlpha;
};

#endif //MUTABLELINE_H
