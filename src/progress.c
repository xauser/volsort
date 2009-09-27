
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

#include "progress.h"

VTable VTableProgress[] =
{
  {0, 0, (VirtualFunctionPointer) progressKill}
};

Progress * progressCreate(Progress * this, uint64_t size_total, uint32_t files_total, bool verbose)
{
  if (this == 0)
    this = (Progress *) malloc(sizeof(Progress));

  if (this)
  {
    objCreate((Object *) this);
#ifdef DEBUG_OBJECTS_ON
    commonPrintf("progressCreate\n");
#endif
    this->pVTable = VTableProgress;
    this->size_total = size_total;
    this->size_now = 0;
    this->files_total = files_total;
    this->files_now = 0;
    this->ts = time(NULL);
    this->verbose = verbose;
  }

  return this;
}

void progressKill(bool dynamic, Progress * this)
{
#ifdef DEBUG_OBJECTS_ON
  commonPrintf("progressKill\n");
#endif
  this->pVTable = VTableProgress;
  objKill(FALSE, (Object *) this);
  if (dynamic) free(this);
}

void progressUpdate(Progress * this, FsNode * fsn)
{
  this->files_now++;

  if (this->verbose)
    commonPrintf("%.2d% \"%s\"\n",
				(this->files_now/(this->files_total/100)),
				strPtr(fsnGetName(fsn)));
}
