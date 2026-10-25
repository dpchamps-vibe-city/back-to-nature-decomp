#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "projected_primitives.h"

void ovl_11_func_800DCE98(s32 arg0);

Ovl11FuncDCF10Entry *ovl_11_func_800DCF10(void *arg0, void *arg1, void *arg2, u8 *arg3) {
    Ovl11FuncDCF10Entry *p;
    s32 i;

    p = (Ovl11FuncDCF10Entry *)D_801290D8;
    for (i = 0; i < 3; i++, p++) {
        if (p->unk2C & 0x8000) {
            ovl_11_func_800DCE98((s32)p);
            p->unk2C &= 0x7FFF;
            InitProjectedPrimitiveSource(p, arg0);
            InitProjectedPrimitiveSource((u8 *)p + 8, arg1);
            InitProjectedPrimitiveSource((u8 *)p + 0x10, arg2);
            p->unk18 = *(u16 *)(arg3 + 0);
            p->unk1A = *(u16 *)(arg3 + 2);
            p->unk1C = *(u16 *)(arg3 + 4);
            return p;
        }
    }
    return 0;
}
