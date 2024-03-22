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

SceInt64 sceKernelGetSystemTimeWidePatched(void);

int PatchLCS(u32 addr, u32 text_addr);
int PatchVCS(u32 addr, u32 text_addr);
int patch();