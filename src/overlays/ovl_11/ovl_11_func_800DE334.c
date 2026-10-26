#include "common.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"
#include "ovl11_hour_counter_adjustment.h"
#include "ovl11_record_pool_lifecycle.h"

void ovl_11_func_8010C95C(void);

s32 ovl_11_func_800DE334(void) {
    Ovl11RunRecordPoolFirstPass();
    ovl_11_func_8010C95C();
    Ovl11ApplyHourCounterDeltas();
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0xC) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0x12) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    return 1;
}
