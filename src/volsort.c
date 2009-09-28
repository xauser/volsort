
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

#include "element.h"
#include "list.h"
#include "filesystem.h"
#include "string.h"
#include "logic.h"
#include "options.h"

int32_t main(int32_t argc, char * argv[])
{
  Options * options = opCreate(0);
  opParse(options, argc, argv);
  if (!opAreComplete(options))
    exit(EXIT_SUCCESS);

  FsNode * fsn = fsnCreate(
      0,
      0,
      "ROOT",
      strPtr(opGetRootFolder(options)));

  if (fsIsDir(strPtr(fsnGetFolder(fsn))))
    fsnSetDir(fsn, TRUE);
  else
  {
    strPrintf("Not a directory %s\n", strPtr(fsnGetFolder(fsn)));
    exit(EXIT_FAILURE);
  }

  fsScanDir(fsn);
  List * volumes = listCreate(0);
  logicCreatePackages(fsn, volumes, options);

#ifdef DEBUG_ON
  strPrintf("Scan root:       %s\n", strPtr(opGetRootFolder(options)));
  if (opGetDestFolder(options))
    strPrintf("Dest dir:        %s\n", strPtr(opGetDestFolder(options)));
  strPrintf("Package depth:   %d\n", opGetDepth(options));
  strPrintf("Files:           %d\n", fsnCountAllFiles(fsn));
  strPrintf("Size:            %d mb\n", (fsnCountAllFileSizes(fsn)/1024/1024));
  strPrintf("Volume capacity: %d mb\n", (opGetCapacity(options)/1024/1024));
  strPrintf("Volume count:    %d\n", listSize(volumes));
  strPrintf("Wasted:          %d mb %d%\n",
      (int)(logicCountWastedSpace(volumes)/1024/1024),
      (int)(logicCountWastedSpace(volumes)/((opGetCapacity(options)*listSize(volumes))/100)));
#endif
  Progress * progress = progressCreate(
      0,
      fsnCountAllFileSizes(fsn),
      fsnCountAllFiles(fsn),
      !opIsQuiet(options));

  if (opGetDestFolder(options) != NULL)
    logicCopyVolumes(volumes, progress, options);
  else
    logicDumpVolumes(volumes);

  (* options->pVTable[0].pFunc) (TRUE, options);
  (* volumes->pVTable[0].pFunc) (TRUE, volumes);
  (* progress->pVTable[0].pFunc) (TRUE, progress);
  (* fsn->pVTable[0].pFunc) (TRUE, fsn);

  return EXIT_SUCCESS;
}
