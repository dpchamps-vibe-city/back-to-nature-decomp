#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 func_8001FABC(s16 arg0);

void ovl_30_func_80133A8C(Ovl30FlagMenuEntry *arg0, s32 arg1) {
    char *labels[2];
    s32 i;
    s32 label;
    s32 buttons;
    Ovl30FlagMenuEntry *entry;
    Ovl30FlagMenuEntry *selected;

    labels[0] = D_8012EFB0[0];
    labels[1] = D_8012EFB0[1];
    for (i = D_80134014 / 9 * 9; i < D_80134014 / 9 * 9 + 9; i++) {
        entry = &arg0[i];
        if (i < arg1) {
            label = entry->id == -2;
            switch (entry->width) {
            case 0:
            default:
                KanjiFntPrint(D_8012EFB8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text);
                break;
            case 1:
                if (*(u8 *)entry->value & entry->mask) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 0);
                }
                break;
            case 2:
                if (*(s16 *)entry->value & entry->mask) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 0);
                }
                break;
            case 4:
                if (*(s32 *)entry->value & entry->mask) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, 0);
                }
                break;
            }
        } else {
            KanjiFntPrint(D_8012EFE8);
        }
    }
    buttons = *(s32 *)D_8005E3A8;
    if (buttons & 0x1000) {
        D_80134014--;
        if (D_80134014 < 0) {
            D_80134014 = arg1 - 1;
        }
    } else if (buttons & 0x4000) {
        D_80134014++;
        if (D_80134014 > arg1 - 1) {
            D_80134014 = 0;
        }
    } else if (buttons & 0x8000) {
        D_80134014 -= 9;
        if (D_80134014 < 0) {
            D_80134014 = 0;
        }
    } else if (buttons & 0x2000) {
        D_80134014 += 9;
        if (D_80134014 > arg1 - 1) {
            D_80134014 = arg1 - 1;
        }
    } else {
        goto edit_entry;
    }
    func_8001FABC(5);
    return;

edit_entry:
    {
        if (((s32 *)D_8005E3A8)[2] & 0x100) {
            D_80134B10 = arg0[D_80134014].id;
            return;
        }
        selected = (Ovl30FlagMenuEntry *)(D_80134014 * sizeof(*arg0) + (u32)arg0);
        if (selected->id != -2) {
            if (buttons & 0x20) {
                switch (selected->width) {
                case 0:
                    break;
                case 1:
                    *(u8 *)selected->value |= (u8)selected->mask;
                    break;
                case 2:
                    *(u16 *)selected->value |= (u16)selected->mask;
                    break;
                case 4:
                    *(s32 *)selected->value |= selected->mask;
                    break;
                }
            } else if (buttons & 0x40) {
                switch (selected->width) {
                case 0:
                    break;
                case 1:
                    *(u8 *)selected->value &= ~(u8)selected->mask;
                    break;
                case 2:
                    *(u16 *)selected->value &= ~(u16)selected->mask;
                    break;
                case 4:
                    *(s32 *)selected->value &= ~selected->mask;
                    break;
                }
            }
        }
        return;
    }
}
