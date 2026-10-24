#include "common.h"
#include "game_types.h"

/* Legacy overlay import of matched Rand at 0x80012A34. */
u32 Rand(s32 arg0);
s32 ovl_21_func_800BA7F0(s16 arg0);
s16 ovl_21_func_800BA868(s16 arg0);
s16 ovl_21_func_800BA944(s16 arg0);

void ovl_21_func_800B9A20(void) {
    s32 i;
    u8 *blk;
    u8 *first;
    u8 *base;

    for (i = 1; i < 3; i++) {
        switch (ovl_21_func_800BA7F0(1)) {
        case 3:
            ((UnkStruct800C0448 *)D_800C0448)[i].unk2C = Rand(3) + 3;
            break;
        case 2:
            first = (u8 *)D_800C0448;
            if ((s32)Rand(100) < *(s16 *)(first + 0x676)) {
                ((UnkStruct800C0448 *)first)[i].unk2C = ovl_21_func_800BA944(1);
            } else {
                ((UnkStruct800C0448 *)first)[i].unk2C = ovl_21_func_800BA868(1);
            }
            break;
        default:
            ((UnkStruct800C0448 *)D_800C0448)[i].unk2C = ovl_21_func_800BA868(1);
            break;
        }
    }
    for (i = 3; i < 6; i++) {
        switch (ovl_21_func_800BA7F0(0)) {
        case 3:
            base = (u8 *)D_800C0448;
            if ((s32)Rand(100) < *(s16 *)(base + 0x67E) -
                (*(s16 *)(base + 0x680) * ((UnkStruct800C0448 *)base)[i].unk22) / 255) {
                ((UnkStruct800C0448 *)base)[i].unk2C = 0;
            } else {
                ((UnkStruct800C0448 *)base)[i].unk2C = ovl_21_func_800BA868(2);
            }
            break;
        case 2:
            blk = (u8 *)D_800C0448;
            if (*(s32 *)(blk + 0x14) == 1) {
                if ((s32)Rand(100) < *(s16 *)(blk + 0x678) -
                    (*(s16 *)(blk + 0x67A) * ((UnkStruct800C0448 *)blk)[i].unk22) / 255) {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = 0;
                } else if ((s32)Rand(100) < *(s16 *)(blk + 0x67C)) {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = ovl_21_func_800BA944(0);
                } else {
                    ((UnkStruct800C0448 *)blk)[i].unk2C = ovl_21_func_800BA868(2);
                }
            } else {
                ((UnkStruct800C0448 *)blk)[i].unk2C = ovl_21_func_800BA868(0);
            }
            break;
        default:
            ((UnkStruct800C0448 *)D_800C0448)[i].unk2C = ovl_21_func_800BA868(0);
            break;
        }
    }
}
