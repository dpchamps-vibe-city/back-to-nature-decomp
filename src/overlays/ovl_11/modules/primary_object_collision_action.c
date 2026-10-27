/*
 * Directional primary-object collision/action query and hit publication.
 * The header-selected output view preserves the original declaration identity
 * of the shared hit-flags word; it must precede common.h.
 * See ovl11_primary_object_collision_action.h and notes/file-groupings.md.
 */
#define OVL11_COLLISION_OUTPUT_VIEW
#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_collision_action.h"
#include "psyq/stddef.h"
#include "ovl11_collision_query_budget.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_tagged_record_handlers.h"

s32 ovl_11_func_800DF010(void *, s32);
s32 ovl_11_func_8010B57C(u16 *, s32);

s32 Ovl11QueryPrimaryObjectCollisionAction(Ovl11CB114Object *object, s32 *center, s16 probeIndex) {
    s32 position[4];
    void *hit;
    s32 hitFlags;
    s32 result;
    s32 originalFlags;
    s32 contactFlags;
    s32 releasedFlags;
    s32 contactAction;
    s32 queryMask;
    s16 *offset;

    hitFlags = 0;
    hit = NULL;
    position[1] = center[1];
    offset = &Ovl11CollisionOffsets[probeIndex + object->direction * 2];
    position[0] = center[0] + offset[0];
    position[2] = center[2] + offset[1];
    originalFlags = object->flags;
    object->flags = (s32) (originalFlags & 0xFFFBFFFF);
    queryMask = 0x8FAE;
    if (originalFlags & 0x100) {
        queryMask = 0x8DAE;
    }
    if (gOvl11PrimaryHeldButtons & 0x20) {
        queryMask &= ~0x80;
    }
    result = Ovl11QueryCollisionBudgeted(&position[0], object->collision, 0x3E8U, 0x3E8U, &hit, &hitFlags, 0, queryMask) * 2;
    Ovl11CollisionFlags = 0;
    gOvl11InteractionHitRecord = NULL;
    if ((probeIndex != 0) && (probeIndex != 0x38)) {
        if (hitFlags & 0x10000) {
            contactFlags = object->flags;
            contactAction = 1;
            if (!(contactFlags & 0x20000010)) {
                if (!(contactFlags & 0x18000000)) {
                    contactAction = 2;
                }
            } else {
                contactAction = 0;
            }
            if (object->mode == 4 || object->mode == 2) {
                contactAction = 2;
            }
            if (object->mode == 0xE || object->mode == 6) {
                contactAction = 0;
            }
            if (contactAction != 0) {
                if (contactAction == 2) {
                    if (!(object->flags & 0x200000)) {
                        Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 1U, 0, -2);
                    }
                    object->flags = (s32) (object->flags | 0x200000);
                }
                if (hit != NULL) {
                    if (hitFlags & 2) {
                        if ((u32) gOvl11InteractionDispatchCount >= 0x33U) {
                            ovl_11_func_800DF010(hit, 0xD);
                        } else {
                            gOvl11InteractionDispatchCount += 1;
                        }
                    }
                    if (hitFlags & 4) {
                        if ((u32) gOvl11InteractionDispatchCount >= 0x33U) {
                            Ovl11DispatchTaggedRecordHandler(hit, 0xD);
                        } else {
                            gOvl11InteractionDispatchCount += 1;
                        }
                    }
                    if (hitFlags & 0x200) {
                        if ((u32) gOvl11InteractionDispatchCount >= 0x33U) {
                            ovl_11_func_8010B57C((u16 *) hit, 0xD);
                        } else {
                            gOvl11InteractionDispatchCount += 1;
                        }
                    }
                }
            } else {
                hitFlags |= 0x4000;
            }
        } else {
            releasedFlags = object->flags;
            if (releasedFlags & 0x200000) {
                object->flags = (s32) ((releasedFlags & 0xFFDFFFFF) | 0x20000);
            }
        }
        if (hitFlags & 0x2000) {
            object->flags = (s32) (object->flags | 0x40000);
            Ovl11CollisionFlags = hitFlags;
            gOvl11InteractionHitRecord = hit;
        }
        Ovl11CollisionFlags = hitFlags;
        gOvl11InteractionHitRecord = hit;
    }
    if (hitFlags & 0x4000) {
        result = 4;
        if (probeIndex == 0x38) {
            result = 0x3E7;
            Ovl11CollisionFlags = hitFlags;
            gOvl11InteractionHitRecord = hit;
        }
    }
    if (object->flags & 8) {
        return 0;
    }
    if (object->flags & 0x80) {
        object->flags &= ~0x220000;
        if (object->mode == 0x39) {
            return 0;
        }
        if (object->mode == 0x3A) {
            return 0;
        }
    }
    return (s16)result;
}
