/*
 * Scene value-slot commands, ovl_11:800EE604-800EEBF4.
 *
 * Script callbacks share the twenty-word value bank with the scene services.
 * This unit steps a camera move, classifies conditions and expands that
 * classification into code slots. Camera interpolation scratch is shared:
 * concurrent moves are not supported. See ovl11_scene_value_commands.h for
 * operand contracts, return timing and unchecked-index constraints.
 */
#include "common.h"
#include "ovl11_scene_value_commands.h"
#include "ovl11_scene_camera_lighting.h"
#include "word_interpolation.h"
#include "psyq/memory.h"

/* Narrow view starting at work +25394. Keeping the fields in one address
 * web preserves the original far-buffer load order. Target Y is a halfword;
 * its interpolation endpoints retain signed-halfword narrowing. */
typedef struct {
    s32 distance;
    s32 height;
    u8 pad[0x1A];
    u16 targetY;
} CameraCommandWork;

s32 Ovl11StepCameraValueCommand(s16 stateSlot, s16 tickSlot,
                              s32 unused, s32 resetOrbit) {
    s32 height;
    s32 distance;
    s32 targetY;
    s32 *ticks;
    s32 *base;
    s32 *phase;
    s32 state;
    s32 count;
    s32 complete;

    complete = 0;
    if (resetOrbit != 0) {
        Ovl11ResetSceneCameraOrbit();
    }
    base = gOvl11ValueSlots;
    phase = (s32 *)((u8 *)base + (stateSlot * 4));
    state = *phase;
    if (state != 1) {
        if (state < 2) {
            if (state == 0) {
                char *work = (char *)&D_8007AFF0;
                gOvl11CameraCommandEndHeight = -0x320;
                gOvl11SelectedPositionRecord = -1;
                *phase = 1;
                gOvl11CameraCommandStartHeight = ((CameraCommandWork *)(work + 0x25394))->height;
                gOvl11CameraCommandStartDistance = ((CameraCommandWork *)(work + 0x25394))->distance;
                gOvl11CameraCommandStartTargetY = ((CameraCommandWork *)(work + 0x25394))->targetY;
                gOvl11CameraCommandEndDistance = ((CameraCommandWork *)(work + 0x25394))->distance - 0x44C;
                gOvl11CameraCommandEndTargetY = ((CameraCommandWork *)(work + 0x25394))->targetY - 0x3E8;
                goto done;
            }
        }
    } else {
        ticks = &gOvl11ValueSlots[tickSlot];
        InterpolateWord(*ticks, 0xB1, gOvl11CameraCommandStartHeight, gOvl11CameraCommandEndHeight, &height);
        InterpolateWord(*ticks, 0xB1, gOvl11CameraCommandStartDistance, gOvl11CameraCommandEndDistance, &distance);
        InterpolateWord(*ticks, 0xB1, gOvl11CameraCommandStartTargetY, gOvl11CameraCommandEndTargetY, &targetY);
        {
            char *work = (char *)&D_8007AFF0;
            ((CameraCommandWork *)(work + 0x25394))->height = height;
            ((CameraCommandWork *)(work + 0x25394))->distance = distance;
            ((CameraCommandWork *)(work + 0x25394))->targetY = targetY;
        }
        count = *ticks + 1;
        *ticks = count;
        if (count >= 0xB1) {
            *phase = 2;
        }
        goto done;
    }
    complete = 1;
done:
    return complete;
}

s32 ovl_11_func_800F002C(void);
s32 ovl_11_func_800F02C8(void);
s16 ovl_11_func_800EFF9C(void);
s32 ovl_11_func_800F00AC(void);
s32 ovl_11_func_800F00E4(void);

s32 Ovl11UpdateConditionValueCommand(s16 mode, s16 outputSlot,
                                   s32 unused2, s32 unused3) {
    s32 category;
    s32 override;
    u16 code;

    if (mode == 0) {
        override = ovl_11_func_800F002C();
        if (override >= 0) {
            ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = (u16) override;
        } else {
            category = ovl_11_func_800F02C8();
            if ((u8) ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_44CD >= 0xAU) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0xAU;
            } else if (ovl_11_func_800EFF9C() < 0x32) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0xCU;
            } else if (ovl_11_func_800F00AC() < 0xB) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0xDU;
            } else if (ovl_11_func_800F00E4() < 6) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0xEU;
            } else if (category == -1) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0xFU;
            } else if (((struct struct_8006C838_800EE7BC *) D_8006C838)->field_91C2 < 6) {
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = 0x10U;
            } else {
                if (category >= 5) {
                    category = 0;
                }
                code = category + 5;
                ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0 = code;
            }
        }
    } else {
        gOvl11ValueSlots[outputSlot] = (s32) ((struct struct_8006C838_800EE7BC *) D_8006C838)->field_E7A0;
    }
    return 1;
}

s16 Ovl11WriteConditionCodeSlots(s16 firstSlot, s16 secondSlot,
                               s32 thirdSlot, s32 fourthSlot) {
    s32 *base;
    s32 *first;
    s32 category;

    Ovl11UpdateConditionValueCommand(1, firstSlot, 0, 0);
    base = gOvl11ValueSlots;
    first = (s32 *) ((u8 *) base + (firstSlot * 4));
    category = *first;
    *first = 0x7B7;
    if (category == 1) {
        gOvl11ValueSlots[secondSlot] = 0x7BA;
    } else if (category == 2) {
        gOvl11ValueSlots[secondSlot] = 0x7BB;
    } else if (category == 4) {
        gOvl11ValueSlots[secondSlot] = 0x7B9;
    } else {
        gOvl11ValueSlots[secondSlot] = 0x7B8;
    }
    if (((struct struct_8006C838_800EE944 *) D_8006C838)->field_AE30 != 0x13) {
        gOvl11ValueSlots[secondSlot] += 4;
    }
    if (category == 3) {
        gOvl11ValueSlots[thirdSlot] = 0x7C1;
    } else if (category == 0) {
        gOvl11ValueSlots[thirdSlot] = 0x7C2;
    } else {
        gOvl11ValueSlots[thirdSlot] = 0x7C0;
    }
    gOvl11ValueSlots[fourthSlot] = gOvl11ConditionCodeOffsets[category] + 0x7C3;
    return 1;
}

s16 Ovl11WriteConditionTripleSlots(s16 firstSlot, s16 secondSlot, s32 thirdSlot) {
    s32 *base;
    s32 *third;
    s32 *first;
    s32 upperGroup;
    s32 lowerGroup;
    s32 row;

    Ovl11UpdateConditionValueCommand(1, firstSlot, 0, 0);
    base = gOvl11ValueSlots;
    first = (s32 *) ((u8 *) base + (firstSlot * 4));
    row = *first;
    if (row >= 0xA) {
        row = 0;
    }
    lowerGroup = row < 5;
    upperGroup = 0;
    if (lowerGroup == 0) {
        row -= 5;
        upperGroup = 1;
    }
    *first = (s32) *((s16 *) ((u8 *) gOvl11ConditionCodeTriples + (row * 6)));
    *((s32 *) ((u8 *) base + (secondSlot * 4))) = (s32) *((s16 *) ((u8 *) gOvl11ConditionCodeTriples + (row * 6) + 2));
    third = (s32 *) ((u8 *) base + (thirdSlot * 4));
    *third = (s32) *((s16 *) ((u8 *) gOvl11ConditionCodeTriples + (row * 6) + 4));
    if (upperGroup != 0) {
        *first += 5;
        *third += 5;
    }
    return 1;
}

s16 Ovl11GetSelectedPositionRecord(void) {
    return gOvl11SelectedPositionRecord;
}

/* Program-start/load callers explicitly supply four zeros. The reset ignores
 * all four; retain that witnessed contract rather than inventing operands. */
s32 Ovl11ClearValueSlotsCommand(s32 unused0, s32 unused1, s32 unused2, s32 unused3) {
    memset(gOvl11ValueSlots, 0, 0x50);
    return 1;
}
