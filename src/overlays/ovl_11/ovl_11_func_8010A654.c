#include "common.h"
#include "game_types.h"
#include "sound_commands.h"
#include "psyq/abs.h"
#include "ovl11_record_pair_order.h"

s32 ovl_11_func_80109188 (Ovl11D04D4Obj *arg0, s32 arg1);
s32 ovl_11_func_800D04D4 (Ovl11A47CObject *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);
s32 ovl_11_func_8010AE64 (Recon_ovl_11_func_800D2594_A0View *arg0);
void ovl_11_func_800D31EC (u16 arg0, u16 arg1);
s32 ovl_11_func_800D3104 (u16 *arg0, s32 arg1);
s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);

s32 ovl_11_func_8010A654(Ovl11A654Object *arg0) {
    s16 temp_v1;
    s32 result;

    result = 0;
    temp_v1 = arg0->unk28;
    switch (temp_v1) {                              /* irregular */
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        D_8012D072 = 0;
        break;
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        arg0->unk34 &= ~0x2000;
        ovl_11_func_80109188((Ovl11D04D4Obj *) arg0, 0);
        break;
    case 1:
    default:
        if (ovl_11_func_800D04D4((Ovl11A47CObject *) arg0, D_80127ADC, &arg0->unk28, -1, &D_8012D072) != 0) {
            arg0->unk28 = -1;
        }
        if (arg0->unk34 & 0x2000) {
            arg0->unk58 = D_8012D080[0x38 / 4];
            arg0->unk5C = D_8012D080[0x3C / 4];
            arg0->unk60 = D_8012D080[0x40 / 4];
            arg0->unk64 = D_8012D080[0x44 / 4];
            ovl_11_func_8010AE64((Recon_ovl_11_func_800D2594_A0View *) arg0);
            if (arg0->unk28 != 1) {
                arg0->unk28 = -1;
            }
        } else {
            if (arg0->unk28 == 1) {
                if (((struct Ovl11UpdateWork *) &D_8007AFF0)->field_25476 == arg0->unk30) {
                    PlaySoundEffect(0x19);
                }
                ovl_11_func_800D31EC(((s16) arg0->unkAC / 10) & 0xFFFF, arg0->unk22);
                ovl_11_func_800D3104((u16 *) D_8012D080, 6);
                D_8012D072 = 0;
                arg0->unk28 = (u16) arg0->unk28 + 1;
            }
            Ovl11SelectRecordPairCode((Recon_ovl_11_func_800D2594_A0View *) arg0);
            ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
        }
        arg0->unk34 |= 0x800;
        break;
    }
    return result;
}
