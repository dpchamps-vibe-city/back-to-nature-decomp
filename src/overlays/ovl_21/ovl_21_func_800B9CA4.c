#include "common.h"
#include "game_types.h"
#include "random.h"

s32 ovl_21_func_800BA7F0(s16 arg0);
s32 *ovl_21_func_800BA698(Ovl21DistanceOrigin *arg0, s16 arg1, s16 arg2);
s32 ovl_21_func_800B98CC(Ovl21Func800B98CCArg *arg0);

void ovl_21_func_800B9CA4(void) {
    s32 i;
    s32 nearSide;
    s32 farSide;
    UnkStruct800C0448 *base;
    u8 *raw;
    u8 *pos;

    for (i = 1; i < 6; i++) {
        base = (UnkStruct800C0448 *)gOvl21SceneControl;
        raw = (u8 *)base;
        pos = raw + 0x1A;
        nearSide = i < 3;
        farSide = i >= 3;
        if (D_800C0448_init.records[i].unk14 == 1) {
            if (ovl_21_func_800BA7F0(farSide) >= 2) {
                if (ovl_21_func_800BA698((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                    if (base[i].unk18 == 6) {
                        if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x672)) {
                            base[i].unk18 = 2;
                        }
                    } else {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66E)) {
                            base[i].unk18 = 6;
                        } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x670)) {
                            base[i].unk18 = 2;
                        }
                    }
                } else {
                    if (base[i].unk18 == 6) {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66C)) {
                            base[i].unk18 = 2;
                        }
                    } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x66A)) {
                        ovl_21_func_800B98CC((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                    }
                }
            } else {
                if (base[i].unk18 == 6) {
                    if (ovl_21_func_800BA698((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                        if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x668)) {
                            base[i].unk18 = 2;
                        }
                    } else if ((s32)Rand(0x80) > *(s16 *)(raw + 0x666)) {
                        base[i].unk18 = 2;
                    }
                } else {
                    if (ovl_21_func_800BA698((Ovl21DistanceOrigin *)(pos + i * 0x108), *(s16 *)(raw + 0x65A), nearSide)) {
                        if ((s32)Rand(0x80) > *(s16 *)(raw + 0x662)) {
                            base[i].unk18 = 6;
                        } else if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x664)) {
                            ovl_21_func_800B98CC((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                        }
                    } else if ((s32)Rand(0x80) <= *(s16 *)(raw + 0x674)) {
                        ovl_21_func_800B98CC((Ovl21Func800B98CCArg *)&D_800C0448_init.records[i].unk10);
                    }
                }
            }
        }
    }
}
