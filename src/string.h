
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

#ifndef STRING_H
#define STRING_H

#include <stdarg.h>
#include "object.h"

typedef struct string String;

struct string
{
  VTable * pVTable;

  char * data;
};

void pure_virtual_called_error_handler();

void strKill(bool dynamic, String * this);

String * strCreate(String * this, char const * data);

char const * strPtr(String * this);

void strAppendCstr(String * this, char const * append);

void strAppendUint32_t(String * this, uint32_t number);

void strAppendint32_t(String * this, int32_t number);

size_t strSize(String * this);

char * strCopy(char * str);

void strClear(String * this);

void commonPrint(char const * msg, va_list argp);

void strPrintf(char const * msg, ...);

#endif
