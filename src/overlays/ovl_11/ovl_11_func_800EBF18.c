#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "ovl11_state_field_access.h"
#include "psyq/abs.h"
#include "ovl11_object_events.h"

s32 ovl_11_func_800EBF18(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s16 i;

    for (i = 1; i < 0x25; i++) {
        if (i != 0x1B && i != 0x21) {
            if ((u32)(arg0 - 0x1F) < 2U) {
                if (Ovl11IsSpecialCounterSlot(i) == 1) {
                    Ovl11UpdateStateField(0x1F, i, arg3, arg1);
                } else {
                    Ovl11UpdateStateField(0x20, i, arg2, arg1);
                }
            } else {
                Ovl11UpdateStateField(arg0, i, arg2, arg1);
            }
        }
    }
    if ((u16)(arg0 - 0x1F) < 2U) {
        for (i = 0; i < 7; i++) {
            Ovl11UpdateStateField(0x2B, i, arg2, arg1);
        }
    }
    return 1;
}
