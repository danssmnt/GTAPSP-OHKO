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

#include <psptypes.h>

u32 memory_low  = 0x08400000; // memory bounds
u32 memory_high = 0x0A000000; // default ~32 MB (with high memory layout -> 0x0C000000) dyanmic overwritten in module_start

int isInMemBounds(int valtocheck) {
  return (valtocheck >= memory_low && valtocheck < memory_high);
}

void setFloat(int adr, float value) { // 32bit
  if( isInMemBounds(adr) && (adr % 4 == 0) ) {
    *(float*)adr = value;
  }
}

float getFloat(int adr) { // 32bit
  if( isInMemBounds(adr) && (adr % 4 == 0) ) {
    return *(float*)adr;
  }
  return 0;
}