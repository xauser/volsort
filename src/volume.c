
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

#include "volume.h"
#include "list.h"

VTable VTableVolume[] =
{
  {0, 0, (VirtualFunctionPointer) volKill}
};

Volume * volCreate(Volume * this, uint64_t capacity, uint32_t extraSpace)
{
  if (this == 0)
    this = (Volume *) malloc(sizeof(Volume));

  if (this)
  {
    objCreate((Object *) this);
#ifdef DEBUG_OBJECTS_ON
    strPrintf("volCreate\n");
#endif
    this->pVTable = VTableVolume;

    this->packages = (List *) listCreate(0);
    this->capacity = capacity;
    this->used_space = 0;
    this->extra_space = extraSpace;
  }

  return this;
}

void volKill(bool dynamic, Volume * this)
{
#ifdef DEBUG_OBJECTS_ON
  strPrintf("volKill\n");
#endif
  this->pVTable = VTableVolume;

  (* this->packages->pVTable[0].pFunc) (TRUE, this->packages);

  objKill(FALSE, (Object *) this);

  if (dynamic) free(this);
}

uint64_t volFreeSpace(Volume * this)
{
  return (this->capacity - this->used_space);
}

uint32_t volExtraSpace(Volume * this)
{
  return this->extra_space;
}

bool volAddFsNode(Volume * this, FsNode * fsn)
{
  uint64_t node_size = fsnCountAllFileSizes(fsn);

  if (node_size > (volFreeSpace(this) - this->extra_space))
    return FALSE;
  else
  {
    this->used_space += node_size;
    listAppend(volPackageList(this), (Object *) fsn);
    return TRUE;
  }
}

void volDump(Volume * this)
{
  strPrintf("Used space:      %d mb\n", (this->used_space/1024/1024));
  strPrintf("Free space:      %d mb\n", (volFreeSpace(this)/1024/1024));
  strPrintf("Extra space:     %d mb\n", (volExtraSpace(this)/1024/1024));
  strPrintf("Usage:           %d%\n", (this->used_space/(volCapacity(this)/100)));
  strPrintf("Packages:        %d\n", listSize(volPackageList(this)));
  strPrintf("Files:           %d\n", volCountFiles(this));
  strPrintf("Directories:     %d\n", volCountDir(this));
}

uint64_t volCountFiles(Volume * this)
{
  uint64_t count = 0;
  Element * elemIt = listFirst(volPackageList(this));

  while (elemIt)
  {
    FsNode * fsn = (FsNode *) elemData(elemIt);
    count += fsnCountAllFiles(fsn);
    elemIt = elemNext(elemIt);
  }

  return count;
}

uint64_t volCountDir(Volume * this)
{
  uint64_t count = 0;
  Element * elemIt = listFirst(volPackageList(this));

  while (elemIt)
  {
    FsNode * fsn = (FsNode *) elemData(elemIt);
    count += fsnCountAllDirectories(fsn);
    elemIt = elemNext(elemIt);
  }

  return count;
}

void volDumpFsNodes(Volume * this, bool recursive)
{
  uint16_t counter = 0;
  Element * elemIt = listFirst(volPackageList(this));
  while (elemIt)
  {
    counter++;
    strPrintf("-----------------------\n", counter);
    strPrintf("Package #%d\n", counter);
    strPrintf("-----------------------\n", counter);
    FsNode * fsn = ((FsNode *) elemData(elemIt));
    fsnDump(fsn);
    if (recursive)
      fsnDumpChildren(fsn,TRUE);
    elemIt = elemNext(elemIt);
  }
}

List * volPackageList(Volume * this)
{
  return this->packages;
}

uint64_t volCapacity(Volume * this)
{
  return this->capacity;
}
