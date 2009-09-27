
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

#ifndef VOLUME_H
#define VOLUME_H

#include "fsnode.h"

typedef struct volume Volume;

struct volume
{
  VTable * pVTable;

  uint64_t capacity;
  uint64_t used_space;
  List * packages;
  uint32_t extra_space;
};

Volume * volCreate(Volume * this, uint64_t capacity, uint32_t extraSpace);

void volKill(bool dynamic, Volume * this);

bool volAddFsNode(Volume * this, FsNode * fsn);

uint64_t volFreeSpace(Volume * this);

uint32_t volExtraSpace(Volume * this);

void volDump(Volume * this);

uint64_t volCapacity(Volume * this);

List * volPackageList(Volume * this);

uint64_t volCountFiles(Volume * this);

uint64_t volCountDir(Volume * this);

void volDumpFsNodes(Volume * this, bool recursive);

#endif /*VOLUME_H*/
