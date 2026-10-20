#include "common.h"
#include "game_types.h"

s32 func_8002261C(s32 arg0, s32 arg1);
s32 ovl_11_func_800D60F0(s16 arg0, s16 arg1);

s32 ovl_11_func_8010DE68(Ovl11DE68Object *arg0) {
    s16 message;
    s16 item;
    s32 marked;
    s32 eligible;
    s32 multiplier;
    s32 alternate;
    u16 clamped;
    u16 flags;

    message = 0;
    marked = 0;
    eligible = 0;
    multiplier = 1;
    alternate = 0;
    if (arg0->unkB8 & 0x10) {
        alternate = 1;
    }
    item = ((Ovl11DE68State *)D_8006C838)->item;
    if (arg0->unkC4 == ((Ovl11DE68State *)D_8006C838)->area) {
        if (arg0->unkC6 == ((Ovl11DE68State *)D_8006C838)->room) {
            multiplier = 5;
        }
    }
    if (item == 0xA4) {
        item = ((Ovl11DE68State *)D_8006C838)->alternateItem;
        arg0->unk16 += 2;
    }
    switch (ovl_11_func_800D60F0(item, 0x1B)) {
    case 0:
        message = D_80127D14[arg0->unkAC];
        arg0->unk16 += D_80127D34[alternate][0] * multiplier;
        if (multiplier != 1) {
            message = D_80127D24[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 1:
        message = D_80127D14[arg0->unkAC] + 1;
        arg0->unk16 += D_80127D34[alternate][1] * multiplier;
        if (multiplier != 1) {
            message = D_80127D24[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 2:
        message = D_80127D14[arg0->unkAC] + 2;
        arg0->unk16 += D_80127D34[alternate][2] * multiplier;
        if (multiplier != 1) {
            message = D_80127D24[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 3:
        message = D_80127D14[arg0->unkAC] + 3;
        arg0->unk16 += D_80127D34[alternate][3] * multiplier;
        marked = 1;
        break;
    case 4:
        message = D_80127D14[arg0->unkAC] + 4;
        arg0->unk16 += D_80127D34[alternate][4] * multiplier;
        marked = 1;
        break;
    case 5:
        message = D_80127D14[arg0->unkAC] + 7;
        break;
    case 6:
        message = D_80127D14[arg0->unkAC] + 8;
        break;
    }
    if (item >= 0xF7) {
        if (item >= 0xFA) {
            if (item == 0x100) {
                marked = 0;
                message = D_80127D14[arg0->unkAC] + 6;
            }
        } else {
            marked = 1;
            message = D_80127D14[arg0->unkAC] + 9;
        }
    }
    if (eligible == 1 && arg0->unkAC == 0 && message != 0) {
        flags = arg0->unkB8;
        if (!(flags & 0x400) && ((Ovl11DE68State *)D_8006C838)->event != 0 && (s16)arg0->unk16 >= 0x65 && item != 0xC1) {
            message = D_80127D14[arg0->unkAC] + 0xA;
            arg0->unkB8 = flags | 0x400;
        }
    }
    clamped = arg0->unk16;
    if ((s16)arg0->unk16 >= 0x100) {
        clamped = 0xFF;
    }
    arg0->unk16 = clamped;
    if ((s16)clamped < 0) {
        clamped = 0;
    }
    arg0->unk16 = clamped;
    if (message != 0) {
        func_8002261C(0, message);
        if (marked == 1) {
            arg0->unkB8 |= 0x810;
        }
        return 0;
    }
    return -1;
}
