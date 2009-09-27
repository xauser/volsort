
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

#ifndef FSNODE_H
#define FSNODE_H

#include "common.h"
#include "node.h"
#include "string.h"

typedef struct fsn FsNode;

struct fsn
{
  VTable * pVTable;

  Node * parent;
  List * children;
  uint16_t depth;

  String * name;
  String * folder;
  int isDir;
  uint64_t size;
};

FsNode * fsnCreate(
		FsNode * this,
		FsNode * parent,
		char const * name,
		char const * folder);

void fsnKill(bool dynamic, FsNode * this);

void fsnAddChild(FsNode * this, FsNode * child);

void fsnDump(FsNode * this);

String * fsnGetName(FsNode * this);

void fsnSetFolder(FsNode * this, String * folder);

/* Returns the member folder */
String * fsnGetFolder(FsNode * this);

int fsnIsDir(FsNode * this);

/* Sets the member isDir */
void fsnSetDir(FsNode * this, bool isDir);

uint64_t fsnGetFileSize(FsNode * this);

void fsnSetFileSize(FsNode * this, uint64_t size);

void fsnDumpChildren(FsNode * this, bool recursive);

uint64_t fsnCountAllFiles(FsNode * this);

uint64_t fsnCountAllFileSizes(FsNode * this);

uint64_t fsnCountAllDirectories(FsNode * this);

#endif
