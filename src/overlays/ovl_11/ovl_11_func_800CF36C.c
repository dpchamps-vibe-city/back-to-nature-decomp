#include "common.h"
#include "game_types.h"
#include "ovl11_object_record_clear.h"


void ovl_11_func_800CF36C(s32 arg0) {
    UnkStruct80075BC4 *ptr;
    s32 mask;
    char *base;
    s32 i;
    s32 flagMask;

    ptr = gStateObjectPool;
    mask = 0x20000;
    base = (char *)&D_8006C838;
    i = 5;
    flagMask = 0x01000000;

    do {
        if ((!(*(s32 *)((u8 *)ptr + 0x34) & mask) || (arg0 != 0)) && (!(*(u16 *)((u8 *)ptr + 0x00) & 0x15B) || !(*(s32 *)(base + 0x44F8) & flagMask))) {
            Ovl11ClearB0ObjectRecord((s32)ptr);
        }
        i -= 1;
        ptr += 1;
    } while (i >= 0);
}
