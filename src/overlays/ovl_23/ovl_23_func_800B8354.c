#include "common.h"
#include "game_types.h"
#include "random.h"

void ovl_23_func_800B8354(void) {
    s32 i;
    s32 pairIndex;
    u8 *base;
    u8 *rec2;
    u8 *work;
    u8 *rec4;
    u16 *src;

    i = 0;
    base = Ovl23_D_800BF87C_draw;
    rec2 = base + 0xA0;
    pairIndex = 0;
    for (; i < 6; i++) {
        Ovl23D87CEntry50 *pr = &((Ovl23D87CView50 *)base)->unk0[i];

        work = D_80074838;

        pr->unk24 = i;
        pr->unk26 = 1;
        pr->unk28 = 2;
        pr->unk2A = 2;
        pr->unk2C = 0;
        ((Ovl23D87CView50 *)base)->unk0[i].unk38 = 0xFFFE5000;
        ((Ovl23D87CView50 *)base)->unk0[i].unk3C = ((s16 *)&gOvl23RecordAnchorY)[i] << 12;
        ((struct Ovl23ScreenInitTail *)pr)->unk70 = D_800BBA64[pairIndex];
        ((struct Ovl23ScreenInitTail *)pr)->unk72 = D_800BBA64[pairIndex + 1];
        if (i == 4) {
            rec4 = base + 0x140;
            *(u16 *)(rec4 + 0x30) = 0xFF;
            *(u16 *)(rec4 + 0x32) = 0xFF;
        } else {
            if (i != 2) {
                pr->unk30 = Rand(0x60) + 0xA0;
                pr->unk32 = Rand(0x9C) + 0x64;
            } else {
                *(u16 *)(rec2 + 0x30) = *(u16 *)(work + 0x11C2);
                *(u16 *)(rec2 + 0x32) = *(u16 *)(work + 0x1258);
            }
        }
        pr->unk2E = D_800BBA90[(s16)pr->unk32 / 50];
        ((Ovl23D87CView50 *)base)->unk0[i].unk34 =
            (s16)*(u16 *)(base + 0x3B0) * ((s16)pr->unk30 + 0x80) << 4;
        pairIndex += 2;
    }
}
