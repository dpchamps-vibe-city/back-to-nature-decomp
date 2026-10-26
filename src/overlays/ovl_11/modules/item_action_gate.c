/*
 * item_action_gate — reconstructed translation unit.
 *
 * Classify the cell under the primary object and gate item-triggered state
 * changes.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_item_action_gate.h"
#include "game_types.h"
#include "sound_commands.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_grid_position.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_primary_object_sprite_slots.h"

/* Leave a selected cell pointer (or NULL) as well as returning its class:
 * 0 = empty/tag 167..169, 2 = other tag, 3 = unavailable/mode mismatch. */
s32 Ovl11ProbeObjectCellClass(Ovl11CE034Object *object) {
    Ovl11CE034Position position;
    u16 column;
    u16 row;
    u16 tag;
    s16 *farWork;
    s16 mode;

    Ovl11ComputeGridOffsetPosition(object, &position, 0);
    gOvl11ProbedObjectCell = NULL;
    farWork = (s16 *)&D_8007AFF0;
    mode = farWork[0x12A3B];
    if (mode == 1) {
        if (Ovl11LookupMode1GridPosition(&position.unk0, &column, &row) != 0) {
            return 3;
        }
        gOvl11ProbedObjectCell = (u16 *)&gMainAreaGrid[(s16)row][(s16)column];
        goto classify;
    }
    if (mode == 6) {
        if (Ovl11LookupMode6GridPosition((s32 *)&position, &column, &row) != 0) {
            goto outside;
        }
        goto alternate;
    }
    goto classify;
outside:
    return 3;
alternate:
    gOvl11ProbedObjectCell = (u16 *)&gAlternateAreaGrid[(s16)row][(s16)column];
classify:
    if (gOvl11ProbedObjectCell != NULL) {
        tag = *gOvl11ProbedObjectCell;
        if ((u32)(tag - 0x167) >= 3U && tag != 0) {
            return 2;
        }
        return 0;
    }
    goto outside;
}

s32 ovl_11_func_800D736C(s16 *itemId);

s32 Ovl11TryBeginItemAction(s32 inputMode) {
    Ovl11C7010Object *obj;
    ItemData *item;
    s32 result;
    u16 allow;
    s32 itemStatus;
    u16 flags;

    obj = (Ovl11C7010Object *)&gPrimaryObjectBase;
    if (inputMode == 0) {
        result = Ovl11ProbeObjectCellClass((Ovl11CE034Object *)obj);
        item = &(*(ItemData **)((u8 *)obj - 0x51A8))[obj->itemIndex];
        if (obj->itemIndex == 0x65) {
            if (gOvl11InteractionHitFlags != 0 && result != 0) {
                return 0;
            }
            goto common_action;
        } else if (obj->itemIndex == 0x64) {
            if (D_80128C50 & 0xC087) {
                return 0;
            }
            if (gOvl11InteractionHitFlags != 0) {
                return 0;
            }
            Ovl11ChangePrimaryObjectState(obj, 0x62, obj->unk38 + 0x17, -2);
            return 1;
        } else if (obj->itemIndex == 0x84) {
            if (gOvl11InteractionHitFlags != 0 && result != 0) {
                return 0;
            }
            goto common_action;
        } else {
            flags = item->u0.flags;
            if (flags & 0x100) {
                if (D_80128C50 & 0xC087) {
                    return 0;
                }
                if (gOvl11InteractionHitFlags == 0 || result == 0) {
                    Ovl11ChangePrimaryObjectState(obj, 9, obj->unk38 + 0x17, -2);
                    return 1;
                }
            }
            flags = item->u0.flags;
            if ((flags & 0x9000) == 0x9000 || (itemStatus = *(s32 *)((u8 *)item + 0x14), itemStatus == -1)) {
                allow = 0;
                if (gOvl11InteractionHitFlags == 0) {
                    allow = 1;
                } else {
                    if (gOvl11InteractionHitFlags & 0x8000) {
                        if (((u16 *)gOvl11InteractionHitRecordView)[1] == 0x64 && item->field_12 != 0) {
                            Ovl11ChangePrimaryObjectState(obj, 0x6F, obj->unk38 + 0x17, -2);
                            return 1;
                        }
                        allow = 1;
                    }
                    if (gOvl11InteractionHitFlags & 0x200) {
                        allow = ((Ovl11CB850State *)D_8006C838)->selectedRecord == 0x15F;
                    }
                }
                if (allow) {
                    goto common_action;
                }
                itemStatus = *(s32 *)((u8 *)item + 0x14);
            }
            if (itemStatus & 0x10) {
                if (result == 0 && gOvl11InteractionHitFlags == 0) {
                    Ovl11ChangePrimaryObjectState(obj, 0x38, obj->unk38 + 0x17, -2);
                    return 1;
                }
                if (result == 3 && gOvl11InteractionHitFlags == 0) {
                    goto common_action;
                }
                if (result == 2) {
                    return 1;
                }
            }
            if (gOvl11InteractionHitFlags != 0 || result != 2) {
                return 0;
            }
        }
common_action:
        Ovl11ChangePrimaryObjectState(obj, 0x2F, obj->unk38 + 0x17, -2);
        return 1;
    }
    if (obj->itemIndex == 0x64) {
        obj->unk6C |= 0x1800;
        Ovl11ChangePrimaryObjectState(obj, 0x46, 0, -2);
        SelectSpriteAnimation((ObjectState *)&obj->sprite, (u8)obj->unk38);
        obj->unk6C |= 0x4000;
        return 1;
    }
    switch ((u32)ovl_11_func_800D736C(&obj->itemIndex)) {
    case 1:
        return 1;
    case 3:
        Ovl11ChangePrimaryObjectState(obj, 3, 5, -2);
        PlaySoundEffect(0x4A);
        return 1;
    case 4:
        obj->unk6C |= 0x5800;
        Ovl11ChangePrimaryObjectState(obj, 0x49, 0, -2);
        PlaySoundEffect(0x4B);
        return 1;
    }
    return 0;
}
