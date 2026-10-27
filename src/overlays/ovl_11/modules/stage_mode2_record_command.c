/* Object-program adapter for the singleton stage-mode-2 record lifecycle.
 * Original identity: ovl_11:800E8760. The allocation/clear services retain
 * their own modules; this entry adds only the command's control/result ABI.
 */
#include "common.h"
#include "ovl11_stage_mode2_record_command.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_stage_mode2_record.h"

s32 Ovl11LoadOrClearStageMode2Record(s32 control) {
    /* Test the low halfword while retaining the command's word-sized ABI. */
    if ((control << 0x10) == 0) {
        Ovl11AllocateItemRecord(0x15E, -1);
    } else {
        Ovl11ClearStageMode2Record((s32)&gStageMode2Record);
    }
    return 1;
}
