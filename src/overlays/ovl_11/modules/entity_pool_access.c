/*
 * Scene-entity pool access (ovl_11:800E8960..800E8BF0).
 *
 * The work-area pointer at +0xDDD0 selects 24-byte records. These operations
 * write the source tag at +0, read the value at +2, control bit 0 at +4,
 * and write the position at +8/+C/+10. Other fields are left untouched.
 * Membership and the limits of the original-TU evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_entity_pool_access.h"

s32 Ovl11ActivateEntityAtPosition(s16 entityIndex, s16 height, s32 x, s32 z) {
    s32 *work;
    s32 *upperWork;
    u8 *entity;
    char *farWork;
    s32 byteOffset;
    u16 sourceTag;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    byteOffset = entityIndex * 0x18;
    farWork = (char *)&D_8007AFF0;
    sourceTag = *(u16 *)(farWork + 0x25476);
    entity = (u8 *)upperWork[0x5DD0 >> 2] + byteOffset;

    *(u16 *)entity = sourceTag;
    *(s32 *)(entity + 0xC) = height;
    *(s32 *)(entity + 0x8) = x;
    *(s32 *)(entity + 0x10) = z;
    *(u16 *)(entity + 0x4) |= 1;
    return 1;
}

s32 Ovl11SetEntityActive(s16 entityIndex, s32 active) {
    s32 *work;
    s32 *upperWork;
    u8 *pool;
    u16 *entity;
    s32 scaledIndex;
    s32 lowHalfwordTest;

    /* The original interface tests only the low 16 bits, not the full word. */
    lowHalfwordTest = active << 0x10;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    scaledIndex = entityIndex * 3;
    pool = (u8 *)upperWork[0x5DD0 >> 2];
    entity = (u16 *)(pool + (scaledIndex << 3));

    if (lowHalfwordTest == 0) {
        entity[2] &= 0xFFFE;
    } else {
        entity[2] |= 1;
    }
    return 1;
}

/* Direction classifier shared with other object operations. */
s32 ovl_11_func_800F06D8(s32 *arg0, s32 *arg1);

s32 Ovl11ActivateEntityNearRecord(s16 entityIndex, s16 recordIndex,
                                 s32 direction, s32 classifyDirection) {
    s32 offset[3];
    s32 *work;
    s32 *upperWork;
    u8 *entity;
    char *farWork;
    s32 *position;
    s32 selectedDirection;
    u16 sourceTag;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    entity = (u8 *)upperWork[0x5DD0 >> 2] + entityIndex * 0x18;
    farWork = (char *)&D_8007AFF0;
    sourceTag = *(u16 *)(farWork + 0x25476);

    *(u16 *)(entity + 0x4) |= 1;
    position = (s32 *)((char *)gObjectPositionRecordCoordinates + recordIndex * 0x1D4);
    *(u16 *)entity = sourceTag;

    if (classifyDirection != 0) {
        selectedDirection = ovl_11_func_800F06D8((s32 *)((char *)work + 0x52C8), position);
    } else {
        selectedDirection = direction & 0xFFFF;
    }
    /* Horizontal offset of 100 coordinate units; height is always 100 lower.
     * There is deliberately no fallback for directions outside 0..3. */
    switch (selectedDirection) {
    case 0:
        offset[0] = 0;
        offset[1] = -0x64;
        offset[2] = -0x64;
        break;
    case 1:
        offset[0] = -0x64;
        offset[1] = -0x64;
        offset[2] = 0;
        break;
    case 2:
        offset[0] = 0;
        offset[1] = -0x64;
        offset[2] = 0x64;
        break;
    case 3:
        offset[0] = 0x64;
        offset[1] = -0x64;
        offset[2] = 0;
        break;
    }
    *(s32 *)(entity + 0xC) = position[1] + offset[1];
    *(s32 *)(entity + 0x8) = position[0] + offset[0];
    *(s32 *)(entity + 0x10) = position[2] + offset[2];
    return 1;
}

s32 Ovl11CopyEntityValueToSlot(s16 entityIndex, s16 slotIndex) {
    s32 *work;
    s32 *upperWork;
    u8 *pool;
    s32 byteOffset;
    s32 value;

    work = (s32 *)&D_8006C838;
    upperWork = work + (0x8000 >> 2);
    pool = (u8 *)upperWork[0x5DD0 >> 2];
    byteOffset = entityIndex * 0x18;

    value = *(u16 *)(pool + byteOffset + 2);
    gOvl11ValueSlots[slotIndex] = value;
    return 1;
}
