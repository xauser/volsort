
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

#include "logic.h"
#include "volume.h"
#include "filesystem.h"
#include "progress.h"
#include <string.h>

void logicCreatePackages(FsNode * fsn, List * volumes, Options * options)
{
  Element * elemIt = listFirst(nodeGetChildrenList((Node *) fsn));
  while (elemIt)
  {
    FsNode * fschild = (FsNode *) elemData(elemIt);

    // we take file nodes even when depth is not reached
    if (fsnIsDir(fschild))
    {
      // if we've reached preferred depth take everything
      if (nodeGetDepth((Node *) fschild) == opGetDepth(options))
      {
#ifdef DEBUG_LOGIC_ON
        fsnDump(fschild);
        strPrintf("[LOGIC-] ADD DIRECTORY    %s\n",
            strPtr(fsnGetName(fschild)));
#endif
        logicTakeFsNode(volumes, fschild, options);
      }
      // take directory if it got no children
      else if (listSize(nodeGetChildrenList((Node *) fschild)) == 0)
      {
#ifdef DEBUG_LOGIC_ON
        fsnDump(fschild);
        strPrintf("[LOGIC-] ADD EMPTY DIRECTORY %s\n",
            strPtr(fsnGetName(fschild)));
#endif
        logicTakeFsNode(volumes, fschild, options);
      }
      // depth is not reached and we have children
      else
      {
#ifdef DEBUG_LOGIC_ON
        fsnDump(fschild);
        strPrintf("[LOGIC-] GOING DOWN TO:   %s\n",
            strPtr(fsnGetFolder(fschild)));
#endif
        logicCreatePackages(fschild, volumes, options);
      }
    }
    else
    {
      // always take file nodes
#ifdef DEBUG_LOGIC_ON
      fsnDump(fschild);
      strPrintf("[LOGIC-] ADD FILE:        %s\n",
          strPtr(fsnGetName(fschild)));
#endif
      logicTakeFsNode(volumes, fschild, options);
    }
    elemIt = elemNext(elemIt);
  }
}

void logicTakeFsNode(List * volumes, FsNode * fsn, Options * options)
{
  Element * elemIt = (Element *) listFirst(volumes);
  bool inserted = FALSE;
  int counter = 0;

  // check for packages bigger then volume capacity
  if (fsnCountAllFileSizes(fsn) > (opGetCapacity(options) - opGetExtraSpace(options)))
  {
    strPrintf(
        "'%s' (%d mb) does not fit into a %d mb volume\n",
        strPtr(fsnGetName(fsn)),
        (int)(fsnCountAllFileSizes(fsn)/1024/1024),
        opGetCapacity(options)/1024/1024);
    strPrintf("Try a higher package depth or increase volume capacity.\n");
    exit(EXIT_SUCCESS);
  }

  // try to insert it into an already created volume
  // (First Fit) strategy
  while (elemIt)
  {
    counter++;
    Volume * vol = (Volume *) elemData(elemIt);

    if (volAddFsNode(vol, fsn))
    {
#ifdef DEBUG_LOGIC_ON
      strPrintf("[LOGIC-] ADDED TO VOLUME: %d\n", counter);
      volDump(vol);
#endif
      inserted = TRUE;
      break;
    }
    else
      elemIt = elemNext(elemIt);
  }

  // ok, this node couldn't get inserted in an already present volume
  // we have to create an new empty one for it
  if (!inserted)
  {
    Volume * newvol = volCreate(0, opGetCapacity(options), opGetExtraSpace(options));

    if (volAddFsNode(newvol, fsn))
    {
      listAppend(volumes, (Object *) newvol);
#ifdef DEBUG_LOGIC_ON
      strPrintf("[LOGIC-] ADDED TO VOLUME: %d\n", listSize((List *) volumes));
      volDump(newvol);
#endif
    }
  }
}

void logicDumpVolumes(List * volumes)
{
  Element * elemIt = (Element *) listFirst(volumes);
  uint64_t counter = 0;

  while (elemIt)
  {
    counter++;
    strPrintf("------------------------\n",counter);
    strPrintf("VOLUME #%d      \n",counter);
    strPrintf("------------------------\n",counter);
    Volume * volume = (Volume *) elemData(elemIt);
    volDump(volume);
#ifdef DEBUG_LOGIC_ON
    volDumpFsNodes(volume,FALSE);
#endif
    elemIt = (Element *) elemNext(elemIt);
  }
}

void logicCopyVolumes(List * volumes, Progress * progress, Options * options)
{
  // check that destination directory exixts
  if (!fsExists(strPtr(opGetDestFolder(options))))
    fsMkDir(strPtr(opGetDestFolder(options)));

  // loop over our volumes
  uint8_t counter = 0;
  Element * volumeIt = listFirst(volumes);
  while (volumeIt)
  {
    counter++;

    // construct the volume directory
    String * volumeDir = strCreate(0, strPtr(opGetDestFolder(options)));
    strAppendCstr(volumeDir, "/");
    strAppendCstr(volumeDir, strPtr(opGetVolumePrefix(options)));
    strAppendUint32(volumeDir, counter);

    // create the volume directory
#ifdef DEBUG_LOGIC_ON
    strPrintf("[LOGIC ] MKDIR %s\n", strPtr(volumeDir));
#endif
    fsMkDir(strPtr(volumeDir));

    // walk over nodes for copy
    Volume * volume = (Volume *) elemData(volumeIt);
    Element * fsnIt = listFirst(volPackageList(volume));
    while (fsnIt)
    {
      FsNode * fsn = (FsNode *) elemData(fsnIt);
      logicCopyPackage(fsn, opGetRootFolder(options), volumeDir, progress);
      fsnIt = elemNext(fsnIt);
    }

    (* volumeDir->pVTable[0].pFunc) (TRUE, volumeDir);
    volumeIt = elemNext(volumeIt);
  }
}

String * logicAssembleDestFolder(String * nodeDir, String * rootDir, String * volumeDir)
{
  char const * substr = strPtr(nodeDir);
  substr += strlen(rootDir->data);
  String * packageDestDir = strCreate(0, strPtr(volumeDir));
  strAppendCstr(packageDestDir, substr);
  return packageDestDir;
}

void logicCopyPackage(FsNode * fsn, String * rootDir, String * volumeDir, Progress * progress)
{
  String * packageDestDir = logicAssembleDestFolder(fsnGetFolder(fsn), rootDir, volumeDir);

  // create package directory in destination
#ifdef DEBUG_LOGIC_ON
  strPrintf("[LOGIC ] MKDIR %s\n",strPtr(packageDestDir));
#endif
  fsMkDirP(strPtr(packageDestDir));

  if (fsnIsDir(fsn))
  {
    // loop over children
    Element * fschildIt = listFirst(nodeGetChildrenList((Node *) fsn));
    while (fschildIt)
    {
      FsNode * fschild = (FsNode *) elemData(fschildIt);
      logicCopyPackage(fschild, rootDir, volumeDir, progress);
      fschildIt = elemNext(fschildIt);
    }
  }
  else
  {
    // create filenames
    String * copySource = strCreate(0, strPtr(fsnGetFolder(fsn)));
    strAppendCstr(copySource, "/");
    strAppendCstr(copySource, strPtr(fsnGetName(fsn)));

    String * copyDest = strCreate(0, strPtr(packageDestDir));
    strAppendCstr(copyDest, "/");
    strAppendCstr(copyDest, strPtr(fsnGetName(fsn)));

#ifdef DEBUG_LOGIC_ON
    strPrintf("[LOGIC ] COPY %s TO %s\n", strPtr(copySource), strPtr(copyDest));
#endif

    // copy file here
    fsCopyFile(strPtr(copySource), strPtr(copyDest));
    progressUpdate(progress, fsn);

    // free filenames
    (* copySource->pVTable[0].pFunc) (TRUE, copySource);
    (* copyDest->pVTable[0].pFunc) (TRUE, copyDest);
  }

  (* packageDestDir->pVTable[0].pFunc) (TRUE, packageDestDir);
}

uint64_t logicCountWastedSpace(List * volumes)
{
  Element * volumeIt = (Element *) listFirst(volumes);
  uint64_t wasted = 0;

  while (volumeIt)
  {
    Volume * volume = (Volume *) elemData(volumeIt);
    wasted += volFreeSpace(volume) - volExtraSpace(volume);
    volumeIt = (Element *) elemNext(volumeIt);
  }

  return wasted;
}
