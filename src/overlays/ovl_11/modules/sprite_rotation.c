/*
 * Ten registered sprite rotations: accelerate outward, hold, and return.
 * Each state references a source object; an optional paired sprite receives
 * a mirrored yaw. Sources are borrowed, not copied or owned by this unit.
 *
 * Public contracts and lookup preconditions: sprite_rotation.h.
 * Original membership and boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "sprite_rotation.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/stdlib.h"

void ovl_11_func_800E54C8(void);
void ovl_11_func_800DCC1C(s16 sprite, void *rotation);
s32 func_8001FABC(s16 soundId);

/* Private views of the externally supplied sprite pool, not new storage. */
typedef struct {
    char pad0[0x10];
    s32 flags;
    s32 unk14;
    s16 linkedSprite;
    char pad1A[0x2C - 0x1A];
} RotationSpriteView;

typedef struct {
    char pad0[0x938];
    RotationSpriteView sprites[50];
} RotationPoolView;

void Ovl11ResetSpriteRotationEntry(Ovl11RotationState *entry) {
    entry->rotation.x = 0;
    entry->rotation.y = 0;
    entry->rotation.z = 0;
    entry->angularStep = 0;
    entry->holdTicks = 0;
    entry->direction = 4;
    entry->state = OVL11_ROTATION_FREE;
    entry->source = 0;
}

void Ovl11ResetSpriteRotations(void) {
    Ovl11RotationState *entry = gOvl11SpriteRotations;
    u32 i;

    for (i = 0; i < 10U; i++) {
        Ovl11ResetSpriteRotationEntry(entry);
        entry++;
    }
    ovl_11_func_800E54C8();
}

void Ovl11RegisterSpriteRotation(Ovl11RotationSource *source) {
    Ovl11RotationState *entry = gOvl11SpriteRotations;
    u32 i;

    i = 0;
    do {
        if (entry->state == OVL11_ROTATION_FREE) {
            Ovl11ResetSpriteRotationEntry(entry);
            entry->state = OVL11_ROTATION_IDLE;
            entry->source = source;
            entry->direction = source->direction;
            break;
        }
        i++;
        entry++;
    } while (i < 10U);
}

void Ovl11RegisterPoolSpriteRotations(s32 poolAddress) {
    u32 i;
    s32 sentinel;
    u8 *source;
    u8 *id;

    i = 0;
    sentinel = 0x63;
    source = (u8 *)poolAddress + 0x10;
    id = (u8 *)poolAddress + 0x1A;
    do {
        if (*(s16 *)id != sentinel) {
            Ovl11RegisterSpriteRotation((Ovl11RotationSource *)source);
        }
        source += 0x2C;
        i++;
        id += 0x2C;
    } while (i < 0x32U);
}

s32 Ovl11RequestSpriteRotation(s16 id, u16 direction) {
    Ovl11RotationState *entry;
    u32 i;
    s32 state;

    entry = gOvl11SpriteRotations;
    for (i = 0; i < 10; i++, entry++) {
        if (entry->source->id != id) {
            continue;
        }
        state = entry->state;
        switch (state) {
        case OVL11_ROTATION_IDLE:
            if (!(entry->source->flags & 0x200)) {
                func_8001FABC(0x16);
            }
            /* fallthrough */
        case OVL11_ROTATION_OUTBOUND:
        case OVL11_ROTATION_RETURN:
            if (entry->source->flags & 0x200) {
                entry->state = OVL11_ROTATION_IDLE;
                return 1;
            } else {
                entry->state = OVL11_ROTATION_OUTBOUND;
                entry->direction = direction;
                return 1;
            }
        case OVL11_ROTATION_HOLD:
            entry->holdTicks += 5;
            return 1;
        default:
            continue;
        }
    }
    return 0;
}

void Ovl11StepSpriteRotation(Ovl11RotationState *entry) {
    Ovl11RotationVector mirrored;
    s16 sprite;
    s16 linkedSprite;
    s32 paired;
    s32 negativeDirection;
    s32 negatedYaw;
    s32 angle;
    s32 direction;
    u16 component;
    u16 state;
    RotationPoolView *pool;

    sprite = entry->source->sprite;
    pool = (RotationPoolView *)((struct Ovl11SpriteWork *)&D_8007AFF0)->header;
    paired = pool->sprites[sprite].flags & 0x400;
    direction = entry->direction - 1;
    negativeDirection = (u32)direction < 2U;
    if (paired != 0) {
        linkedSprite = pool->sprites[sprite].linkedSprite;
    } else {
        linkedSprite = -1;
    }
    state = entry->state;
    if (state == OVL11_ROTATION_OUTBOUND) {
        entry->angularStep += 8;
    }
    if ((u16)entry->state == OVL11_ROTATION_RETURN) {
        entry->angularStep = (s32)(entry->angularStep - 8);
    }
    if (negativeDirection != 0) {
        entry->rotation.y -= entry->angularStep;
    } else {
        entry->rotation.y += entry->angularStep;
    }
    ovl_11_func_800DCC1C(sprite, &entry->rotation.x);
    if (paired != 0) {
        s32 y;

        /* Keep the halfword copies separate from the negated signed yaw. */
        component = (u16)entry->rotation.x;
        mirrored.x = component;
        y = entry->rotation.y;
        negatedYaw = -y;
        mirrored.y = negatedYaw;
        component = (u16)entry->rotation.z;
        mirrored.z = component;
        ovl_11_func_800DCC1C(linkedSprite, &mirrored);
    }
    if (entry->state == OVL11_ROTATION_OUTBOUND) {
        angle = abs(entry->rotation.y);
        if (angle >= 0x420) {
            entry->holdTicks = 0xB;
            entry->state = OVL11_ROTATION_HOLD;
            entry->angularStep = 0;
            /* The snapped hold angle is applied to the sprite on a later
             * step, not by a second setter call during this transition. */
            entry->rotation.y = (u16)gOvl11RotationHoldAngles[entry->direction];
            return;
        }
    }
    if (entry->state == OVL11_ROTATION_RETURN) {
        if (negativeDirection != 0) {
            if (entry->rotation.y < 0) {
                return;
            }
        } else if (entry->rotation.y > 0) {
            return;
        }
        func_8001FABC(0x4C);
        entry->state = OVL11_ROTATION_IDLE;
        entry->angularStep = 0;
        entry->rotation.y = 0U;
        ovl_11_func_800DCC1C(sprite, &entry->rotation.x);
        if (paired != 0) {
            ovl_11_func_800DCC1C(linkedSprite, &entry->rotation.x);
        }
    }
}

void Ovl11UpdateSpriteRotations(void) {
    Ovl11RotationState *entry;
    s32 state;
    u32 i;

    entry = gOvl11SpriteRotations;
    if (!(D_8006C844 & 0x08000000)) {
        for (i = 0; i < 10U; i++, entry++) {
            if (entry->state != OVL11_ROTATION_FREE) {
                state = entry->state;
                switch (state) {
                case OVL11_ROTATION_OUTBOUND:
                    Ovl11StepSpriteRotation(entry);
                    break;
                case OVL11_ROTATION_HOLD:
                    entry->holdTicks--;
                    if (entry->holdTicks == 0) {
                        entry->state = OVL11_ROTATION_RETURN;
                    }
                    break;
                case OVL11_ROTATION_RETURN:
                    Ovl11StepSpriteRotation(entry);
                    break;
                }
            }
        }
    }
}

u16 Ovl11GetSpriteRotationState(s32 id) {
    Ovl11RotationState *entry = gOvl11SpriteRotations;
    u32 i;

    for (i = 0; i < 10U; i++) {
        if (entry->source->id == id) {
            return entry->state;
        }
        entry++;
    }
    return 0;
}
