/*The MutableColorShape class of the ImageEvolverQt program
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

#ifndef MUTABLECOLORSHAPE_H
#define MUTABLECOLORSHAPE_H

#include <QPainter>

namespace shapes
{
    class MutableColorShape;
}

class MutableColorShape
{
    public:
        virtual ~MutableColorShape(){}
        virtual bool mutate() = 0;
        virtual void reset() = 0;
        virtual void draw(QPainter *painter) = 0;
        virtual QString toSVG() = 0;
};

#endif // MUTABLECOLORSHAPE_H
