
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

#ifndef FILESYSTEM_H
#define FILESYSTEM_H

#include "fsnode.h"

int32_t fsCopyFile(char const * src, char const * dest);

int32_t fsMkDir(char const * dir);

void fsMkDirP(char const * dir);

void fsScanDir(FsNode * parent);

bool fsIsDir(char const * elem);

bool fsIsFile(char const * elem);

bool fsExists(char const * elem);

uint64_t fsSize(char const * elem);

#endif
