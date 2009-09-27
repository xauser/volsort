
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

#ifndef LOGIC_H
#define LOGIC_H

#include "list.h"
#include "fsnode.h"
#include "progress.h"
#include "options.h"

void logicCreatePackages(FsNode * fsn, List * volumes, Options * options);

void logicTakeFsNode(List * volumes, FsNode * fsn, Options * options);

void logicDumpVolumes(List * volumes);

void logicCopyVolumes(List * volumes, Progress * progress, Options * options);

void logicCopyPackage(FsNode * fsn, String * rootDir, String * volumeDir, Progress * progress);

String * logicAssembleDestFolder(String * nodeDir, String * rootDir, String * volumeDir);

uint64_t logicCountWastedSpace(List * volumes);

#endif
