/* Slot-sourced state-record position command. The three operands select
 * full-word value slots in x/y/z order; unlike the direct position command,
 * this entry always uses resolver field 0x22. See the shared command header.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "ovl11_state_field_access.h"
#include "ovl11_state_record_commands.h"

s32 Ovl11WriteStateRecordPositionFromSlots(s16 recordId, s16 ySlot,
                                         s32 xSlot, s32 zSlot) {
    u8 *position;

    Ovl11ChangeStateRecordFlag800(recordId, 0, 0, 0);
    position = Ovl11ResolveStateField(0x22, recordId, NULL);
    *(s32 *)position = gOvl11ValueSlots[xSlot];
    *(s32 *)(position + 4) = gOvl11ValueSlots[ySlot];
    *(s32 *)(position + 8) = gOvl11ValueSlots[zSlot];
    return 1;
}
