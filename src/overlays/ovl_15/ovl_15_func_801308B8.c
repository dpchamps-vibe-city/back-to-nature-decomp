#include "common.h"
#include "game_types.h"

void func_8001AC10(u32 *arg0, s32 arg1, s32 arg2);
s32 ovl_15_func_80136288(s16 arg0, s16 arg1, Ovl15SaveSummary *arg2);
s32 ovl_15_func_80135FF0(s16 arg0, s16 arg1);

void ovl_15_func_801308B8(void) {
    s16 *cardStates;
    s32 slot;
    s32 text;
    s32 status;
    s32 fileStatus;

    text = gActiveRenderContext->field_D8;
    func_8001AC10((u32 *) (text + 0x18), text + 0x14, D_80054BBC[0] + (s32) D_80052FC4);
    cardStates = D_80137594;
    for (slot = 0; slot < 3; slot++) {
        D_80137820[slot] = 2;
        status = ovl_15_func_80136288(D_8013758C, (s16) slot, (Ovl15SaveSummary *) &D_801376E0_headers[slot]);
        if (status == 8) {
            D_80137584 = 6;
            return;
        }
        if ((u32) ((status - 4) & 0xFFFF) < 2U) {
            D_80137584 = 0x10;
            return;
        }
        if (status == 9) {
            fileStatus = ovl_15_func_80135FF0(D_8013758C, (s16) slot);
            if (fileStatus == 0) {
                D_80137820[slot] = 1;
            } else if (fileStatus == 0xF) {
                D_80137820[slot] = 3;
            } else {
                D_80137820[slot] = 2;
            }
        } else if (status == 0) {
            D_80137820[slot] = 0;
        }
        if ((u32) ((u16) cardStates[D_8013758C] - 6) < 2U) {
            D_80137820[slot] = 1;
        }
    }
    if (D_80137820[0] != 0 && D_80137820[1] != 0 && D_80137820[2] != 0) {
        D_80137584 = 8;
    } else {
        D_80137587 = 0;
        D_80137584 = 9;
    }
}
