/* State-record commands: position writes, animation selection/polling, flag
 * 0x800 changes and graded resets of matching records. The writer and reset
 * driver share the adjacent animation/flag operations through the state-field
 * resolver. See ovl11_state_record_commands.h and notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_state_record_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
#include "sprite_source.h"
#include "ovl11_state_object_pool_clear.h"

s32 Ovl11WriteStateRecordPosition(s16 recordId, s16 y, s32 x, s32 z) {
    u8 *position;

    if (recordId >= 0x32) {
        position = &D_80077600[recordId * 0xF8];
    } else {
        Ovl11ChangeStateRecordFlag800(recordId, 0, 0, 0);
        position = Ovl11ResolveStateField(0x22, recordId, NULL);
    }
    *(s32 *)position = x;
    *(s32 *)(position + 8) = z;
    *(s32 *)(position + 4) = y;
    return 1;
}

s32 Ovl11StateRecordAnimationCommand(s16 recordId, s16 animation, s32 frame, s32 forceFrame) {
    u8 *sprite;

    sprite = Ovl11ResolveStateField(0x24, recordId, NULL);
    if (forceFrame == 0) {
        if (*(u8 *)(sprite + 4) != animation) {
            SelectSpriteAnimation((ObjectState *)sprite, animation & 0xFF);
            return 0;
        }
        if ((*(u16 *)(sprite + 2) & 0x300) != 0) {
            ((Ovl11AnimationCommandWork *)&D_8006C838)->flags &= ~0x100000;
            return 1;
        }
        return 0;
    }
    if (*(u8 *)(sprite + 4) != animation) {
        SelectSpriteAnimation((ObjectState *)sprite, animation & 0xFF);
    }
    SelectSpriteAnimationFrame((ObjectState *)sprite, frame & 0xFF);
    return 1;
}

s32 Ovl11ChangeStateRecordFlag800(s16 recordOrSlot, s16 clearFlag, s32 fromValueSlot, s32 unused) {
    s32 recordId;
    u8 *flags;

    if (recordOrSlot != 0x2C) {
        if (fromValueSlot != 0) {
            recordId = gOvl11ValueSlots[recordOrSlot];
        } else {
            recordId = recordOrSlot;
        }
        flags = Ovl11ResolveStateField(0x25, recordId, NULL);
        if (clearFlag == 0) {
            *(u16 *)flags |= 0x800;
        } else {
            *(u16 *)flags &= 0xF7FF;
        }
    } else {
        Ovl11ClearEligibleStateObjects(0);
    }
    return 1;
}

s32 ovl_11_func_800E7504(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 Ovl11ResetMatchingStateRecords(s16 recordId, s16 requestedMatch, s32 resultSlot, s32 resetDepth) {
    s16 nextRecord;
    s16 matchValue;
    s16 currentValue;
    s32 nextFixed;
    s32 fixedStep;
    s32 previousFixed;
    s32 result;
    Ovl11RecordResetCommandWork *record;

    if (recordId == 0x2A) {
        nextRecord = 1;
        nextFixed = 0x20000;
        fixedStep = 0x10000;
        do {
            Ovl11ResetMatchingStateRecords(nextRecord, requestedMatch, resultSlot, resetDepth);
            previousFixed = nextFixed;
            nextFixed += fixedStep;
            nextRecord = (s16) (previousFixed >> 0x10);
        } while (nextRecord < 0x25);
        return 1;
    }
    if (requestedMatch == -1) {
        matchValue = ((Ovl11RecordResetCommandWork *)D_8006C838)->defaultMatchValue;
    } else {
        matchValue = requestedMatch;
    }
    if (resultSlot != -1) {
        gOvl11ValueSlots[resultSlot] = (s32) ((Ovl11RecordResetCommandWork *)((u8 *)D_8006C838 + recordId * 0x1D4))->matchValue;
    }
    {
        Ovl11RecordResetCommandWork *work = (Ovl11RecordResetCommandWork *)D_8006C838;
        record = (Ovl11RecordResetCommandWork *)((u8 *)work + recordId * 0x1D4);
    }
    currentValue = record->matchValue;
    result = 0;
    if (currentValue == matchValue) {
        if (resetDepth > 0) {
            ovl_11_func_800E7504(recordId, -1, -1, -1);
            result = 1;
            if (resetDepth >= 2) {
                Ovl11ChangeStateRecordFlag800(recordId, 0, 0, 0);
                result = 1;
                if (resetDepth >= 3) {
                    Ovl11StateRecordAnimationCommand(recordId, 0, (s32) record->animationFrame, 1);
                    result = 1;
                }
            }
            return result;
        }
        return 1;
    }
    return result;
}
