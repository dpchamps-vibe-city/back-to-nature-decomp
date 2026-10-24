#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800CB114(Ovl11CB114Object *, s32 *, s16);
s32 ovl_11_func_800BDB88(s32 *, s32);
/* Overlay name for the matched GetVal8005E524 word-returning getter. */
s32 GetVal8005E524(void);

void ovl_11_func_800C498C(Ovl11C498CObject *arg0) {
    s32 position[4];
    s16 *offset;

    ovl_11_func_800CB114((Ovl11CB114Object *)arg0, arg0->position, 0x18);
    position[0] = arg0->position[0];
    position[1] = arg0->position[1];
    position[2] = arg0->position[2];
    position[3] = arg0->position[3];
    offset = D_80123110[arg0->direction];
    position[0] += offset[0];
    position[2] += offset[1];
    ovl_11_func_800BDB88(position, 0);
    D_80128C50 = *(s32 *)GetVal8005E524();
    if (!(D_80128C50 & 0xFFFF0000)) {
        D_80128C50 = 0x8000;
    }
}
