#include "common.h"
#include "psyq/memory.h"

void SetGfxClip(s32 arg0, s32 arg1);

void SetGfxOffset(s32 arg0, s32 arg1);

void func_80013A24(void) {
    memset(D_8005E3A8, 0, 0x38);
    memset(D_8005E3AC, 0, 0x38);
    memset(&D_8005E9C8, 0, 0x44);
    memset(&D_8005EA18, 0, 0x10);
    SetGfxOffset(0x14, 7);
    SetGfxClip(0x14, 7);
}
