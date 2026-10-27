/* Animation plus limit-directed record motion, at raw magnitude 20.
 * This is the complete adapter, not a second motion integrator. The remote
 * magnitude-12 adapter returns completion; this entry keeps its void contract.
 * Unit/boundary evidence: notes/file-groupings.md.
 */
#include "common.h"
#include "ovl11_state_record_commands.h"
#include "ovl11_state_record_motion_limit.h"

void Ovl11AnimateStateRecordMotion20(s16 recordId, s16 direction, s32 limit, s16 animation) {
    Ovl11StateRecordAnimationCommand(recordId, animation, 0, 0);
    Ovl11LimitStateRecordMotion(recordId, direction, limit, 0x14);
}
