#include "common.h"
#include "game_types.h"
#include "ovl23_record_control_fields.h"

extern Ovl23D87CView39C gOvl23RecordRunWork;

void ovl_23_func_800B9A00(Ovl23Func800BB0C8Arg *arg0) {
    s32 temp_a0;
    s32 temp_s0;
    s32 temp_v1;
    s32 temp_v1_2;

    temp_a0 = *(s32 *)((u8 *)arg0 + 0xC);
    temp_s0 = (gOvl23RecordRunWork.unk39C + 0x77) << 12;
    if (temp_a0 < temp_s0) {
        Ovl23SetAnchorRecordControl(arg0, 2, 2, 0);
        temp_v1 = (*(s32 *)((u8 *)arg0 + 0xC)) + ((gOvl23RecordRunWork.unk3A0 << 12) / 10);
        *(s32 *)((u8 *)arg0 + 0xC) = temp_v1;
        if (temp_s0 < temp_v1) {
            *(s32 *)((u8 *)arg0 + 0xC) = temp_s0;
        }
    } else if (temp_s0 < temp_a0) {
        Ovl23SetAnchorRecordControl(arg0, 2, 6, 0);
        temp_v1_2 = (*(s32 *)((u8 *)arg0 + 0xC)) - ((gOvl23RecordRunWork.unk3A0 << 12) / 10);
        *(s32 *)((u8 *)arg0 + 0xC) = temp_v1_2;
        if (temp_v1_2 < temp_s0) {
            *(s32 *)((u8 *)arg0 + 0xC) = temp_s0;
        }
    }
    if ((*(s32 *)((u8 *)arg0 + 0xC)) == temp_s0) {
        Ovl23SetAnchorRecordControl(arg0, 1, 6, 0);
    }
}
