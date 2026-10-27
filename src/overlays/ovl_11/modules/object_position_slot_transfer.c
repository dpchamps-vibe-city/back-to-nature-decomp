/* Complementary position-triple commands for the shared script value slots.
 * Slot order is height/X/Z. The live halfword snapshot deliberately differs
 * from the selected-record word snapshot and from the apply command's
 * low-halfword height load. Keep those widths and the asymmetric guards.
 * Original entries: ovl_11:800E9724/9778/9838. Conservative -G0 membership
 * and boundaries: notes/file-groupings.md. */
#include "ovl11_object_position_slot_transfer.h"

s32 ovl_11_func_800E60A0(s16 recordId, s32 height, s32 x, s32 z);

s32 Ovl11ApplySlotPositionTriple(s16 recordId, s16 heightSlot, s32 xSlot, s32 zSlot) {
    return ovl_11_func_800E60A0(recordId, *(s16 *)&gOvl11ValueSlots[heightSlot],
                              gOvl11ValueSlots[xSlot], gOvl11ValueSlots[zSlot]);
}

s32 Ovl11SaveRecordPositionToSlots(s16 recordId, s16 heightSlot, s32 xSlot, s32 zSlot) {
    s32 *position;

    if (recordId == 0x29) {
        position = gObjectWorkPosition;
    } else if ((u32)(recordId - 1) < 0x24) {
        position = (s32 *)((char *)gObjectPositionRecordCoordinates + recordId * 0x1D4);
    } else {
        return 0;
    }
    if (heightSlot != -1) {
        gOvl11ValueSlots[heightSlot] = position[1];
    }
    if (xSlot != -1) {
        gOvl11ValueSlots[xSlot] = position[0];
    }
    if (zSlot != -1) {
        gOvl11ValueSlots[zSlot] = position[2];
    }
    return 1;
}

s32 Ovl11SaveLivePositionToSlots(s16 consecutive, s16 heightSlot, s32 xSlot, s32 zSlot) {
    s32 effectiveXSlot;

    effectiveXSlot = xSlot;
    if (consecutive != 0) {
        if (heightSlot == -1) {
            return 0;
        }
        effectiveXSlot = heightSlot + 1;
        zSlot = heightSlot + 2;
    }
    if (heightSlot != -1 && heightSlot < 0x14) {
        s32 *slots = gOvl11ValueSlots;
        s32 offset = heightSlot << 2;
        char *work = (char *)&D_8007AFF0;
        *(s32 *)((char *)slots + offset) = *(s16 *)(work + 0x253B6);
    }
    if (effectiveXSlot != -1 && effectiveXSlot < 0x14) {
        s32 *slots = gOvl11ValueSlots;
        s32 offset = effectiveXSlot << 2;
        char *work = (char *)&D_8007AFF0;
        *(s32 *)((char *)slots + offset) = *(s16 *)(work + 0x253B4);
    }
    if (zSlot != -1 && zSlot < 0x14) {
        s32 *slots = gOvl11ValueSlots;
        s32 offset = zSlot << 2;
        char *work = (char *)&D_8007AFF0;
        *(s32 *)((char *)slots + offset) = *(s16 *)(work + 0x253B8);
    }
    return 1;
}
