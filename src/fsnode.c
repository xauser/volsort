
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

#include "fsnode.h"

VTable VTableFsNode[] =
{
  {0, 0, (VirtualFunctionPointer) fsnKill}
};

FsNode * fsnCreate(FsNode * this, FsNode * parent, char const * name, char const * folder)
{
  if (this == 0)
    this = (FsNode *) malloc(sizeof(FsNode));

  if (this)
  {
    nodeCreate((Node *) this, (Node *) parent);
    this->pVTable = VTableFsNode;
#ifdef DEBUG_OBJECTS_ON
    strPrintf("fsnCreate(%s)\n", name);
#endif
    this->name = strCreate(0, name);
    this->folder = strCreate(0, folder);
    this->isDir = 0;
    this->size = 0;
  }

  return this;
}

void fsnKill(bool dynamic, FsNode * this)
{
#ifdef DEBUG_OBJECTS_ON
  strPrintf("~fsnKill(%s)\n", strPtr(this->name));
#endif
  this->pVTable = VTableFsNode;

  strKill(TRUE, this->name);
  strKill(TRUE, this->folder);

  nodeKill(FALSE, (Node *) this);

  if (dynamic) free(this);
}

void fsnAddChild(FsNode * this, FsNode * child)
{
  listAppend(this->children, (Object *) child);
}

void fsnDump(FsNode * this)
{
  // verbosing our direct info
  strPrintf("Name:            %s\n", strPtr(fsnGetName(this)));
  if (this->parent != 0)
    strPrintf("Parent:          %s\n", strPtr(fsnGetFolder((FsNode *) this->parent)));
  strPrintf("Is dir:          %d\n", fsnIsDir(this));
  strPrintf("Folder:          %s\n", strPtr(fsnGetFolder(this)));
  strPrintf("Size Rec:        %lld mb\n", (fsnCountAllFileSizes(this)/1024/1024));
  strPrintf("Children direct: %d\n", listSize(nodeGetChildrenList((Node *) this)));
  strPrintf("Children all:    %d\n", nodeCountAllChildren((Node *) this));
  strPrintf("Depth:           %d\n", nodeGetDepth((Node *) this));
  strPrintf("Files:           %d\n", fsnCountAllFiles(this));
}

void fsnDumpChildren(FsNode * this, bool recursive)
{
  Element * elemIt = listFirst(nodeGetChildrenList((Node *) this));

  while (elemIt)
  {
    FsNode * fsn = (FsNode *) elemData(elemIt);
    fsnDump(fsn);
    if (recursive)
      fsnDumpChildren(fsn, TRUE);
    elemIt = elemNext(elemIt);
  }
}

uint64_t fsnCountAllFiles(FsNode * this)
{
  uint64_t count = 0;

  if (fsnIsDir(this))
  {
    Element * elemIt = listFirst(nodeGetChildrenList((Node *) this));

    while (elemIt)
    {
      FsNode * fsn = (FsNode *) elemData(elemIt);
      if (fsnIsDir(fsn))
        count += fsnCountAllFiles(fsn);
      else
        count++;
      elemIt = elemNext(elemIt);
    }
  }
  else
    count = 1;

  return count;
}

uint64_t fsnCountAllDirectories(FsNode *this)
{
  uint64_t count = 0;

  if (fsnIsDir(this))
  {
    count++;
    Element * elemIt = listFirst(nodeGetChildrenList((Node *) this));
    while (elemIt)
    {
      FsNode *fsn = (FsNode *) elemData(elemIt);
      count += fsnCountAllDirectories(fsn);
      elemIt = elemNext(elemIt);
    }
  }

  return count;
}

String *fsnGetName(FsNode *this)
{
  return this->name;
}

void fsnSetFolder(FsNode *this, String *folder)
{
  this->folder = folder;
}

String * fsnGetFolder(FsNode * this)
{
  return this->folder;
}

int fsnIsDir(FsNode *this)
{
  return this->isDir;
}

void fsnSetDir(FsNode * this, bool isDir)
{
  this->isDir = isDir;
}

uint64_t fsnGetFileSize(FsNode *this)
{
  return this->size;
}

void fsnSetFileSize(FsNode *this, uint64_t size)
{
  this->size = size;
}

uint64_t fsnCountAllFileSizes(FsNode *this)
{
  uint64_t count = 0;

  if (fsnIsDir(this))
  {
    Element * elemIt = listFirst(nodeGetChildrenList((Node *) this));

    while (elemIt)
    {
      FsNode *fsn = (FsNode *) elemData(elemIt);

      count += fsnCountAllFileSizes(fsn);
      elemIt = elemNext(elemIt);
    }
  }
  else
    count = fsnGetFileSize(this);

  return count;
}
