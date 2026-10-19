#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_80109188(Ovl11D04D4Obj *arg0, s32 arg1);
s32 ovl_11_func_800CEFCC(Ovl11CEFCCObject *arg0, Ovl11CollisionTag *arg1, VECTOR *arg2);
s32 ovl_11_func_800D3104(u16 *arg0, s32 arg1);
s32 ovl_11_func_800CF258(s32 arg0);
void ovl_11_func_800D3200(s32 arg0);

s32 ovl_11_func_800CF044(s16 arg0, Ovl25SetupStatePair *arg1, VECTOR *arg2) {
    Ovl11CEFCCObject *var_s0;
    s32 *var_a0;
    s32 var_s2;
    s32 var_v1;
    s32 var_v1_2;
    u16 temp_v1;

    if ((u32) (arg0 - 0x109) < 2U) {
        var_a0 = &D_800742EC;
        for (var_v1 = 0; var_v1 < 10; var_v1++, var_a0 += 0x2D) {
            if ((*(s32 *) ((u8 *) var_a0 + 0x34)) & 0x20000) {
                goto block_14;
            }
        }
        return 1;
    }
    if ((u32) (arg0 - 0x161) < 2U) {
        var_a0 = &D_800749F4;
        for (var_v1_2 = 0; var_v1_2 < 20; var_v1_2++, var_a0 += 0x2E) {
            if ((*(u16 *) ((u8 *) var_a0 + 0)) == arg0 &&
                ((*(s32 *) ((u8 *) var_a0 + 0x34)) & 0x20000)) {
                goto block_14;
            }
        }
        return 1;
    }
    if ((u32) (arg0 - 0x106) < 2U) {
        if (D_800759E4.f34 & 0x20000) {
            ovl_11_func_80109188((Ovl11D04D4Obj *) &D_800759E4, 9);
            var_a0 = (s32 *) &D_800759E4;
block_14:
            return ovl_11_func_800CEFCC((Ovl11CEFCCObject *) var_a0, arg1, arg2);
        }
        /* Duplicate return node #28. Try simplifying control flow for better match */
        return 1;
    }
    var_s0 = (Ovl11CEFCCObject *) D_80075BC4_reset;
    var_s2 = 0;
    do {
        if (((*(u16 *) ((u8 *) var_s0 + 0)) == arg0) && (var_s0->flags & 0x20000)) {
            (*(s32 *) ((u8 *) var_s0 + 0x8C)) = 0;
            temp_v1 = *(u16 *) ((u8 *) var_s0 + 0);
            switch (temp_v1) {
            case 0x118:
            case 0x119:
            case 0x11A:
                ovl_11_func_800D3104((u16 *) var_s0, 0x10);
                (*(u16 *) ((u8 *) var_s0 + 0x22)) = (u16) D_80071A38;
                break;
            case 0x114:
            case 0x115:
                ovl_11_func_800D3104((u16 *) var_s0, 0xF);
                (*(u16 *) ((u8 *) var_s0 + 0x22)) = (u16) D_80071A38;
                break;
            case 0x116:
            case 0x117:
                ovl_11_func_800D3104((u16 *) var_s0, 0xE);
                (*(u16 *) ((u8 *) var_s0 + 0x22)) = (u16) D_80071A38;
                break;
            }
            ovl_11_func_800CEFCC(var_s0, arg1, arg2);
            if (ovl_11_func_800CF258((s32) (*(u16 *) ((u8 *) var_s0 + 0))) == 0) {
                ovl_11_func_800D3200((s32) var_s0);
            }
        }
        var_s2 += 1;
        var_s0 = (Ovl11CEFCCObject *) ((u8 *) var_s0 + 0xB0);
    } while (var_s2 < 6);
    return 1;
}
