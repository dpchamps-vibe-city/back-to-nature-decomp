#include "common.h"
#include "game_types.h"
#include "ovl11_direction_vector.h"

s32 ovl_11_func_80110544 (Ovl11E644Object *arg0, s16 arg1);
void ovl_11_func_800D05D0 (Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
void ovl_11_func_80110E34 (M2C_269c1560d5f5_Unk80110E34 *arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
s32 ovl_11_func_80110188 (Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);

s32 ovl_11_func_8010F4CC(Ovl11E644Object *arg0) {
    Ovl11PaddedVec3 position;
    s32 result;

    result = 0;
    arg0->unk34 |= 0x3C000800;
    if (*(s16 *) ((u8 *) arg0 + 0x28) == -1) {
        ovl_11_func_80110544(arg0, 0xE6);
        if (*(s16 *) ((u8 *) arg0 + 0x30) == 1) {
            *(s16 *) ((u8 *) arg0 + 0x28) = 0;
        }
    } else {
        if (*(s16 *) ((u8 *) arg0 + 0x30) == 1) {
            switch (*(s16 *) ((u8 *) arg0 + 0x28)) {
            case 0:
                arg0->unk34 |= 0x800;
                *(s16 *) ((u8 *) arg0 + 0x28) = 1;
                position.field_0 = -0xCE4;
                position.field_4 = 0;
                position.field_8 = 0xFA0;
                ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, position);
                break;
            case 1:
                if (!ovl_11_func_80110544(arg0, 0x258)) {
                    break;
                }
                *(s16 *) ((u8 *) arg0 + 0x28) = 2;
                /* fall through */
            case 2:
                arg0->unk24 = 1;
                *(s16 *) ((u8 *) arg0 + 0x22) = 2;
                Ovl11SetDirectionVector(2, (Recon800D0408A1View *) &arg0->unk48, 0x18);
                if (*(s32 *) ((u8 *) arg0 + 0x40) >= 0x1A2D) {
                    ovl_11_func_80110E34((M2C_269c1560d5f5_Unk80110E34 *) arg0);
                    arg0->unk34 &= ~0x800;
                    ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) arg0, 0);
                    result = 1;
                }
                break;
            }
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 4) {
            ovl_11_func_80110188((Ovl11StateArg0 *) arg0, 4, &position.field_0, 4);
            ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 5) {
            ovl_11_func_80110188((Ovl11StateArg0 *) arg0, 5, &position.field_0, 5);
            ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        } else if (*(s16 *) ((u8 *) arg0 + 0x30) == 6) {
            ovl_11_func_80110188((Ovl11StateArg0 *) arg0, 3, &position.field_0, 6);
            ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, position);
            *(s16 *) ((u8 *) arg0 + 0x28) = -1;
        }
    }
    return result;
}
