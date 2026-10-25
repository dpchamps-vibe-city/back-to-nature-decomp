#include "common.h"
#include "game_types.h"
#include "game_flags.h"

void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800CB4E0(void);

s32 ovl_11_func_800C8FA8(Ovl11C8FA8Object *arg0) {
    u16 *temp_s1;
    u16 temp_a0;

    if (D_80128C54 & 0x80) {
        temp_s1 = D_80128C4C_dispatch;
        temp_a0 = temp_s1[15];
        if (!(temp_a0 & 3)) {
            if (arg0->unk3C == 0x32) {
                if (GetGameFlag(2U) == 0) {
                    ovl_11_func_800F0FB0(9);
                    arg0->unk22 = temp_s1[0];
                    arg0->unk6C |= 0x800000;
                }
                return 0;
            }
            if (!(temp_a0 & 0x1000)) {
                arg0->unk6C |= 0x800000;
            }
            ovl_11_func_800F0FB0(3);
            arg0->unk22 = temp_s1[0];
            ovl_11_func_800CB4E0();
            return 1;
        }
        return 0;
    }
    return 0;
}
