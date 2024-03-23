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
#include <pspsdk.h>
#include <pspkernel.h>
#include <pspctrl.h>
#include <pspdisplay.h>
#include <systemctrl.h>
#include <stdio.h>
#include <string.h>
#include <time.h>
#include "utils/patch.h"
#include "utils/utils.h"
#include "main.h"

PSP_MODULE_INFO("GTA OHKO", 0, VERSION, VERSION_MIN);

#define HEALTH_OFFSET_LCS 0x4B8 // LCS Health Offset (pplayer)
#define HEALTH_OFFSET_VCS 0x4E4 // VCS Health Offset (pplayer)
#define ARMOR_OFFSET_LCS 0x4BC  // LCS Armor Offset (pplayer)
#define ARMOR_OFFSET_VCS 0x4E8  // VCS Armor Offset (pplayer)

int pplayer; // pplayer pointer
float pplayer_health; // Health
float pplayer_armor; // Armor

int sceKernelSysClock2USecWidePatched(SceInt64 clock, unsigned *low, unsigned int *high) { // LCS & VCS
    pplayer = GetPPLAYER(); // Get pplayer address
    if (!pplayer) // Break if pplayer not found
        goto BREAK;
  
    pplayer_health = getFloat(pplayer + (LCS ? HEALTH_OFFSET_LCS : HEALTH_OFFSET_VCS)); // Get Health
    pplayer_armor = getFloat(pplayer + (LCS ? ARMOR_OFFSET_LCS : ARMOR_OFFSET_VCS)); // Get Armor

    if (pplayer_health > 1.1f) // If health > 1.1f, set to 1.1f (less than 1.1f health = player dies)
        setFloat(pplayer+(LCS ? HEALTH_OFFSET_LCS : HEALTH_OFFSET_VCS), 1.1f);

    if (pplayer_armor > 0.0f) // No armor :)
        setFloat(pplayer+(LCS ? ARMOR_OFFSET_LCS : ARMOR_OFFSET_VCS), 0.0f);
    
    BREAK:
    return sceKernelSysClock2USecWide(clock, low, high);
}


static void CheckModules() { // PPSSPP only
    SceUID modules[10];
    int count = 0;
    if( sceKernelGetModuleIdList(modules, sizeof(modules), &count) >= 0 ) {
        int i;
        SceKernelModuleInfo info;
        for( i = 0; i < count; ++i ) {
            info.size = sizeof(SceKernelModuleInfo);
            if (sceKernelQueryModuleInfo(modules[i], &info) < 0) {
                continue;
            }
            if (strcmp(info.name, "GTA3") == 0) {
                mod_text_addr = info.text_addr;
                mod_text_size = info.text_size;
                mod_data_size = info.data_size;

                sceKernelDelayThread(1000 * 100); // Delay to fix crashes
                int ret = patch();
                if( ret != 0 ) // Patching returned error
                    return;

                sceKernelDcacheWritebackAll();
                return;
            }
        }
    }
}   

int OnModuleStart(SceModule2 *mod) {
  char *modname = mod->modname;
  
  if( strcmp(modname, "GTA3") == 0 ) {
    mod_text_addr = mod->text_addr;
    mod_text_size = mod->text_size;
    mod_data_size = mod->data_size;

    int ret = patch();
    if( ret != 0 ) // Patching returned error
      return -1;
    sceKernelDcacheWritebackAll();
  }

  if( !previous )
    return 0;

  return previous(mod);
}

int module_start(SceSize argc, void* argp) {

    if(sceIoDevctl("kemulator:", 0x00000003, NULL, 0, NULL, 0) == 0)
        PPSSPP = 1;

    if( PPSSPP ) 
        CheckModules(); // Scan the modules using normal/official syscalls (https://github.com/hrydgard/ppsspp/pull/13335#issuecomment-689026242)
    
    else // PSP
        previous = sctrlHENSetStartModuleHandler(OnModuleStart); 
    return 0;
}