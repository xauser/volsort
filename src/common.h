
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

#ifndef COMMON_H
#define COMMON_H

#include <stdarg.h>
#include <inttypes.h>

//#define DEBUG_ON
//#define DEBUG_FILESYSTEM_ON
//#define DEBUG_LOGIC_ON

void commonPrint(char const * msg, va_list argp);

void commonPrintf(char const * msg, ...);

void commonDebug(uint8_t level, char const * msg, ...);

#endif
