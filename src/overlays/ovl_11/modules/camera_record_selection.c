/*
 * camera_record_selection — reconstructed translation unit.
 *
 * Resolve an optional value-slot selector into a camera-position record,
 * drive the shared camera target and cache successful selections.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_camera_position_control.h"
#include "ovl11_camera_record_selection.h"

s32 Ovl11SelectCameraPositionRecord(s16 recordOrSlot, s16 fromSlot, s32 moveMode, s32 speedCode) {
    s32 result;
    s32 index;
    u8 *position;

    if (fromSlot != 0) {
        index = gOvl11ValueSlots[recordOrSlot];
    } else {
        index = recordOrSlot;
    }
    if (index == 0x29) {
        index = -2;
    }
    result = 1;
    ((struct struct_8006C838_view *)D_8006C838)->field_0C |= 0x400000;
    if (index != -1 && moveMode != 0) {
        if (gOvl11SelectedPositionRecord == index) {
            return 1;
        }
        /* -2 selects the primary object's coordinates, not the record pool. */
        if (index == -2) {
            position = (u8 *)D_8006C838 + 0x52C8;
        } else {
            if ((u32)(index - 1) >= 0x24U) {
                return 0;
            }
            position = (u8 *)gObjectPositionRecordCoordinates + index * 0x1D4;
        }
        result = Ovl11MoveCameraTarget(speedCode, *(s16 *)(position + 4),
                                    *(s32 *)position, *(s32 *)(position + 8));
    }
    if (result == 1) {
        if (moveMode != 2) {
            gOvl11SelectedPositionRecord = index;
            if (index == -2) {
                ((struct struct_8006C838_view *)D_8006C838)->field_0C &= ~0x400000;
            }
        }
    }
    return result;
}
