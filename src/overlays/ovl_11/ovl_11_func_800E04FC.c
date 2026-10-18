#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800E0D3C(UnkStruct800E109C *arg0, s32 arg1);

UnkStruct800E109C *ovl_11_func_800E04FC(s32 arg0, s32 arg1) {
    UnkStruct800E109C *temp_s0;
    void *temp_v1;

    temp_s0 = (UnkStruct800E109C *) ((u8 *) &D_800749F4 + (arg1 * 0xB8));
    ovl_11_func_800E0D3C(temp_s0, 0x164);
    temp_v1 = (void *) ((u8 *) &D_800749F4 + (arg0 * 0xB8));
    *(s32 *) ((u8 *) temp_s0 + 0x38) = *(s32 *) ((u8 *) temp_v1 + 0x38);
    *(s32 *) ((u8 *) temp_s0 + 0x3C) = *(s32 *) ((u8 *) temp_v1 + 0x3C);
    *(s32 *) ((u8 *) temp_s0 + 0x40) = *(s32 *) ((u8 *) temp_v1 + 0x40) - 0xC8;
    *(u16 *) ((u8 *) temp_s0 + 0x30) = *(u16 *) ((u8 *) temp_v1 + 0x30);
    *(u16 *) ((u8 *) temp_s0 + 0x22) = *(u16 *) ((u8 *) temp_v1 + 0x22);
    return temp_s0;
}
