/*
 * State-record endpoint/continuation commands. The resolved position (field
 * 0x22) and increment (0x23) are separate three-word X/Y/Z records. The local
 * animation facade uses magnitude 12; callers may use the core independently.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_state_field_access.h"
#include "ovl11_direction_vector.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_state_record_motion_limit.h"

s32 Ovl11LimitStateRecordMotion(s16 recordId, s16 direction,
                               s32 limit, s32 magnitude) {
    s16 selector;
    s32 reached;
    u8 *position;
    u8 *increment;

    position = Ovl11ResolveStateField(0x22, (s32)recordId, NULL);
    increment = Ovl11ResolveStateField(0x23, (s32)recordId, NULL);
    selector = direction + 2;
    reached = 0;
    switch (selector) {
    case 2:
        if (limit >= (*(s32 *)((u8 *)position + 8))) {
            (*(s32 *)((u8 *)position + 8)) = limit;
            reached = 1;
        }
        break;
    case 3:
        if (limit >= (*(s32 *)((u8 *)position + 0))) {
            (*(s32 *)((u8 *)position + 0)) = limit;
            reached = 1;
        }
        break;
    case 4:
        if ((*(s32 *)((u8 *)position + 8)) >= limit) {
            (*(s32 *)((u8 *)position + 8)) = limit;
            reached = 1;
        }
        break;
    case 5:
        if ((*(s32 *)((u8 *)position + 0)) >= limit) {
            (*(s32 *)((u8 *)position + 0)) = limit;
            reached = 1;
        }
        break;
    case 1:
        if (limit >= (*(s32 *)((u8 *)position + 4))) {
            (*(s32 *)((u8 *)position + 4)) = limit;
            reached = 1;
        }
        break;
    case 0:
        if ((*(s32 *)((u8 *)position + 4)) >= limit) {
            (*(s32 *)((u8 *)position + 4)) = limit;
            reached = 1;
        }
        break;
    }
    if (reached != 0) {
        Ovl11SetDirectionVector(4, (Recon800D0408A1View *)increment, 0);
        return 1;
    }
    Ovl11SetDirectionVector((u16)direction,
                            (Recon800D0408A1View *)increment, magnitude * 2);
    return 0;
}

s32 Ovl11AnimateStateRecordMotion12(s16 recordId, s16 direction,
                                   s32 limit, s16 animation) {
    Ovl11StateRecordAnimationCommand(recordId, animation, 0, 0);
    return Ovl11LimitStateRecordMotion(recordId, direction, limit, 12);
}
