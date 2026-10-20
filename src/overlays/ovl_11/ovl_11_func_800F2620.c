#include "common.h"
#include "game_types.h"

void ovl_11_func_800F27B0(s16 kind, s16 value);
void ovl_11_func_800F2880(s16 arg0, s32 arg1);
void ovl_11_func_800F291C(void);

void ovl_11_func_800F2620(void) {
    s16 day;
    s32 i;
    s8 kind;
    Ovl11QueuedEntry *p;
    u8 *base;

    base = (u8 *)D_8006C838;
    p = (Ovl11QueuedEntry *)(base + 0x49E4);
    i = 0;
    day = *(s16 *)(base + 0x44BC);
    do {
        if (p->kind != -1 && (s8)p->day == day) {
            ovl_11_func_800F2880(i, 1);
        }
        i++;
        p++;
    } while (i < 20);

    base = D_8007121C;
    p = (Ovl11QueuedEntry *)(base + 0x50);
    for (i = 0; i < 20; i++, p++) {
        kind = p->kind;
        if (kind != -1) {
            if ((s8)--p->day <= 0) {
                ovl_11_func_800F27B0(kind, p->value);
                p--;
                ovl_11_func_800F2880(i, 0);
                i--;
            }
        }
    }
    D_80126C90 = 0;
    ovl_11_func_800F291C();
}
