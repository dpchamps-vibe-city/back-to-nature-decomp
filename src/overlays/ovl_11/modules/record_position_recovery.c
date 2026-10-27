/* Restore colliding or gated records to fixed raw-coordinate placements.
 * Keep both state-1/state-6 store arms: state 6 also transitions to state 1.
 * This batch shares the collision budget with the rest of the frame; see
 * ovl11_record_position_recovery.h for the null-output budget constraint. */

#include "common.h"
#include "game_types.h"
#include "ovl11_record_position_recovery.h"
#include "psyq/stddef.h"
#include "ovl11_collision_query_budget.h"

/* A zero gate restores state-6 records without querying collision. Other
 * states outside 1/6 (and outside 1 for the two middle pools) are untouched. */
void Ovl11RecoverRecordPositions(void) {
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
               Ovl11QueryCollisionBudgeted(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
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
        Ovl11QueryCollisionBudgeted(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
        record->position[0] = 5000;
        record->position[1] = 0;
        record->position[2] = 2700;
    }
    for (i = 0; i < 20; i++) {
        record = (Ovl11ResetRecordB4 *)&((Ovl11Status99C8View *)D_8006C838)->records_81BC[i];
        if (record->collision.field_2 == 1 &&
            Ovl11QueryCollisionBudgeted(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
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
                   Ovl11QueryCollisionBudgeted(record->position, record->collision, 0, 0, NULL, NULL, 0, flags)) {
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
