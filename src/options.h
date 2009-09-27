
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

#ifndef OPTIONS_H
#define OPTIONS_H

#include "object.h"
#include "string.h"

typedef struct options Options;

struct options
{
	VTable * pVTable;

	String * rootFolder;
	String * rootName;
	String * destFolder;
	String * volumePrefix;
	uint64_t volumeCapacity;
	uint8_t packageDepth;
	uint32_t extraSpace;
	bool quiet;
};

void opKill(bool dynamic, Options * this);

Options * opCreate(Options * this);

void opPrintUsage(Options * this);

void opParse(Options * this, int argc, char * argv[]);

String * opGetRootFolder(Options * this);

String * opGetDestFolder(Options * this);

String * opGetVolumePrefix(Options * this);

uint64_t opGetCapacity(Options * this);

uint8_t opGetDepth(Options * this);

uint32_t opGetExtraSpace(Options * this);

bool opAreComplete(Options * this);

bool opIsQuiet(Options * this);

void opPrintVersion(Options * this);

#endif
