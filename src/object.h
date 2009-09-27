
/* Copyright 2009 Ralf Schmitt
 *
 * This file is part of volsort.
 *
 * volsort is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * volsort is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with volsort. If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef OBJECT_H
#define OBJECT_H

//#define DEBUG_OBJECTS_ON

#include <stdlib.h>
#include "types.h"

typedef struct object Object;
typedef struct vtable VTable;
typedef void (* VirtualFunctionPointer)(bool, ...);

struct object
{
  VTable * pVTable;
};

struct vtable
{
  int d;
  int i;
  VirtualFunctionPointer pFunc;
};

void objKill(bool dynamic, Object * this);

Object * objCreate(Object * this);

#endif
