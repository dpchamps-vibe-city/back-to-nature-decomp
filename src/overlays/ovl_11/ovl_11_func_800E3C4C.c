#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libetc.h"
#include "psyq/libcd.h"
#include "psyq/memory.h"
#include "cd_loader.h"


int CdControl (u_char com, u_char *param, u_char *result);
CdlLOC *CdIntToPos (int i, CdlLOC *p);
int CdRead (int sectors, u_long *buf, int mode);
int CdReadSync (int mode, u_char *result);
int CdSync (int mode, u_char *result);
int ResetCallback (void);
void *memmove (unsigned char *, const unsigned char *, int);
s32 func_8001AF44 (u32 arg0);
void ovl_11_func_800E3CDC (u8 *arg0);
s32 ovl_11_func_800E351C (u8 *arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_800E3C4C(void) {
    s32 temp_s0;

    if (func_8001AF44(2U) != 1) {
        if (D_8006C844 & 0x18000) {
            return 0;
        }
        DrawSync(0);
        ClearOTagR((u32 *) gActiveRenderContext->field_120, 0x800);
        temp_s0 = D_8005E70C;
        CdLoadArchiveRangeBlocking(0x1D, 0x1E, temp_s0, temp_s0);
        ovl_11_func_800E3CDC((u8 *) temp_s0);
    }
    return 0;
}
