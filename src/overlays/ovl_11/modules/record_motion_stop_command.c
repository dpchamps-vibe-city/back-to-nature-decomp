/*
 * Stop a state record's motion, ovl_11:800EAE24 (opcode entry).
 * mode 0/1 forwards animation/frame/mode to the animation command. Mode 2
 * chooses frame 0/1/2/3 from vector -Z/-X/+Z/+X, in that priority, forcing
 * the frame; a zero X/Z vector leaves animation unchanged. Mode 3 orients
 * toward the shared work position using animation as the animation operand.
 * Every mode, including unknown modes, then clears the field-23 vector.
 * Return 1 without waiting for animation. Record IDs and resolved pointers
 * are unchecked; position/vector units are not established. No C callers.
 * Membership/boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_state_field_access.h"
#include "ovl11_direction_vector.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_pending_record_animation.h"

s32 Ovl11StopRecordMotionCommand(s16 recordId, s16 animation, s32 frame, s32 mode) {
    s32 x;
    s32 z;
    u8 *vector;

    vector = Ovl11ResolveStateField(0x23, (s32) recordId, NULL);
    switch (mode) {
    case 0:
    case 1:
        Ovl11StateRecordAnimationCommand(recordId, animation, frame, mode);
        break;
    case 2:
        z = *(s32 *) ((u8 *) vector + 8);
        if (z < 0) {
            Ovl11StateRecordAnimationCommand(recordId, animation, 0, 1);
        } else {
            x = *(s32 *) ((u8 *) vector + 0);
            if (x < 0) {
                Ovl11StateRecordAnimationCommand(recordId, animation, 1, 1);
            } else if (z > 0) {
                Ovl11StateRecordAnimationCommand(recordId, animation, 2, 1);
            } else if (x > 0) {
                Ovl11StateRecordAnimationCommand(recordId, animation, 3, 1);
            }
        }
        break;
    case 3:
        Ovl11OrientAndAnimateStateRecord(recordId, 0, animation, 0);
        break;
    }
    Ovl11SetDirectionVector(4, (Recon800D0408A1View *) vector, 0);
    return 1;
}
