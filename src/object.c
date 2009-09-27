
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

#include "object.h"

VTable VTableObject[] =
{
  {0, 0, (VirtualFunctionPointer) objKill}
};

Object * objCreate(Object * this)
{
  // check if we were called by a derived class
  if (this == 0)
    this = (Object *) malloc(sizeof(Object));

  if (this)
  {
    this->pVTable = VTableObject;
#ifdef DEBUG_OBJECTS_ON
    commonPrintf("objCreate()\n");
#endif
  }

  return this;
}

void objKill(bool dynamic, Object * this)
{
#ifdef DEBUG_OBJECTS_ON
  commonPrintf("~objKill()\n");
#endif
  this->pVTable = VTableObject;

  if (dynamic) free(this);
}
