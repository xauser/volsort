
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

#ifndef PROGRESS_H 
#define PROGRESS_H

#include "common.h"
#include "object.h"
#include "fsnode.h"

typedef struct progress Progress;

struct progress
{
	// members of obj
	VTable *pVTable;

	// our own members
	uint64_t size_total;
	uint64_t size_now;
	uint32_t files_total;
	uint32_t files_now;
	time_t ts;
	bool verbose;
};

void progressKill(bool dynamic, Progress *this);
Progress *progressCreate(Progress *this, uint64_t size_total, uint32_t files_total, bool verbose);
void progressUpdate(Progress *this, FsNode *fsn);

#endif
