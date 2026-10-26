#include "common.h"
#include "game_types.h"
#include "ovl23_record_control_fields.h"


void ovl_23_func_800BA610(Ovl23Func800BB0C8Arg *arg0) {
    s32 temp_a0;
    s32 temp_v1;

    switch (arg0->unk4) {
    case 0:
        return;
    case 2:
        temp_a0 = (Ovl23_D_800BF87C_bounds.unk3A0 << 0xC) / 10;
        break;
    case 3:
        temp_a0 = (Ovl23_D_800BF87C_bounds.unk3A2 << 0xC) / 10;
        break;
    default:
        return;
    }

    if (arg0->unk6 == 6) {
        *(s32 *)((u8 *)arg0 + 0xC) -= temp_a0;
    } else {
        *(s32 *)((u8 *)arg0 + 0xC) += temp_a0;
    }
    temp_v1 = (Ovl23_D_800BF87C_bounds.unk39C + 0x77) << 0xC;
    if (*(s32 *)((u8 *)arg0 + 0xC) >= temp_v1) {
        *(s32 *)((u8 *)arg0 + 0xC) = temp_v1;
        if (*(s16 *)((u8 *)arg0 + 0) != 2) {
            Ovl23SetAnchorRecordControl(arg0, 1, 6, arg0->unk8);
        }
    }
    if (*(s32 *)((u8 *)arg0 + 0xC) <= -0x30000) {
        *(s32 *)((u8 *)arg0 + 0xC) = -0x30000;
        if (*(s16 *)((u8 *)arg0 + 0) != 2) {
            Ovl23SetAnchorRecordControl(arg0, 1, 2, arg0->unk8);
        }
    }
}
