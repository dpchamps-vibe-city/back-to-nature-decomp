/*
 * sprite_rotation_commands — reconstructed translation unit.
 *
 * Script-facing sprite rotation request/state command and its raw-ID
 * mode-four discard wrapper.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_sprite_rotation_commands.h"
#include "psyq/stddef.h"
#include "sprite_rotation.h"

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);

s32 Ovl11SpriteRotationCommand(s16 idOrSlot, s16 operation, s32 direction,
                         s32 fromValueSlot) {
    s16 id;
    s32 result;

    if (fromValueSlot != 0) {
        id = *(s16 *)&gOvl11ValueSlots[idOrSlot];
        result = 1;
    } else {
        id = idOrSlot;
        result = 1;
    }
    switch (operation) {
    case 0:
        ovl_11_func_80107C80(id, direction & 0xFFFF, NULL, 1);
        break;
    case 1:
        result = Ovl11GetSpriteRotationState(id) == OVL11_ROTATION_IDLE;
        break;
    case 2:
        result = Ovl11GetSpriteRotationState(id) == OVL11_ROTATION_HOLD;
        break;
    case 3:
        result = Ovl11GetSpriteRotationState(id) == OVL11_ROTATION_OUTBOUND;
        break;
    case 4:
        result = 0;
        if (Ovl11GetSpriteRotationState(id) != OVL11_ROTATION_FREE) {
            result = Ovl11GetSpriteRotationState(id) != OVL11_ROTATION_IDLE;
        }
        break;
    }
    return result;
}

void Ovl11DiscardSpriteRotationActiveQuery(s16 id) {
    Ovl11SpriteRotationCommand(id, 4, 0, 0);
}
