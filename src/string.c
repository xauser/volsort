
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

#include "string.h"
#include <stdio.h>
#include <string.h>

VTable VTableString[] =
{
  {0, 0, (VirtualFunctionPointer) strKill}
};

String * strCreate(String * this, char const * data)
{
  if (this == 0)
    this = (String *) malloc(sizeof(String));

  if (this)
  {
    objCreate((Object *) this);
#ifdef DEBUG_OBJECTS_ON
    commonPrintf("strCreate(%s)\n", data);
#endif
    this->data = strdup(data);
  }

  return this;
}

void strKill(bool dynamic, String * this)
{
#ifdef DEBUG_OBJECTS_ON
  commonPrintf("~strKill(%s)\n", strPtr(this));
#endif
  this->pVTable = VTableString;

  free(this->data);
  objKill(FALSE, (Object *) this);

  if (dynamic) free(this);
}

char * strCopy(char * str)
{
  char * sdup;
  size_t malloc_size = strlen(str) + 1;
  sdup = (char *) malloc(malloc_size * sizeof(char));
  memset(sdup, '\0', malloc_size);
  memcpy(sdup, str, strlen(str));
  return sdup;
}

char const * strPtr(String * this)
{
  return (char const *) this->data;
}

size_t strSize(String * this)
{
  return strlen(this->data);
}

void strClear(String * this)
{
  memset(this->data, '\0', strlen(this->data));
}

void strAppendCstr(String * this, char const * append)
{
  size_t realloc_size = strlen(append);
  realloc_size += strlen(this->data);
  realloc_size++;
  this->data = realloc(this->data, realloc_size);
  strcat(this->data, append);
}

void strAppendUint32_t(String * this, uint32_t number)
{
  char tmp[255];
  snprintf(tmp, 255, "%d", number);
  strAppendCstr(this, tmp);
}

void strAppendint32_t(String * this, int32_t number)
{
  char tmp[255];
  snprintf(tmp, 255, "%u", number);
  strAppendCstr(this, tmp);
}
