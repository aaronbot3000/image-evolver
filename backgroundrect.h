/*The BackgroundRect class of the ImageEvolverQt program
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

#ifndef BACKGROUNDRECT_H
#define BACKGROUNDRECT_H

#include <QPainter>

#include "mutablecolorshape.h"

namespace shapes
{
    class MutablePolygon;
}

class BackgroundRect : public MutableColorShape
{
    public:
		BackgroundRect(){}
        BackgroundRect(int w, int h, QColor c);
        virtual bool mutate();
        virtual void draw(QPainter *painter);
        virtual QString toSVG();
        virtual void reset();
        void setColor(QColor c);
		QColor getColor();

    private:
        QColor color;
        QRectF rect;
};

#endif // BACKGROUNDRECT_H
