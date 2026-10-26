/*
 * work_area_init — reconstructed translation unit.
 *
 * The title setup initializer quartet: frame-ID records, primary-object
 * defaults, work-option halfwords/stereo and five initial six-byte work
 * records. One coordinated setup call sequence and private resident tables
 * corroborate original order; see notes/file-groupings.md.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "sound_driver.h"
#include "ovl27_work_area_init.h"

void Ovl27InitSpriteFrameRecords(s32 tableIndex) {
    Ovl11D124Entry *var_a1;
    s32 i;
    s32 j;
    s32 k;
    u16 *var_a3;
    s16 fill;
    u8 *base;
    D8006C838Pools *temp_t1;

    switch (tableIndex) {
    case 1:
        var_a3 = &gOvl27SpriteFrameIds1;
        break;
    case 2:
        var_a3 = &gOvl27SpriteFrameIds2;
        break;
    case 3:
        var_a3 = &gOvl27SpriteFrameIds3;
        break;
    case 0:
        var_a3 = &gOvl27SpriteFrameIds0;
        break;
    default:
        var_a3 = &gOvl27SpriteFrameIds0;
        break;
    }
    i = 0;
    fill = 0x167;
    base = (u8 *) D_80071DFC;
    temp_t1 = (D8006C838Pools *) (base - 0x55C4);
    do {
        k = i + 1;
        var_a1 = (Ovl11D124Entry *) (base + i * 0x168);
        for (j = 0x2C; j >= 0; j--) {
            u32 temp_v0;

            var_a1->unk0 = fill;
            var_a1->unk2 = fill;
            var_a1->unk4 = 0;
            var_a1->unk5 = 0;
            var_a1->unk6 = 0;
            temp_v0 = *var_a3;
            var_a1->unk0 = (s16) temp_v0;
            temp_v0 = temp_v0 - 0x22;
            temp_v0 = temp_v0 & 0xFFFF;
            if (temp_v0 < 0xFU) {
                var_a1->unk4 = temp_t1->unk2C[temp_t1->unk20[(u16) var_a1->unk0].unk3].unk2;
                var_a1->unk2 = 0x168;
            }
            var_a1 += 1;
            var_a3 += 1;
        }
        i = k;
    } while (i < 0x19);
}

void Ovl27InitPrimaryObject(void) {
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

    base = (char *)&gPrimaryObjectBase;
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

void Ovl27InitWorkOptions(void) {
    char *base;

    /* Two-stage base formation keeps +0x8000 as runtime ori/addu (matched idiom) */
    base = (char *)&D_8006C838;
    base += 0x8000;
    *(s16 *)(base + 0x64CC) = 0;
    *(s16 *)(base + 0x64CE) = 1;
    *(s16 *)(base + 0x64D0) = 1;
    *(s16 *)(base + 0x64D2) = 1;
    SetSoundStereo();
}

/* Both ends are only halfword-aligned; copy six bytes, not an eight-byte
 * naturally aligned record with trailing padding. */
typedef struct __attribute__((packed, aligned(2))) {
    s32 unk0;
    s16 unk4;
} WorkRecord6;

void Ovl27CopyInitialWorkRecords(void) {
    WorkRecord6 *src;
    WorkRecord6 *dst;
    char *base;
    u32 i;

    base = (char *)&D_8006C838;
    dst = (WorkRecord6 *)(base + 0x468A);
    src = (WorkRecord6 *)gOvl27InitialWorkRecords;
    for (i = 0; i < 5; i++) {
        dst[i] = src[i];
    }
}
