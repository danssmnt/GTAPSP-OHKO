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

#include "patch.h"

int PPSSPP = 0;
int LCS = 0;
int VCS = 0;

int PatchLCS(u32 addr, u32 text_addr) {
    if( _lw(addr + 0x8) == 0x000429C0 && _lw(addr + 0xC) == 0x00A53021 && _lw(addr + 0x18) == 0x00A42023 && _lw(addr + 0x2C) == 0x8C820000 ) {  //0x1d18b0
        GetPPLAYER = (void*)(addr); // Get pplayer addr
        return 1;
    }
    if( _lw(addr - 0x70) == 0x3C043586  && _lw(addr + 0x18) == 0x00402025  && _lw(addr + 0x64) == 0x34040001 ) {
        MAKE_CALL(addr, sceKernelGetSystemTimeWidePatched); // Loop
        return 1;
    }

    return 0;  
}

int PatchVCS(u32 addr, u32 text_addr) {
    if( _lw(addr + 0x8) == 0x00043200 && _lw(addr + 0x18) == 0x00C42021 && _lw(addr + 0x28) == 0x8C820000 ) {  // 0x15c424
        GetPPLAYER = (void*)(addr); // Get pplayer addr
        return 1;
    }

    if( _lw(addr - 0xC) == 0x1000FFF7 && _lw(addr + 0x4) == 0x00000000 && _lw(addr + 0x8) == 0x27A60010 && _lw(addr + 0x10) == 0x00602825 && _lw(addr + 0x1C) == 0x8FA40014 ) {
        MAKE_CALL(addr, sceKernelGetSystemTimeWidePatched); // Loop
        return 1;
    }

    return 0;  
}

int patch() {
    u32 i;
    for( i = 0; i < mod_text_size; i += 4 ) {
        u32 addr = mod_text_addr + i;
    
        // first hit decides on version -> yes, potentially dangerous
        if( (LCS == 1 || (LCS == VCS)) && PatchLCS(addr, mod_text_addr) ) {
            LCS = 1;
            continue;
        }
        if( (VCS == 1 || (LCS == VCS)) && PatchVCS(addr, mod_text_addr) ) {
            VCS = 1;
            continue;
        }
    }

    return 0;
}