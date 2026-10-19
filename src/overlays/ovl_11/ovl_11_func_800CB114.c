#define OVL11_COLLISION_OUTPUT_VIEW
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800BE208(s32 *, Ovl11CollisionTag, u16, u16, void **, s32 *, s32, s32);
void ovl_11_func_800C70CC(Ovl11C7010Object *, u16, s32, s32);
s32 ovl_11_func_800DF010(void *, s32);
s32 ovl_11_func_800E0C24(void *, s32);
s32 ovl_11_func_8010B57C(u16 *, s32);

s32 ovl_11_func_800CB114(Ovl11CB114Object *arg0, s32 *arg1, s16 arg2) {
    s32 position[4];
    void *sp30;
    s32 sp34;
    s32 var_s3;
    s32 temp_a2;
    s32 temp_v1_2;
    s32 temp_v1_3;
    s32 var_a2;
    s32 var_t2;
    s16 *temp_v1;

    sp34 = 0;
    sp30 = NULL;
    position[1] = arg1[1];
    temp_v1 = &Ovl11CollisionOffsets[arg2 + arg0->direction * 2];
    position[0] = arg1[0] + temp_v1[0];
    position[2] = arg1[2] + temp_v1[1];
    temp_a2 = arg0->flags;
    arg0->flags = (s32) (temp_a2 & 0xFFFBFFFF);
    var_t2 = 0x8FAE;
    if (temp_a2 & 0x100) {
        var_t2 = 0x8DAE;
    }
    if (D_801231F4 & 0x20) {
        var_t2 &= ~0x80;
    }
    var_s3 = ovl_11_func_800BE208(&position[0], arg0->collision, 0x3E8U, 0x3E8U, &sp30, &sp34, 0, var_t2) * 2;
    Ovl11CollisionFlags = 0;
    D_80128C4C = NULL;
    if ((arg2 != 0) && (arg2 != 0x38)) {
        if (sp34 & 0x10000) {
            temp_v1_2 = arg0->flags;
            var_a2 = 1;
            if (!(temp_v1_2 & 0x20000010)) {
                if (!(temp_v1_2 & 0x18000000)) {
                    var_a2 = 2;
                }
            } else {
                var_a2 = 0;
            }
            if (arg0->mode == 4 || arg0->mode == 2) {
                var_a2 = 2;
            }
            if (arg0->mode == 0xE || arg0->mode == 6) {
                var_a2 = 0;
            }
            if (var_a2 != 0) {
                if (var_a2 == 2) {
                    if (!(arg0->flags & 0x200000)) {
                        ovl_11_func_800C70CC((Ovl11C7010Object *) &D_80071A00, 1U, 0, -2);
                    }
                    arg0->flags = (s32) (arg0->flags | 0x200000);
                }
                if (sp30 != NULL) {
                    if (sp34 & 2) {
                        if ((u32) D_80123158 >= 0x33U) {
                            ovl_11_func_800DF010(sp30, 0xD);
                        } else {
                            D_80123158 += 1;
                        }
                    }
                    if (sp34 & 4) {
                        if ((u32) D_80123158 >= 0x33U) {
                            ovl_11_func_800E0C24(sp30, 0xD);
                        } else {
                            D_80123158 += 1;
                        }
                    }
                    if (sp34 & 0x200) {
                        if ((u32) D_80123158 >= 0x33U) {
                            ovl_11_func_8010B57C((u16 *) sp30, 0xD);
                        } else {
                            D_80123158 += 1;
                        }
                    }
                }
            } else {
                sp34 |= 0x4000;
            }
        } else {
            temp_v1_3 = arg0->flags;
            if (temp_v1_3 & 0x200000) {
                arg0->flags = (s32) ((temp_v1_3 & 0xFFDFFFFF) | 0x20000);
            }
        }
        if (sp34 & 0x2000) {
            arg0->flags = (s32) (arg0->flags | 0x40000);
            Ovl11CollisionFlags = sp34;
            D_80128C4C = sp30;
        }
        Ovl11CollisionFlags = sp34;
        D_80128C4C = sp30;
    }
    if (sp34 & 0x4000) {
        var_s3 = 4;
        if (arg2 == 0x38) {
            var_s3 = 0x3E7;
            Ovl11CollisionFlags = sp34;
            D_80128C4C = sp30;
        }
    }
    if (arg0->flags & 8) {
        return 0;
    }
    if (arg0->flags & 0x80) {
        arg0->flags &= ~0x220000;
        if (arg0->mode == 0x39) {
            return 0;
        }
        if (arg0->mode == 0x3A) {
            return 0;
        }
    }
    return (s16)var_s3;
}
