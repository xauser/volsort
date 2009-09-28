
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

#ifndef LIST_H
#define LIST_H

#include "element.h"

typedef struct list List;

struct list
{
  VTable * pVTable;

  Element * first;
  Element * last;
  uint64_t list_size;
};

List * listCreate(List * this);

void listKill(bool dynamic, List * this);

void listAppend(List * list, Object * data);

void listClear(List * this);

uint64_t listSize(List * this);

Element * listFirst(List * this);

#endif
