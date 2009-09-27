
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
 * along with volsort.  If not, see <http://www.gnu.org/licenses/>.
 */

#ifndef VOLUNIT_H_
#define VOLUNIT_H_

#include "element.h"

typedef struct volunit VolUnit;

struct volunit
{
  // fields inherited from Element
  Element * next;
  VTable * pVTable;

  // own members
  uint8_t * filename;
  uint8_t * name;
  uint64_t size;
  uint32_t volnr;
};

VolUnit * vuCreate(
		VolUnit * this,
		Element * next,
		char const * filename,
		char const * name,
		uint64_t size,
		uint32_t volnr);

void vuKill(bool dynamic, VolUnit * this);

#endif /*VOLUNIT_H_*/
