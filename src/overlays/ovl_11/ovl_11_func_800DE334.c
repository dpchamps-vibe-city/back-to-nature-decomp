#include "common.h"
#include "psyq/memory.h"
#include "ovl11_object_event_pool.h"

void ovl_11_func_800CEBC8(void);
void ovl_11_func_8010C95C(void);
void ovl_11_func_800CD45C(void);

s32 ovl_11_func_800DE334(void) {
    ovl_11_func_800CEBC8();
    ovl_11_func_8010C95C();
    ovl_11_func_800CD45C();
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0xC) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    if (((struct struct_8006C838_time *) D_8006C838)->field_44C0 == 0x12) {
        Ovl11ClearEventSlotTimeOrigins();
    }
    return 1;
}
