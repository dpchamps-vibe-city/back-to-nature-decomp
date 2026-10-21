#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"

void ovl_11_func_800F27B0(s16 kind, s16 value);

void ovl_11_func_800F2538(s16 kind, s16 value, s16 days) {
    s32 i;
    Ovl11QueuedEntry *p;
    u8 *tail;

    if (days == 0) {
        ovl_11_func_800F27B0(kind, value);
        return;
    }
    if (value != 0) {
        p = (Ovl11QueuedEntry *)D_8007126C;
        for (i = 0; i < 20; i++, p++) {
            if (p->kind == -1) {
                p->kind = kind;
                p->value = value;
                p->day = days;
                break;
            }
        }
        if (i == 20) {
            tail = D_8007126C;
            memmove(tail, tail + 4, 0x4C);
            tail -= 0x4A34;
            *(s8 *)(tail + 0x4A82) = kind;
            *(s16 *)(tail + 0x4A80) = value;
            *(u8 *)(tail + 0x4A83) = days;
        }
    }
}
