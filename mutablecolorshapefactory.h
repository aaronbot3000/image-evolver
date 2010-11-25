/*The MutableColorShapeFactory class of the ImageEvolverQt program
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

#ifndef MUTABLECOLORSHAPEFACTORY_H
#define MUTABLECOLORSHAPEFACTORY_H

#include "mutablecolorshape.h"
#include "mutableellipse.h"
#include "mutablepolygon.h"
#include "mutablerectangle.h"
#include "mutableline.h"

namespace shapes
{
   class MutableShapeFactory;
}

class MutableColorShapeFactory
{
   public:
       static const int TypePolygon = 0;
       static const int TypeRectangle = 1;
       static const int TypeEllipse = 2;
               static const int TypeLine = 3;

       static MutableColorShape* createShape(int type, int xBounds,
int yBounds)
       {
           switch(type)
           {
               case TypePolygon:
                   return new MutablePolygon(xBounds, yBounds);
                   break;
               case TypeRectangle:
                   return new MutableRectangle(xBounds, yBounds);
                   break;
               case TypeEllipse:
                   return new MutableEllipse(xBounds, yBounds);
                   break;
                               case TypeLine:
                                       return new MutableLine(xBounds, yBounds);
                                       break;
               default:
                   return new MutablePolygon(xBounds, yBounds);
                   break;
           }
       }
}
#endif //MUTABLECOLORSHAPEFACTORY_H
