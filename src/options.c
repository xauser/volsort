
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

#include "options.h"
#include <getopt.h>
#include "config.h"
#include "string.h"

VTable VTableOptions[] =
{
  {0, 0, (VirtualFunctionPointer) opKill}
};

static char short_options[] = "s:t:c:d:f:p:qvh";

static struct option long_options[] =
{
  {"source-dir", 1, 0, 's'},
  {"target-dir", 1, 0, 't'},
  {"volume-capacity", 0, 0, 'c'},
  {"package-depth", 1, 0, 'd'},
  {"free-space", 1, 0, 'f'},
  {"volume-prefix", 1, 0, 'p'},
  {"quiet", 1, 0, 'q'},
  {"version", 1, 0, 'v'},
  {"help", 0, 0, 'h'},
  {0, 0, 0, 0}
};

Options * opCreate(Options * this)
{
  if (this == 0)
    this = (Options *) malloc(sizeof(Options));

  if (this)
  {
    objCreate((Object *) this);
    this->pVTable = VTableOptions;
#ifdef DEBUG_OBJECTS_ON
    strPrintf("opCreate()\n");
#endif
    this->rootFolder = 0;
    this->destFolder = 0;
    this->volumePrefix = strCreate(0, VOLSORT_DEFAULT_PREFIX);
    this->volumeCapacity = VOLSORT_DEFAULT_CAPACITY;
    this->packageDepth = VOLSORT_DEFAULT_DEPTH;
    this->extraSpace = VOLSORT_DEFAULT_FREESPACE;
    this->quiet = FALSE;
  }

  return this;
}

void opKill(bool dynamic, Options * this)
{
#ifdef DEBUG_OBJECTS_ON
  strPrintf("~opKill()\n");
#endif
  this->pVTable = VTableOptions;

  strKill(TRUE, this->volumePrefix);

  objKill(FALSE, (Object *) this);

  if (dynamic) free(this);
}

void opParse(Options * this, int argc, char * argv[])
{
  // reset getopt
  optind = 1;

  while (1)
  {
    int option_index  = 0;
    int c = getopt_long(argc, argv, short_options, long_options, &option_index);
    if (c == -1)
      break;

    switch (c)
    {
      case 'c':
        this->volumeCapacity = (uint64_t) strtol(optarg, (char **) NULL, 10);
        this->volumeCapacity *= 1024*1024;
        break;
      case 'f':
        this->extraSpace = (uint64_t) strtol(optarg, (char **) NULL, 10);
        break;
      case 'p':
        strClear(this->volumePrefix);
        strAppendCstr(this->volumePrefix, optarg);
        break;
      case 'd':
        this->packageDepth = atoi(optarg);
        break;
      case 't':
        this->destFolder = strCreate(0, optarg);
        break;
      case 's':
        this->rootFolder = strCreate(0, optarg);
        break;
      case 'q':
        this->quiet = TRUE;
        break;
      case 'v':
        opPrintVersion(this);
        exit(EXIT_SUCCESS);
      default:
        opPrintUsage(this);
        exit(EXIT_SUCCESS);
    }
  }
}

void opPrintUsage(Options * this)
{
  strPrintf("Usage: volsort\n");
  strPrintf("  -s  --source-dir       source directory (mandatory)\n");
  strPrintf("  -t  --target-dir       target directory (this param starts copy)\n");
  strPrintf("  -d  --package-depth    depth in source dir to create packages (default %d)\n", opGetDepth(this));
  strPrintf("  -c  --volume-capacity  capacity of a volume (default %dmb)\n",(opGetCapacity(this)/1024/1024));
  strPrintf("  -f  --free-space       free space at the end of each volume (default %dmb)\n",(opGetExtraSpace(this)/1024/1024));
  strPrintf("  -p  --volume-prefix    prefix for volume name creation (default %s)\n",strPtr(opGetVolumePrefix(this)));
  strPrintf("  -q  --quiet            no verbose is done (default %d, off)\n",opIsQuiet(this));
  strPrintf("  -v  --version          version text of volsort\n");
  strPrintf("  -h  --help             this help\n");
}

String * opGetRootFolder(Options * this)
{
  return this->rootFolder;
}

String * opGetDestFolder(Options * this)
{
  return this->destFolder;
}

uint64_t opGetCapacity(Options * this)
{
  return this->volumeCapacity;
}

uint8_t opGetDepth(Options * this)
{
  return this->packageDepth;
}

uint32_t opGetExtraSpace(Options * this)
{
  return this->extraSpace;
}

String * opGetVolumePrefix(Options * this)
{
  return this->volumePrefix;
}

bool opIsQuiet(Options * this)
{
  return this->quiet;
}

bool opAreComplete(Options * this)
{
  bool valid = TRUE;

  if (this->rootFolder == NULL)
  {
    strPrintf("source folder is not set\n");
    valid = FALSE;
  }

  if (this->volumeCapacity <= 1024*1024)
  {
    strPrintf("capacity must be greater then 1mb\n");
    valid = FALSE;
  }

  if (this->volumeCapacity < this->extraSpace)
  {
    strPrintf("free space is bigger then capacity\n");
    valid = FALSE;
  }

  if (this->packageDepth < 0)
  {
    strPrintf("package depth must be greater or equal 0\n");
    valid = FALSE;
  }

  if (!valid)
    strPrintf("try -h for usage\n");

  return valid;
}

void opPrintVersion(Options * this)
{
  strPrintf("volsort %s (compiled %s)\n", VOLSORT_VERSION, __DATE__);
  exit(EXIT_SUCCESS);
}
