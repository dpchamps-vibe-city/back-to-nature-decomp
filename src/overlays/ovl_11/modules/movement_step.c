/*
 * movement_step — reconstructed translation unit.
 *
 * Movement-record tile-probe latch and vertical-offset progression. The probe
 * sets bit 0x01000000; reaching zero in the vertical step clears that bit and
 * the active-step bit 0x00400000.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_movement_step.h"
#include "ovl11_directional_tile_probe.h"

s32 Ovl11ProbeRecordStep(Recon_ovl_11_func_800D2594_A0View *record,
                        VECTOR position, s16 direction, s32 latchResultTwo) {
    s32 result;

    result = Ovl11ProbeDirectionalTile(position,
                                *(Ovl11CollisionTag *) ((u8 *) record + 0x2E),
                                direction, 0);
    if (result == 2 && latchResultTwo == 1) {
        record->unk34 |= 0x01000000;
    }
    return result;
}

s32 Ovl11StepRecordVerticalOffset(Recon_ovl_11_func_800D2594_A0View *record,
                                 s32 movePeriod) {
    s32 verticalStep;
    s32 active;

    active = 1;
    /* Preserve the two signed divisions and the remainder correction: this
     * is integer arithmetic, not a continuous speed/period conversion. */
    verticalStep = 0x190 / movePeriod / 2;
    if (0x190 % movePeriod != 0) {
        verticalStep += 2;
    }
    verticalStep = 0xC8 / verticalStep;
    if ((record->unk34 & 0x400000) == 0) {
        if (record->unk3C < -0xC8) {
            record->unk4C = verticalStep;
            record->unk34 = record->unk34 | 0x400000;
        } else {
            record->unk4C = -verticalStep;
        }
    } else {
        record->unk4C = verticalStep;
        if (record->unk3C + verticalStep >= 0) {
            record->unk4C = 0;
            record->unk3C = 0;
            record->unk34 &= 0xFEFFFFFF;
            record->unk34 &= 0xFFBFFFFF;
            active = 0;
        }
    }
    return active;
}
