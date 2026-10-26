#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_80110944(s16 area, u16 mode);
s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);
s32 ovl_11_func_80110494(Ovl11StateKeyProbeView *arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_800DF010(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_800E0C24(Ovl11ActionDispatchView *arg0, s32 arg1);
s32 ovl_11_func_80110520(s32 arg0, s16 arg1);
s32 ovl_11_func_80110188(Ovl11StateArg0 *arg0, s16 arg1, s32 *arg2, s16 arg3);
u16 ovl_11_func_80110118(Ovl11ActionCompletionView *arg0);

s32 ovl_11_func_8010FBC4(Ovl11SetFieldsView *arg0, s32 arg1) {
    Ovl11PaddedVec3 position;
    Ovl11ActionDispatchView *record;
    Ovl11ActionDispatchView *animal;
    u8 *base;
    s16 index;
    s32 selected;
    s32 state;

    state = -1;
    index = *(s16 *)((u8 *)arg0 + 0xBA);
    if (index != state) {
        animal = (Ovl11ActionDispatchView *)((u8 *)&gTaggedRecordBase + index * 0xB8);
        record = animal;
    } else {
        animal = NULL;
        record = NULL;
    }
    state = *(s16 *)((u8 *)arg0 + 0x28);
    switch (state) {
    case 0:
        selected = ovl_11_func_80110944(arg0->field_30, 6);
        if (selected == -1) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
            *(s16 *)((u8 *)arg0 + 0xBA) = selected;
            return 1;
        }
        base = (u8 *)D_8006C838;
        *(s16 *)((u8 *)arg0 + 0xBA) = selected;
        ovl_11_func_800D05D0(arg0,
            ((Ovl11D5224LargeView *)(base + (s16)selected * 0xB8))->position);
        *(s16 *)((u8 *)arg0 + 0x28) = 1;
        break;
    case 1:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 2;
        } else {
            if (*(s16 *)((u8 *)record + 0x30) == arg0->field_30) {
                ovl_11_func_800D05D0(arg0, *(Ovl11PaddedVec3 *)((u8 *)record + 0x38));
                break;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
                break;
            }
        }
        /* fall through */
    case 2:
        if (ovl_11_func_80110494((Ovl11StateKeyProbeView *)arg0, 9, 0x3C) != 0) {
            if (!(record->unk34 & 0x10000)) {
                ovl_11_func_800DF010(animal, 6);
            } else {
                ovl_11_func_800E0C24(animal, 6);
            }
            ovl_11_func_800D05D0(arg0,
                ((Ovl11D5224LargeView *)((u8 *)D_8006C838 + *(s16 *)((u8 *)arg0 + 0xBA) * 0xB8))->position);
            *(s16 *)((u8 *)arg0 + 0x28) = 4;
        }
        break;
    case 4:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0xE6) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 3;
        } else {
            if (*(s16 *)((u8 *)record + 0x30) == arg0->field_30) {
                ovl_11_func_800D05D0(arg0, *(Ovl11PaddedVec3 *)((u8 *)record + 0x38));
                break;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
                break;
            }
        }
        /* fall through */
    case 3:
        if (ovl_11_func_80110494((Ovl11StateKeyProbeView *)arg0, 0xA, 0x3C) != 0) {
            animal->unkB6 = *(u16 *)((u8 *)arg0 + 0xAC);
            if (!(record->unk34 & 0x10000)) {
                ovl_11_func_800DF010(animal, 7);
            } else {
                ovl_11_func_800E0C24(animal, 8);
            }
            if (*(u16 *)((u8 *)arg0 + 0xB6) != 0) {
                *(s16 *)((u8 *)arg0 + 0x28) = 7;
            } else {
                *(s16 *)((u8 *)arg0 + 0x28) = 0;
                break;
            }
        } else {
            break;
        }
        /* fall through */
    case 7:
        if (ovl_11_func_80110520((s32)arg0, *(s16 *)((u8 *)arg0 + 0xB6)) != 0) {
            ovl_11_func_80110188((Ovl11StateArg0 *)arg0, 2, &position.field_0, arg0->field_30);
            ovl_11_func_800D05D0(arg0, position);
            *(s16 *)((u8 *)arg0 + 0x28) = 5;
        }
        break;
    case 5:
        if (ovl_11_func_80110544((Ovl11E644Object *)arg0, 0x258) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 6;
        }
        break;
    case 6:
        if (ovl_11_func_80110118((Ovl11ActionCompletionView *)arg0) != 0) {
            *(s16 *)((u8 *)arg0 + 0x28) = 0;
        }
        break;
    }
    return 0;
}
