#include "common.h"
#include "game_types.h"

s32 ovl_11_func_8010D20C(u16 arg0, u16 arg1, u16 *arg2, u16 *arg3);
void func_80015840();
void func_8001585C();
void ovl_11_func_801110CC(M2C_ececd3ff98bf_Struct_801110CC *arg0);
void ovl_11_func_800D001C(u8 *arg0);

void ovl_11_func_8010CA0C(M2C_ececd3ff98bf_Struct_801110CC *arg0) {
    u16 sp10;
    u16 sp12;
    u16 temp_v1;

    ovl_11_func_8010D20C(*(u16 *) ((u8 *) arg0 + 0x24), *(u16 *) ((u8 *) arg0 + 0x22), &sp10, &sp12);
    if (sp10 != (*(u8 *) ((u8 *) arg0 + 0x7C))) {
        func_80015840((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp10);
    }
    temp_v1 = *(u16 *) ((u8 *) arg0 + 0x24);
    if (((temp_v1 == 0) || (temp_v1 == 0xF)) && (sp12 != (*(u8 *) ((u8 *) arg0 + 0x7D)))) {
        func_8001585C((ObjectState *) ((u8 *) arg0 + 0x78), (u8) sp12);
    }
    ovl_11_func_801110CC(arg0);
    ovl_11_func_800D001C((u8 *) arg0);
}
