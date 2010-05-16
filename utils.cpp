/*The Utils class of the ImageEvolverQt program
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


#include "utils.h"

CRandomMersenne Utils::randomizer(time(0));

int Utils::getRand(int limit)
{
    return randomizer.IRandom(0, limit-1);
}

bool Utils::mutateColor(QColor &color)
{
    int red = 0, green = 0, blue = 0, alpha = 0;
    color.getRgb(&red, &green, &blue, &alpha);

    bool changed=false;

    //Randomize the red component within a set Constants::C_DELTA
    if (getRand(Constants::C_MUTATE_COLOR)==1)
    {
        red = red+Constants::C_DELTA_COLOR-Utils::getRand(Constants::C_DELTA_COLOR*2+1);
        red = red<256?red:255;
        red = red>=0?red:0;

        changed=true;
    }

    //Randomize the green component within a set Constants::C_DELTA
    if (getRand(Constants::C_MUTATE_COLOR)==1)
    {
        green = green+Constants::C_DELTA_COLOR-Utils::getRand(Constants::C_DELTA_COLOR*2+1);
        green = green<256?green:255;
        green = green>=0?green:0;

        changed=true;
    }

    //Randomize the blue component within a set Constants::C_DELTA
    if (getRand(Constants::C_MUTATE_COLOR)==1)
    {
        blue = blue+Constants::C_DELTA_COLOR-Utils::getRand(Constants::C_DELTA_COLOR*2+1);
        blue = blue<256?blue:255;
        blue = blue>=0?blue:0;

        changed=true;
    }

    //Randomize the alpha component within a set Constants::C_DELTA
    if (getRand(Constants::C_MUTATE_COLOR)==1)
    {
        alpha = alpha+Constants::C_DELTA_COLOR-Utils::getRand(Constants::C_DELTA_COLOR*2+1);
        alpha = alpha<256?alpha:255;
        alpha = alpha>=0?alpha:0;

        changed=true;
    }

    color.setRgb(red, green, blue, alpha);

    return changed;
}
