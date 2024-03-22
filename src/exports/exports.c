/*
 *  GTA PSP OHKO
 *  Copyright (C) 2024, danssmnt
 *  
 *  This program is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *  
 *  This program is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
 *  GNU General Public License for more details.
 *  
 *  You should have received a copy of the GNU General Public License
 *  along with this program. If not, see <http://www.gnu.org/licenses/>.
 */

#include <pspmoduleexport.h>
#define NULL ((void *) 0)

extern void module_start;
extern void module_info;
static const unsigned int __syslib_exports[4] __attribute__((section(".rodata.sceResident"))) = {
  0xD632ACDB,
  0xF01D73A7,
  (unsigned int) &module_start,
  (unsigned int) &module_info,
};

const struct _PspLibraryEntry __library_exports[1] __attribute__((section(".lib.ent"), used)) = {
  { NULL, 0x0000, 0x8000, 4, 1, 1, &__syslib_exports },
};
