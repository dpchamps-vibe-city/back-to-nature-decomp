#include "common.h"
#include "cd_audio.h"
#include "ovl11_pending_stage_dispatch.h"

void ovl_11_func_800D2240(void);
void ovl_11_func_800D2160(void);
void ovl_11_func_800D1EB8(void);
void ovl_11_func_800D2334(void);
u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);

void ovl_11_func_800CE834(void) {
    if (!(((struct Ovl11UpdateState *) D_8006C838)->field_10 & 0x2000)) {
        ovl_11_func_800D2240();
        ovl_11_func_800D2160();
        ovl_11_func_800D1EB8();
        ovl_11_func_800D2334();
        Ovl11ProcessPendingStageMode();
        if (((struct Ovl11UpdateWork *) &D_8007AFF0)->field_25476 == 1 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_44C0 >= 0x12 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_44C0 < 0x17 &&
            (((struct Ovl11UpdateState *) D_8006C838)->field_44F8 & 0x01800000) == 0x800000) {
            if (((struct Ovl11UpdateState *) D_8006C838)->field_44C0 == 0x12) {
                if (((struct Ovl11UpdateState *) D_8006C838)->field_44C2 >= 0xA) {
                    ovl_11_func_800CE744(0x15B, -1);
                }
            } else {
                ovl_11_func_800CE744(0x15B, -1);
            }
        }
        if (((struct Ovl11UpdateState *) D_8006C838)->field_99E6 != 0 &&
            GetCdAudioState() == 0 &&
            ((struct Ovl11UpdateState *) D_8006C838)->field_CC == 0) {
            StartCdAudioTrack(((struct Ovl11UpdateState *) D_8006C838)->field_99E6);
            ((struct Ovl11UpdateState *) D_8006C838)->field_99E6 = 0;
        }
    }
}
