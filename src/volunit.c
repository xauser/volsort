
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

#include "volunit.h"

#include <stdio.h>
#include <string.h>

VTable VTableVolUnit[] =
{
  {0, 0, (VirtualFunctionPointer) vuKill}
};

VolUnit * vuCreate(
		VolUnit * this,
		Element * next,
		char const * filename,
		char const * name,
		uint64_t size,
		uint32_t volnr)
{
  // check if we were called by a derived class
  if (this == 0)
    this = (VolUnit *) malloc(sizeof(VolUnit));

  if (this)
  {
    // contructing parent class
    elemCreate((Element *) this,0);
    // set/overwrite our virtual table
    this->pVTable = VTableVolUnit;

    printf("vuCreate\n");

    // initialise members
    this->next = next;
    this->filename = strdup(filename);
    this->name = strdup(name);
    this->size = size;
    this->volnr = volnr;
  }

  return this;
}

void vuKill(bool dynamic, VolUnit *this)
{
  // restore vtable
  this->pVTable = VTableVolUnit;

  printf("vuKill\n");

  // freeing our memebrs
  free(this->filename);
  free(this->name);

  // call destructor of base class
  elemKill(FALSE, (Element *) this);

  if (dynamic) free(this);
}
