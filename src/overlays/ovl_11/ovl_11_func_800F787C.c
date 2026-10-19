#include "common.h"
#include "game_types.h"

void func_80017A38(s16 arg0, s16 arg1);
s32 func_800226B0(void);
void func_80022738(void);
s32 func_8001FABC(s16 arg0);
s32 func_8001F250(s32 arg0, s32 arg1);
void func_800226D8(s32 arg0);
void func_80017B18(s32 arg0);
s32 func_8002261C(s32 arg0, s32 arg1);
void ovl_11_func_800F8C1C(u32 *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_11_func_800F8B4C(s32 arg0, s32 arg1);
s32 ovl_11_func_800F7AA0(void);
void ovl_11_func_800F889C(s32 *arg0, s32 arg1, s16 *arg2);
void ovl_11_func_800F882C(s32 *arg0, s32 arg1, s32 arg2, s16 *arg3);
void ovl_11_func_800F84F0(s32 arg0);
void ovl_11_func_800F87AC(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void ovl_11_func_800F787C(void) {
    s32 firstText;
    s32 secondText;
    s32 message;
    s32 items;
    s32 selected;
    s32 snapshot;
    s32 object;
    s32 text;
    u32 *panel;
    s32 *display;

    func_80017A38(0, 2);
    if (func_800226B0() && (*(s32 *)((u8 *)D_8005E3A8 + 8) & 0x800)) {
        D_80126E40 = 0xFE;
        func_80022738();
        func_8001FABC(3);
        return;
    }
    firstText = D_80126E40;
    if (firstText == 0) {
        firstText = 0x448;
        secondText = 0x466;
        message = 0x3AD;
        items = (s32)D_8006C838 + 0x468A;
        selected = (s32)D_8006C838 + 0x524C;
        snapshot = (s32)D_8006C838 + 0x5258;
    } else if (firstText == 2) {
        firstText = 0x454;
        secondText = 0x476;
        message = 0x3AE;
        items = (s32)D_8006C838 + 0x450A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else if (firstText == 1) {
        firstText = 0x454;
        secondText = 0x484;
        message = 0x3AF;
        items = (s32)D_8006C838 + 0x480A;
        selected = (s32)D_8006C838 + 0x5252;
        snapshot = (s32)D_8006C838 + 0x5288;
    } else {
        return;
    }
    firstText = (s32)((u8 *)D_8005175C + (D_80054BBC[0] + firstText));
    secondText = (s32)((u8 *)D_8005175C + (D_80054BBC[0] + secondText));
    object = D_8005E3C0->field_D8;
    panel = (u32 *)(object + 0x68);
    text = object + 0x64;
    display = (s32 *)(object + 0x5C);
    ovl_11_func_800F8C1C(panel, text, firstText, secondText);
    ovl_11_func_800F8B4C(text, 0x40);
    ovl_11_func_800F7AA0();
    ovl_11_func_800F889C(display, 0, (s16 *)selected);
    ovl_11_func_800F882C(display, 1, func_8001F250(2, D_80071A1E + 1), (s16 *)snapshot);
    ovl_11_func_800F84F0(0x40);
    items += D_80126E4A * 48;
    ovl_11_func_800F882C(display, 9, 8, (s16 *)items);
    ovl_11_func_800F87AC(text, (s32)panel, D_80126E44, D_80126E4A);
    if (D_80126E4E == 0xFF) {
        func_800226D8(0);
        func_80017B18(0);
        func_8002261C(3, message);
    }
}
