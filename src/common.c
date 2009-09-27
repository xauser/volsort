
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

#include "common.h"
#include <stdio.h>

void commonPrint(char const * msg, va_list argp)
{
  char tmp[1024];
  vsnprintf(tmp, 1024, msg, argp);
  printf(tmp);
}

void commonPrintf(char const * msg, ...)
{
  va_list ap;
  va_start(ap, msg);
  commonPrint(msg, ap);
  va_end(ap);
}

void commonDebug(uint8_t level, char const * msg, ...)
{
#ifdef DEBUG_ON
  va_list ap;
  va_start(ap, msg);
  commonPrint(msg, ap);
  va_end(ap);
#endif
}
