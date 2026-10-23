#include "common.h"
#include "game_types.h"

void ovl_11_func_80110E98(Ovl11DE68Object *arg0, s32 *arg1, s32 *arg2) {
    s32 coord;

    coord = 0;
    if (arg0->unk30 == 0x28) {
        if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->field_44BA == 3 &&
            ((struct Ovl11CalendarDisplayStateView *) D_8006C838)->field_44BC >= 0x1C) {
            *arg1 = 0;
            *arg2 = 2;
        } else if (((struct Ovl11CalendarDisplayStateView *) D_8006C838)->field_44BA != 0) {
            *arg1 = 0;
            *arg2 = 0;
        } else {
            *arg1 = 0;
            *arg2 = 2;
        }
    } else if (arg0->unkB8 & 0x40) {
        switch (arg0->unk26) {
        case 2:
            *arg1 = 0;
            *arg2 = 0xA;
            arg0->unk2C = 0x12C;
            arg0->unk24 = 0;
            arg0->unk34 &= ~0x800;
            break;
        case 3:
            *arg1 = 0;
            *arg2 = 0xC;
            break;
        case 4:
        case 7:
            *arg1 = 0;
            *arg2 = 8;
            break;
        default:
            *arg1 = 0;
            *arg2 = 8;
            break;
        }
    } else {
        if (arg0->unkB8 & 1) {
            coord = arg0->unkAE;
        } else if (arg0->unkB8 & 2) {
            coord = arg0->unkB0;
        } else if (arg0->unkB8 & 4) {
            coord = arg0->unkB2;
        }
        if (coord < 0xC8) {
            *arg1 = 0;
            *arg2 = 4;
        } else {
            *arg1 = 0;
            *arg2 = 6;
        }
    }
    *arg2 += D_80127F50[arg0->unkAC];
    if (arg0->unkB8 & 8) {
        *arg2 = *arg2 + 1;
    }
}
