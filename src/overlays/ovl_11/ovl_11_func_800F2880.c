#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"

void ovl_11_func_800F2880(s16 arg0, s32 arg1) {
    s32 index;
    s32 off;
    Ovl11QueuedEntry *p;
    Ovl11QueuedEntry *src;
    Ovl11QueuedEntry *q;
    s32 len;

    index = arg0;
    if (arg1 == 1) {
        u8 *base = D_8007121C;
        u8 *mid;
        off = index * 4;
        p = (Ovl11QueuedEntry *)(off + (s32)base);
        mid = base + 4;
        src = (Ovl11QueuedEntry *)(off + (s32)mid);
        len = 0x50 - (index + 1) * 4;
        q = (Ovl11QueuedEntry *)(base + 0x4C);
    } else {
        u8 *base = D_8007126C;
        u8 *mid;
        off = index * 4;
        p = (Ovl11QueuedEntry *)(off + (s32)base);
        mid = base + 4;
        src = (Ovl11QueuedEntry *)(off + (s32)mid);
        len = 0x50 - (index + 1) * 4;
        q = (Ovl11QueuedEntry *)(base + 0x4C);
    }
    if (index == 19) {
        p->kind = -1;
        p->value = 0;
        p->day = 0;
    } else {
        memmove((char *)p, (char *)src, len);
    }
    q->kind = -1;
    q->value = 0;
    q->day = 0;
}
