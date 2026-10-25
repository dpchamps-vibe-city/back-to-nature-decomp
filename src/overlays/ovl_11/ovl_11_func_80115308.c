#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "text_entry.h"

void SetVal8005E2BC(s32 arg0);
u16 *ovl_11_func_800CE744(s32 arg0, s32 arg1);
s32 *ovl_11_func_800D0CD8(void);
s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_800E2904(s32 arg0);
s32 ovl_11_func_800F2354(s32 arg0, s32 arg1);
s32 ovl_11_func_80113FF0(void);
void ovl_11_func_80115C60(void *arg0);
s32 ovl_11_func_80116980(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80117178(s16 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2);
s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2);
s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3, ovl_11_DD48_arg0 *arg8);
s32 ovl_11_func_8011D06C(s32 arg0);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);
void ovl_11_func_8011D0B4(void);
s16 ovl_11_func_8011D98C(StructD548 x);
s16 ovl_11_func_8011D9DC(s16 arg0, s32 arg1);
s32 ovl_11_func_8011DAD4(s16 id, s16 amount, s32 mode);

s32 ovl_11_func_80115308(void) {
    Ovl11D8012D7B8 params;
    s32 result;
    s32 selected;
    s32 i;
    u16 *dest;
    Ovl11D8012D538 *state;
    struct struct_8006C838_800E8F78 *resident;

    result = 0;
    resident = (struct struct_8006C838_800E8F78 *)D_8006C838;
    params.unk4 = 2;
    params.unk6 = 0x44;
    params.unk8 = 2;
    params.unkA = 0x45;
    params.unkC = 2;
    params.unkE = 0x4E;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    resident->field_522C = 0;
    resident->field_5230 = 0;
    resident->field_5234 |= 0x800000;
    switch (gOvl11InteractionStep) {
    case 0:
        ovl_11_func_8011D084(2, 0);
        break;
    case 1:
        ovl_11_func_80113FF0();
        gOvl11InteractionDrawMode = 4;
        params.unk4 = -1;
        ovl_11_func_8011D084(2, 0);
    case 2:
    case 3:
    case 4:
    case 5:
    case 6:
    case 7:
    case 8:
        switch (ovl_11_func_8011B840(&gOvl11InteractionStep, 2, params)) {
        case 0:
        case 2:
            break;
        case 3:
            ovl_11_func_8011D084(24, 0);
            break;
        case 1:
            switch (((StructD548 *)gOvl11PrimarySelectionRecord)->index) {
            case 0:
                if (((Ovl11FeedGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x4C);
                    ovl_11_func_8011D084(22, 1);
                } else {
                    RequestMessageWindow(2, 0x4B);
                    ovl_11_func_8011D084(22, 9);
                }
                break;
            case 1:
                if (ovl_11_func_800D0CD8() == 0) {
                    RequestMessageWindow(2, 0x55);
                    ovl_11_func_8011D084(22, 21);
                } else if (ovl_11_func_8011D06C(ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord))) != 1) {
                    RequestMessageWindow(2, 0x4F);
                    ovl_11_func_8011D084(22, 21);
                } else {
                    RequestMessageWindow(2, 0x56);
                    ovl_11_func_8011D084(22, 16);
                }
                break;
            case 2:
                if (ovl_11_func_80117178(0, 0, -1) == 0) {
                    RequestMessageWindow(2, 0x59);
                    ovl_11_func_8011D084(22, 21);
                } else {
                    RequestMessageWindow(2, 0x58);
                    gOvl11InteractionDrawMode = 0;
                    ovl_11_func_80116980(0, 0, -1);
                    ovl_11_func_8011D084(22, 18);
                }
                break;
            case 3:
                if (ovl_11_func_8011D06C(ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord))) == 1) {
                    if (ovl_11_func_8011D9DC(ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord), 1) == 0) {
                        RequestMessageWindow(2, 0x54);
                        ovl_11_func_8011D084(22, 21);
                    } else {
                        ovl_11_func_8011DAD4(ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord), 1, 1);
                        ovl_11_func_800F2354(-ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
                        RequestMessageWindow(2, 0x51);
                        ovl_11_func_8011D084(22, 21);
                    }
                } else {
                    RequestMessageWindow(2, 0x4F);
                    ovl_11_func_8011D084(22, 21);
                }
                break;
            }
            break;
        }
        break;
    case 9:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        ovl_11_func_8011D084(23, 10);
        break;
    case 10:
    case 11:
    case 12:
    case 13:
    case 14:
    case 15:
        state = &gOvl11QuantityPrompt;
        switch (ovl_11_func_8011BAFC(&gOvl11InteractionStep, 10, 5, *(Ovl11BAFCParams *)&params, (ovl_11_DD48_arg0 *)state)) {
        case 0:
        case 2:
            break;
        case 3:
            ovl_11_func_8011D084(22, 21);
            break;
        case 1:
            CloseMessageWindow();
            if (ovl_11_func_8011D06C(state->unk8 * state->unkA) == 1) {
                if (state->unk8 + ((Ovl11FeedGlobals *)D_8006C838)->amount >= 1000) {
                    RequestMessageWindow(2, 0x4C);
                    ovl_11_func_8011D084(22, 9);
                } else {
                    ovl_11_func_80115C60(state);
                    RequestMessageWindow(2, 0x52);
                    ovl_11_func_8011D084(22, 21);
                }
            } else {
                RequestMessageWindow(2, 0x4F);
                ovl_11_func_8011D084(22, 9);
            }
            break;
        }
        break;
    case 16:
        ResetTextEntry();
        BeginTextEntry(D_8012D6EC);
        gOvl11InteractionDrawMode = 0;
        ovl_11_func_8011D084(17, 0);
        break;
    case 17:
        switch (UpdateTextEntry()) {
        case -1:
            RequestMessageWindow(2, 0x4E);
            ovl_11_func_8011D084(22, 21);
            break;
        case 1:
            dest = ovl_11_func_800CE744(0x109, -1) + 2;
            for (i = 0; i < 9; i++) {
                dest[i] = D_8012D6E8.values[i];
            }
            ovl_11_func_800F2354(-ovl_11_func_800D6014((u16)ovl_11_func_8011D98C(*(StructD548 *)gOvl11PrimarySelectionRecord)), 1);
            RequestMessageWindow(2, 0x57);
            ovl_11_func_8011D084(22, 21);
            break;
        }
        break;
    case 18:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x5A);
        ovl_11_func_8011D084(23, 19);
        break;
    case 19:
        selected = ovl_11_func_80116980(0, 0, 0);
        if (selected == -2) {
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            ovl_11_func_8011D084(22, 21);
        } else if (selected != -1) {
            CloseMessageWindow();
            SetMessagePortraitEnabled(0);
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[selected].flags & 0x40) {
                RequestMessageWindow(2, 0x61);
            } else {
                RequestMessageWindow(2, 0x60);
            }
            ovl_11_func_8011D084(23, 20);
        }
        break;
    case 20:
        selected = ovl_11_func_80116980(0, 0, 1);
        switch (ovl_11_func_80118B84(gActiveRenderContext->field_D8 + 0x2C, 0xC8, 0x80)) {
        case 0:
            break;
        case 1:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5E);
            if (((struct Ovl11Scan109Globals *)D_8006C838)->records[selected].flags & 0x40) {
                ovl_11_func_800F2354(2000, 1);
            } else {
                ovl_11_func_800F2354(500, 1);
            }
            ovl_11_func_800E2904((s32)((u8 *)&D_800742EC + selected * 0xB4));
            ovl_11_func_8011D084(22, 21);
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, 0x5B);
            ovl_11_func_8011D084(22, 21);
            break;
        }
        break;
    case 21:
        RequestMessageWindow(2, 0x53);
        ovl_11_func_8011D084(22, 1);
        break;
    case 22:
        if (GetMessageWindowStatus() == 2) {
            ovl_11_func_8011D0B4();
        }
        break;
    case 23:
        if (gOvl11InteractionResumeStep == 20) {
            ovl_11_func_80116980(0, 0, 1);
        }
        if (GetMessageWindowStatus() == 5) {
            SetVal8005E2BC(0);
            ovl_11_func_8011D0B4();
        }
        break;
    case 24:
        ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5234 &= ~0x800000;
        result = 1;
        break;
    }
    return result;
}
