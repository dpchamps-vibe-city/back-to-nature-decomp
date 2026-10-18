#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

void func_8001AF70(u16 arg0, u16 arg1);
s32 ovl_11_func_801136D0(s32 *arg0, u16 *arg1, u16 *arg2);
s32 ovl_11_func_80112EC4(s32 arg0, s32 arg1, s16 arg2, s16 arg3);

s32 ovl_11_func_80112DA8(Ovl11C7010Object *arg0) {
    VECTOR position;
    u16 column;
    u16 row;
    s16 plane;

    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37) {
        func_8001AF70(0x63U, 1U);
    }
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 0x48) {
        func_8001AF70(0x64U, 1U);
    }
    if (!(arg0->unk6C & 8)) {
        if (!(((Ovl11GridState *)D_8012D110)->flags & 0xE000)) {
            position.vx = *(s32 *)((u8 *)arg0 + 0x100);
            position.vy = *(s32 *)((u8 *)arg0 + 0x104);
            position.vz = *(s32 *)((u8 *)arg0 + 0x108);
            if (ovl_11_func_801136D0((s32 *)&position, &column, &row) == 0) {
                plane = *(s16 *)D_8012D110;
                if (plane != 0) {
                    if (ovl_11_func_80112EC4(0, plane - 1, (s16)column, (s16)row) != 0) {
                        return 1;
                    }
                }
                if (ovl_11_func_80112EC4(1, *(s16 *)D_8012D110, (s16)column, (s16)row) != 0) {
                    return 1;
                }
            }
        }
    }
    return 0;
}
