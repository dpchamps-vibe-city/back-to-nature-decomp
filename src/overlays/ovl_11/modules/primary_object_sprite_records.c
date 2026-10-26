/*
 * primary_object_sprite_records — reconstructed translation unit.
 *
 * Initialize the primary object record sets and preload the sprite data
 * selected by their IDs. The reset seeds records read by the preload loops;
 * neither routine owns the external object storage or asset staging buffer.
 * See ovl11_primary_object_sprite_records.h for record extents and effects.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_sprite_records.h"

s32 ovl_11_func_800D6A94(s16 arg0);

void Ovl11LoadPrimaryObjectRecordSprites(void) {
    u32 i;
    s16 (*base)[3];
    s16 (*table)[3];

    base = (s16 (*)[3])&gPrimaryObjectBase;
    table = base + 22;
    for (i = 0; i < 2; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
    table = base + 32;
    for (i = 0; i < 8; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
    table = base + 24;
    for (i = 0; i < 8; i++) {
        ovl_11_func_800D6A94(table[i][0]);
    }
}

void ovl_11_func_800CD0C0(s32 *arg0);

void Ovl11InitPrimaryObjectRecordSprites(void) {
    u8 *base;

    base = (u8 *)&gPrimaryObjectBase;
    D_80128C54 = 0;
    D_80128C4C_dispatch = 0;
    gOvl11PrimaryHeldButtons = 0;
    gOvl11PrimaryPressedButtons = 0;
    if (*(s32 *)(base - 0x51BC) & 0x8000) {
        *(s16 *)(base + 0x90) = 3;
        *(s16 *)(base + 0x92) = 2;
        *(s16 *)(base + 0x96) = 8;
        *(s16 *)(base + 0x98) = 2;
        *(s16 *)(base + 0x9C) = 0xD;
        *(s16 *)(base + 0x9E) = 2;
        *(s16 *)(base + 0xA2) = 0x12;
        *(s16 *)(base + 0xA4) = 2;
        *(s16 *)(base + 0xA8) = 0x17;
        *(s16 *)(base + 0xAA) = 2;
        *(s16 *)(base + 0xAC) = 0x64;
        *(s16 *)(base + 0xAE) = 0x22;
        *(s16 *)(base + 0xB2) = 0xA;
        *(s16 *)(base + 0x1E) = 2;
        *(s16 *)(base + 0x20) = 8;
    }
    Ovl11LoadPrimaryObjectRecordSprites();
    ovl_11_func_800CD0C0((s32 *)(base + 0x100));
    D_80128D30 = 0;
    gOvl11MotionHoldCount = 0;
    gOvl11ItemActionChargeFrames = 0;
}
