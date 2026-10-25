#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 func_8001FABC(s16 arg0);
void ovl_30_func_80133974(u8 *arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4);

void ovl_30_func_80133454(Ovl30ValueMenuEntry *arg0, s32 arg1) {
    char *labels[2];
    s32 i;
    s32 label;
    s32 buttons;
    Ovl30ValueMenuEntry *entry;
    Ovl30ValueMenuEntry *selected;

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
                if (entry->lower == 0) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *entry->value);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *(s8 *)entry->value);
                }
                break;
            case 2:
                if (entry->lower == 0) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *(u16 *)entry->value);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *(s16 *)entry->value);
                }
                break;
            case 4:
                if (entry->lower == 0) {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *(u32 *)entry->value);
                } else {
                    KanjiFntPrint(D_8012EFD8, i == D_80134014 ? D_8012EFD0 : D_8012EFD4, labels[label], entry->text, *(s32 *)entry->value);
                }
                break;
            }
        } else {
            KanjiFntPrint(D_8012EFE8);
        }
    }
    buttons = *(s32 *)gPad0Input;
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
    if (((s32 *)gPad0Input)[2] & 0x100) {
        D_80134B10 = arg0[D_80134014].id;
        return;
    }
    selected = (Ovl30ValueMenuEntry *)(D_80134014 * sizeof(*arg0) + (u32)arg0);
    if (selected->id != -2) {
        if (buttons & 0x20) {
            ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, 1);
        } else if (buttons & 0x40) {
            ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, -1);
        } else if (buttons & 0x10) {
            if (((s32 *)gPad0Input)[1] & 0x100) {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, 100);
            } else if (((s32 *)gPad1Input)[1] & 8) {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, 1000);
            } else {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, 10);
            }
        } else if (buttons & 0x80) {
            if (((s32 *)gPad0Input)[1] & 0x100) {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, -100);
            } else if (((s32 *)gPad1Input)[1] & 8) {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, -1000);
            } else {
                ovl_30_func_80133974(selected->value, selected->width, selected->upper, selected->lower, -10);
            }
        }
    }
}
