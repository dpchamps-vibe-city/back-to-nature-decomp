#include "common.h"
#include "psyq/memory.h"
#include "random.h"

s32 ovl_11_func_800D3D60(u16 arg0) {
    u8 buf[20];
    s32 count;
    s32 i;
    s32 limit;
    u8 *p;
    u8 *large;
    u8 *small;
    s32 smallOffset;
    s32 largeOffset;
    u32 mask;
    s32 one;

    memset(buf, 0xFF, 20);
    count = 0;
    limit = 10;
    one = 1;
    if (arg0 == 0) {
        limit = 20;
    }
    i = 0;
    if (limit != 0) {
        mask = 0x20400;
        large = (u8 *)&gTaggedRecordBase;
        small = large - 0x708;
        smallOffset = 0;
        largeOffset = 0;
        do {
            if (arg0 == 0) {
                p = (u8 *)(largeOffset + (u32)large);
            } else {
                p = (u8 *)(smallOffset + (u32)small);
            }
            do {
                do {
                    if (*(u32 *)(p + 0x34) & mask) {
                        goto next_record;
                    }
                    if (*(s16 *)(p + 0x30) != one) {
                        goto next_record;
                    }
                } while (0);
                if (*(u16 *)p == 0) {
                    goto next_record;
                }
            } while (0);
            buf[count++] = i;
next_record:
            smallOffset += 0xB4;
            largeOffset += 0xB8;
            i++;
        } while (i < limit);
    }
    if (count == 0) {
        return -1;
    }
    return buf[Rand(count & 0xFFFF)];
}
