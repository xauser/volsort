
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

#include "list.h"

VTable VTableList[] =
{
  {0, 0, (VirtualFunctionPointer) listKill}
};

List * listCreate(List * this)
{
  if (this == 0)
    this = (List *) malloc(sizeof(List));

  if (this)
  {
    objCreate((Object *) this);
#ifdef DEBUG_OBJECTS_ON
    commonPrintf("listCreate()\n");
#endif
    this->pVTable = VTableList;

    this->first = 0;
    this->last = 0;
    this->list_size = 0;
  }

  return this;
}

void listKill(bool dynamic, List * this)
{
#ifdef DEBUG_OBJECTS_ON
  commonPrintf("~listKill()\n");
#endif
  this->pVTable = VTableList;

  listClear(this);
  objKill(FALSE, (Object *) this);
  if (dynamic) free(this);
}

void listAppend(List * this, Object * data)
{
  Element * elem = elemCreate(0, data);

  // do we already have elements in list
  if (!this->first)
  {
    this->first = elem;
    this->last = elem;
  }
  else
  {
    // append element at the end of list
    this->last->next = elem;
    this->last = elem;
  }

  this->list_size++;
}

void listClear(List * this)
{
  Element * elemIt = this->first;

  // Iterate over all our elements
  while (elemIt)
  {
    Element * tmp = elemIt;
    elemIt = elemIt->next;

    // Calling virtual destructor of current element
    (* tmp->pVTable[0].pFunc) (TRUE, tmp);
  }

  this->list_size = 0;
}

uint64_t listSize(List * this)
{
  return this->list_size;
}

Element * listFirst(List * this)
{
  return this->first;
}
