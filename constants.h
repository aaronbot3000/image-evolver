/*The Constants class of the ImageEvolverQt program
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

#ifndef CONSTANTS_H
#define CONSTANTS_H

class Constants
{
    public:
        static int C_MUTATE_ADD_POINT;
        static int C_MUTATE_DEL_POINT;
        static int C_MUTATE_MOVE_POINT;
        static int C_DELTA_POINT;
        static int C_MUTATE_COLOR;
        static int C_DELTA_COLOR;
        static int C_ADD_SHAPE;
        static int C_MOVE_SHAPE;
        static int C_REMOVE_SHAPE;
        static int C_UPDATE_SPEED;
        static int C_AUTOSAVE_SPEED;
};

#endif //CONSTANTS_H
