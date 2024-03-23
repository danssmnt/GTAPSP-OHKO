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
#include <systemctrl.h>

#define MAKE_CALL(a, f) _sw(0x0C000000 | (((u32)(f) >> 2) & 0x03FFFFFF), a);

u32 mod_text_addr;
u32 mod_text_size;
u32 mod_data_size;
extern int PPSSPP;
extern int LCS;
extern int VCS;
static STMOD_HANDLER previous;

int (* GetPPLAYER)();

int sceKernelSysClock2USecWidePatched(SceInt64 clock, unsigned *low, unsigned int *high);

int PatchLCS(u32 addr, u32 text_addr);
int PatchVCS(u32 addr, u32 text_addr);
int patch();