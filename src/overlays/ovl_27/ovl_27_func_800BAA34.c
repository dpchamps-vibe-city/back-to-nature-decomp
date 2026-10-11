#include "common.h"

void ovl_27_func_800BAA34(void) {
    char *base;
    char *rec;
    char *a_x;
    char *a_y;
    char *a_z;
    char *b_x;
    char *b_y;
    char *b_z;
    s16 *list;
    s32 *slot_x;
    s32 *slot_y;
    s32 *slot_z;
    u32 i;

    base = (char *)&D_80071A00;
    i = 0;
    rec = base + 0x88;
    *(s32 *)(base + 0x6C) = 0;
    *(s16 *)(base + 0x36) = 0;
    *(s16 *)(base + 0x16) = 0;
    *(s16 *)(base + 0x18) = 0;
    *(s16 *)(base + 0x1A) = 0;
    *(s16 *)(base + 0x1C) = 0;
    *(s16 *)(base + 0x38) = 0;
    *(s16 *)(base + 0x3A) = 0;
    *(s16 *)(base + 0x3C) = 0;
    *(s16 *)(base + 0x3E) = 0;
    *(s32 *)(base + 0x50) = 0;
    *(s32 *)(base + 0x54) = 0;
    *(s16 *)(base + 0x22) = -1;
    /* The 100s follow the 0x22 store: the longer -1 lifetime lets
       local-alloc place the load first, leaving $a0 for the -1. */
    *(s16 *)(base + 0x12) = 100;
    *(s16 *)(base + 0x14) = 100;
    *(s16 *)(base + 0x24) = 0;
    *(s32 *)(base + 0x58) = 0;
    *(s32 *)(base + 0x5C) = 500;
    *(s16 *)(base + 0x2CA) = -1;
    *(s32 *)(base + 0x4C) = 0;
    *(s16 *)(base + 0x2C) = 0;
    *(u16 *)(base + 0x2A) = *(u16 *)(base - 0xCFE);
    do {
        *(s16 *)(rec - 4) = 0;
        *(s16 *)(rec - 2) = 0;
        *(s16 *)rec = 0;
        rec += 6;
        i += 1;
    } while (i < 2);
    i = 0;
    list = (s16 *)(base + 0x40);
    slot_x = (s32 *)(base + 0x120);
    slot_y = (s32 *)(base + 0x124);
    slot_z = (s32 *)(base + 0x128);
    b_z = base + 0xC4;
    b_y = base + 0xC2;
    b_x = base + 0xC0;
    a_z = base + 0x94;
    a_y = base + 0x92;
    a_x = base + 0x90;
    do {
        *(s16 *)a_x = 0;
        *(s16 *)a_y = 0;
        *(s16 *)a_z = 0;
        *(s16 *)b_x = 0;
        *(s16 *)b_y = 0;
        *(s16 *)b_z = 0;
        b_z += 6;
        b_y += 6;
        b_x += 6;
        a_z += 6;
        a_y += 6;
        i += 1;
        a_x += 6;
    } while (i < 8);
    i = 0;
    *(s16 *)(base + 0xFE) = 1;
    *(s16 *)(base + 0xF0) = 0;
    *(s16 *)(base + 0xF2) = 0;
    *(s16 *)(base + 0xF4) = 0;
    *(s16 *)(base + 0xFC) = 0;
    *(s16 *)(base + 0x1E) = 0;
    *(s16 *)(base + 0x20) = 2;
    do {
        list[i] = 0;
        i += 1;
    } while (i < 5);
    i = 0;
    *(s16 *)(base + 0x2CC) = -1;
    *(s16 *)(base + 0x2CE) = 0;
    do {
        slot_x[i * 4] = 0;
        slot_y[i * 4] = 0;
        slot_z[i * 4] = 0;
        i += 1;
    } while (i < 10);
    *(s16 *)(base + 0x32) = 0;
    *(s16 *)(base + 0x2C0) = -1;
    *(s16 *)(base + 0x2C2) = -1;
    *(s16 *)(base + 0x2C4) = -1;
    *(s16 *)(base + 0x2C8) = -1;
    *(s16 *)(base + 0x2CA) = -1;
    *(s16 *)(base + 0x28) = 0;
}
