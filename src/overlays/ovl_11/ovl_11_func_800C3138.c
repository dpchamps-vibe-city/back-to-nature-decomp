#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800C35EC(Ovl11AnimationObject *obj);

s32 ovl_11_func_800C3138(u8 *arg0) {
    s32 temp_a1;
    s32 temp_a2;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s2;
    struct_80076220 *var_s0;
    u16 temp_a0;

    var_s0 = &gObjectEventSlotBase;
    D_80128B70 = 0x7FFFFFFF;
    for (var_s2 = 0; var_s2 < 35; var_s2++) {
        temp_a0 = var_s0->unk1E;
        if ((temp_a0 & 0x100) && (*(s16 *) (arg0 + 0xFE) == (s16) var_s0->unk2E) && ((*(s16 *) ((u8 *) var_s0 + 0x12)) == 0) && ((*(u16 *) ((u8 *) var_s0 + 0x1A)) == 2) && !((*(u16 *) ((u8 *) var_s0 + 0x20)) & 0x1000) && !(temp_a0 & 0x1003) && ((s16) var_s0->unkA < 0x1E)) {
            temp_a1 = (*(s32 *) (arg0 + 0x100) - (*(s32 *) ((u8 *) var_s0 + 0x30))) + 1;
            temp_a2 = (*(s32 *) (arg0 + 0x108) - (*(s32 *) ((u8 *) var_s0 + 0x38))) + 1;
            D_80128B6C = temp_a1 * temp_a1;
            D_80128B6C += temp_a2 * temp_a2;
            if (D_80128B6C <= 0xF423FF) {
                temp_v0 = ratan2(temp_a1 * 0xA, temp_a2 * 0xA);
                temp_v1 = D_80122F8C[*(u16 *) (arg0 + 0x38)];
                if (((temp_v0 - 0x258) < temp_v1) && (temp_v1 < (temp_v0 + 0x258))) {
                    ovl_11_func_800C35EC((Ovl11AnimationObject *) var_s0);
                }
            }
        }
        var_s0 += 1;
    }
    return 0;
}
