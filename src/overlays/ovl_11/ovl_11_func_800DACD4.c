#include "common.h"

Ovl11D124Entry *ovl_11_func_800DAF60(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800D5C90(s16 arg0);

void ovl_11_func_800DACD4(s16 arg0, s16 arg1, s32 arg2) {
    Ovl11D124Entry *entry;
    u16 flags;
    s16 texture;

    entry = ovl_11_func_800DAF60(arg2, arg1, arg0);
    if (entry != 0) {
        if ((u16)entry->unk0 == 0x36) {
            entry->unk4 = ovl_11_func_800D5C90(entry->unk0);
            entry->unk5 = 0;
            flags = (u16)entry->unk6 | 0x8000;
            entry->unk6 = flags;
            if (entry->unk4 != 0) {
                texture = (((entry->unk4 - 1) * 8) | 5) + (flags & 15);
                D_80123DFC[arg2](arg1, arg0, texture);
            }
        }
    }
}
