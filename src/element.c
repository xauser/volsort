
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

#include "element.h"

VTable VTableElement[] =
{
  {0, 0, (VirtualFunctionPointer) elemKill}
};

Element * elemCreate(Element * this, Object * data)
{
  if (this == 0)
    this = (Element *) malloc(sizeof(Element));

  if (this)
  {
    objCreate((Object *) this);
    this->pVTable = VTableElement;
#ifdef DEBUG_OBJECTS_ON
    strPrintf("elemCreate()\n");
#endif
    this->data = data;
    this->next = 0;
  }

  return this;
}

void elemKill(bool dynamic, Element * this)
{
#ifdef DEBUG_OBJECTS_ON
  strPrintf("~elemKill()\n");
#endif
  this->pVTable = VTableElement;

  objKill(FALSE, (Object *) this);

  if (dynamic) free(this);
}

Object * elemData(Element * this)
{
  return this->data;
}

Element * elemNext(Element * this)
{
  return this->next;
}

