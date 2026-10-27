#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"
#include "ovl11_condition_evaluation.h"
#include "ovl11_object_event_slot_flags_command.h"
#include "ovl11_clock_advance_command.h"


u16 ovl_11_func_800EAF5C (s16 arg0, s32 arg1, s32 arg2, s32 arg3);
s32 Ovl11QueueConditionResultAnySlot (s32 arg0, s32 arg1);

struct struct_8006C838_800E7798 {
    char pad_000[0x4450];
    s32 field_4450;
    char pad_4454[0x5234 - 0x4454];
    s32 field_5234;
    char pad_5238[0x7A74 - 0x5238];
    s32 field_7A74;
};

s32 ovl_11_func_800E7798(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 var_v0;

    switch (arg0) {
    case 1: {
        char *far_base = (char *)&D_8007AFF0;

        (*(s32 *)(far_base + 0x254A0)) = 0xC;
        ((struct struct_8006C838_800E7798 *)D_8006C838)->field_5234 |= 0x400;
        SetGameFlag(0x47U, 1U);
        break;
    }
    case 2:
        Ovl11ClockAdvanceCommand(arg1, 0, 1, 0);
        break;
    case 3:
        Ovl11ClockAdvanceCommand(arg1, 0, 0, 0);
        break;
    default:
        break;
    }
    var_v0 = arg3 & 2;
    if (var_v0 != 0) {
        Ovl11ClearObjectEventSlotFlags(0x2A, 2, 0, 0);
    }
    ovl_11_func_800EAF5C(2, arg3 & 1, 0, 0);
    if (arg2 != 0) {
        ((struct struct_8006C838_800E7798 *)D_8006C838)->field_4450 |= 8;
    }
    ((struct struct_8006C838_800E7798 *)D_8006C838)->field_7A74 = 2;
    return 1;
}
