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

PSP_MODULE_INFO("GTA OHKO", 0, 1, 0); // user

int pplayer = 0;

float pplayer_health;
float pplayer_armor;

SceInt64 sceKernelGetSystemTimeWidePatched(void) { // LCS & VCS
    pplayer = GetPPLAYER();
    if (pplayer) {
        pplayer_health = getFloat(pplayer + (LCS ? 0x4b8 : 0x4E4));
        pplayer_armor = getFloat(pplayer + (LCS ? 0x4bc : 0x4E8));
        if (pplayer_health > 1.1f) {
            setFloat(pplayer+(LCS ? 0x4b8 : 0x4E4), 1.1f);
        }
        if (pplayer_armor > 0.0f) {
            setFloat(pplayer+(LCS ? 0x4bc : 0x4E8), 0.0f);
        }

    }

    return sceKernelGetSystemTimeWide();
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
    
        /// with this approach the game continues to run when patch() is still in progress
      
        sceKernelDelayThread(1000 * 100); // small delay to fix blackscreen for LCS (mostly slow android devices)
    
        int ret = patch();
        if( ret != 0 ) // patching returned error
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
    if( ret != 0 ) // patching returned error
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
        CheckModules(); // scan the modules using normal/official syscalls (https://github.com/hrydgard/ppsspp/pull/13335#issuecomment-689026242)
    
    else // PSP
        previous = sctrlHENSetStartModuleHandler(OnModuleStart); 
    return 0;
}