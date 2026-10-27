#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "sound_commands.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_keyframe_step.h"

s32 ovl_11_func_8010AE64(Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_8010AFCC(Ovl11SetFieldsView *arg0);
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11CC7CCPosition v);
s32 ovl_11_func_800D0408(s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
void ovl_11_func_8010B05C(Recon_ovl_11_func_800D2594_A0View *arg0);

s32 ovl_11_func_8010A850(Ovl11A47CObject *arg0) {
    s32 result;
    s32 flag;
    u32 status;

    flag = arg0->unk34 & 0x01000000;
    result = 0;
    switch (arg0->unk28) {
    case 0:
        arg0->unk28 = 1;
        arg0->unk34 |= 0x800;
        D_8012D074 = 0;
        D_8012D076 = 0;
        break;
    case -3:
        if (arg0->unk34 & 0x01000000) {
            D_8012D084 = 1;
            ovl_11_func_8010AE64((Recon_ovl_11_func_800D2594_A0View *) arg0);
            break;
        }
    case -2:
        ovl_11_func_8010B05C((Recon_ovl_11_func_800D2594_A0View *) arg0);
    case -1:
        result = 1;
        arg0->unk2C = 0x12C;
        arg0->unk34 &= ~0x800;
        arg0->unk34 &= ~0x2000;
        break;
    case 1:
    default:
        if (Ovl11StepPackedKeyframes(arg0, D_80127AE8, &arg0->unk28, -1, &D_8012D074)) {
            arg0->unk28 = -2;
        }
        switch (arg0->unk28) {
        case 1:
            status = ovl_11_func_8010AFCC((Ovl11SetFieldsView *) arg0);
            switch (status) {
            case 1:
                arg0->unk28++;
                D_8012D076 = 1;
                break;
            case 0:
                break;
            case 2:
                arg0->unk28 = -1;
                break;
            }
            break;
        case 2:
            if ((u32) ((u16) D_8012D076 - 1) >= 2U) {
                arg0->unk28 = -1;
                break;
            }
            D_8012D084 = 1;
            Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *) (((struct Ovl11A850State *) D_8006C838)->record + 0x18), NULL);
            ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, ((struct Ovl11A850State *) D_8006C838)->position);
            if (arg0->unk7A & 0x300) {
                if (D_8012D076 == 2) {
                    arg0->unk28++;
                }
                D_8012D076 = 2;
            }
            break;
        case 3:
            if (D_8012D076 == 2) {
                if (arg0->unk34 & 0x2000) {
                    ovl_11_func_800D05D0((Ovl11SetFieldsView *) arg0, ((struct Ovl11A850State *) D_8006C838)->position);
                    D_8012D084 = 1;
                    ovl_11_func_8010AE64((Recon_ovl_11_func_800D2594_A0View *) arg0);
                } else {
                    if (!flag) {
                        ovl_11_func_800D0408(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
                    }
                    arg0->unk28++;
                    D_8012D074 = 0;
                    D_8012D076 = 3;
                    if (((struct Ovl11UpdateWork *) &D_8007AFF0)->field_25476 == arg0->unk30) {
                        PlaySoundEffect(0x19);
                    }
                }
            } else {
                arg0->unk28 = -1;
            }
            break;
        case 4:
            if ((D_8012D076 != 3) || flag) {
                arg0->unk28 = -3;
            }
            break;
        }
        arg0->unk34 |= 0x800;
        break;
    }
    return result;
}
