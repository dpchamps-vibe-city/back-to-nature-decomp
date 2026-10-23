#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800BE208(s32 *arg0, Ovl11CollisionTag arg1, u16 arg2, u16 arg3, void **arg4, s32 *arg5, s32 arg6, s32 arg7);

/* Puts records that 800BE208 reports as colliding back at fixed positions.
 * A record in state 6 is also set back to state 1, without the collision
 * test when its gate value is zero. */
void ovl_11_func_800D0FB4(void) {
    Ovl11ResetRecordB4 *record;
    s32 i;
    s32 flags;

    flags = 0x30;
    record = &((Ovl11Status99C8View *)D_8006C838)->record_91AC;
    if (record->collision.field_2 == 6 && D_80070D0C == 0) {
        record->position[0] = -1800;
        record->position[1] = 0;
        record->position[2] = 3100;
        record->collision.field_2 = 1;
    } else if ((record->collision.field_2 == 1 || record->collision.field_2 == 6) &&
               ovl_11_func_800BE208(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
        if (record->collision.field_2 == 1) {
            record->position[0] = -1800;
            record->position[1] = 0;
            record->position[2] = 3100;
        } else {
            record->position[0] = -1800;
            record->position[1] = 0;
            record->position[2] = 3100;
            record->collision.field_2 = 1;
        }
    }
    record = &((Ovl11Status99C8View *)D_8006C838)->record_929C;
    if (record->collision.field_2 == 1 &&
        ovl_11_func_800BE208(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
        record->position[0] = 5000;
        record->position[1] = 0;
        record->position[2] = 2700;
    }
    for (i = 0; i < 20; i++) {
        record = (Ovl11ResetRecordB4 *)&((Ovl11Status99C8View *)D_8006C838)->records_81BC[i];
        if (record->collision.field_2 == 1 &&
            ovl_11_func_800BE208(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
            record->position[0] = i * 400 - 1000;
            record->position[1] = 0;
            record->position[2] = 2200;
        }
    }
    for (i = 0; i < 10; i++) {
        record = &((Ovl11Status99C8View *)D_8006C838)->records_7AB4[i];
        if (record->collision.field_2 == 6 && ((Ovl11Status99C8View *)D_8006C838)->field_44D4 == 0) {
            record->position[0] = -5000;
            record->position[1] = 0;
            record->position[2] = i * 400 - 7600;
            record->collision.field_2 = 1;
        } else if ((record->collision.field_2 == 1 || record->collision.field_2 == 6) &&
                   ovl_11_func_800BE208(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
            if (record->collision.field_2 == 1) {
                record->position[0] = -5000;
                record->position[1] = 0;
                record->position[2] = i * 400 - 7600;
            } else {
                record->position[0] = -5000;
                record->position[1] = 0;
                record->position[2] = i * 400 - 7600;
                record->collision.field_2 = 1;
            }
        }
    }
}
