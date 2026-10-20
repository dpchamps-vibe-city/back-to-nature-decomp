#include "common.h"
#include "game_types.h"

s32 ovl_11_func_801105C8(Ovl11SetFieldsView *arg0, s32 arg1, s32 arg2);
s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);
s32 ovl_11_func_80110494(Ovl11StateKeyProbeView *arg0, s16 arg1, s16 arg2);
void ovl_11_func_801103E8(Ovl11StateDirectionView *arg0, s32 arg1);

s32 ovl_11_func_8010F80C(Ovl11SetFieldsView *arg0, s32 arg1) {
    switch (*(s16 *) ((u8 *) arg0 + 0x28)) {
    case 0:
        if (ovl_11_func_801105C8(arg0, arg1, 0) != 0) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
            return 1;
        }
        *(s16 *) ((u8 *) arg0 + 0x28) = 1;
        break;
    case 1:
        if (ovl_11_func_80110544((Ovl11E644Object *) arg0, 0xE6) != 0) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 2;
        }
        break;
    case 2:
        if (ovl_11_func_80110494((Ovl11StateKeyProbeView *) arg0, 8, 0) != 0) {
            ovl_11_func_801103E8((Ovl11StateDirectionView *) arg0, arg1);
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
        }
        break;
    }
    return 0;
}
