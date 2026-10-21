#include "common.h"

void ovl_11_func_800DD8AC(void) {
    s32 i;
    Ovl11FuncD8ACEntry *p;

    D_80129198 = 0;
    for (i = 0, p = (Ovl11FuncD8ACEntry *)D_80128E08; i < 0xF; i++, p++) {
        if (p->unk28 & 0x40) {
            p->unk10 = 0;
            p->unk12 = 0;
            p->unk14 = 0;
            D_80129194 = p;
            return;
        }
    }
    D_80129194 = 0;
}
