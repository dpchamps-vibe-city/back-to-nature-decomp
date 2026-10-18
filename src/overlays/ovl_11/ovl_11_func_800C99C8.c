#include "common.h"
#include "game_types.h"

void ovl_11_func_800CBE14(s32 arg0);
s32 ovl_11_func_800C9888(u16 arg0, u16 arg1);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, u16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C9938(Ovl119938View *arg0);

s32 ovl_11_func_800C99C8(void) {
    Ovl11C7010Object *obj;
    s32 var_s2;
    u16 temp_s3;
    u16 var_s0;

    obj = (Ovl11C7010Object *) &D_80071A00;
    var_s2 = 0;
    var_s0 = *(u16 *) &D_801231F4;
    temp_s3 = obj->unk38;
    if (var_s0 & 0x20) {
        var_s2 = 1;
        if (obj->unk3C == 4) {
            var_s2 = 2;
        }
    }
    var_s0 &= 0xF000;
    ovl_11_func_800CBE14((s32) (s16) obj->unk38);
    obj->unk38 = ovl_11_func_800C9888(var_s0, obj->unk38);
    if ((obj->unk6C & 0x200000) && (var_s2 != 0)) {
        var_s2 = 0;
        var_s0 = 1;
    }
    if (var_s0 == 0) {
        if (var_s2 != 0) {
            if (obj->unk38 != temp_s3 || var_s2 == 1) {
                ovl_11_func_800C70CC(obj, 4U, D_80071A38 + 6, -2);
            }
            obj->unk6C = (s32) (obj->unk6C & 0xDFFFFFFF);
            ovl_11_func_800C9938((Ovl119938View *) &D_80071A00);
            return 1;
        }
        ovl_11_func_800C70CC(obj, 1U, 0, -2);
        obj->unk6C = (s32) ((obj->unk6C & 0xDFFFFFFF) | 0x04000000);
        return 0;
    }
    ovl_11_func_800C9938((Ovl119938View *) &D_80071A00);
    if (var_s2 != 0) {
        if ((obj->unk38 != temp_s3) || (obj->unk3C != 4)) {
            ovl_11_func_800C70CC(obj, 4, ((Ovl11C7010Object *) &D_80071A00)->unk38 + 6, -2);
            obj->unk6C &= 0xDFFFFFFF;
        }
    } else if ((obj->unk38 != temp_s3) || (obj->unk3C != 2)) {
        ovl_11_func_800C70CC(obj, 2, ((Ovl11C7010Object *) &D_80071A00)->unk38 + 1, -2);
        obj->unk6C &= 0xDFFFFFFF;
    }
    return 1;
}
