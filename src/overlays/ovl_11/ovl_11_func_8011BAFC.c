#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"
#include "ovl11_selection_record_access.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_800D6014(u16 arg0);
s32 ovl_11_func_8011CF40(s16 arg0, u16 *arg1);
void ovl_11_func_8011DD48(ovl_11_DD48_arg0 *arg0, s32 arg1, s16 arg2, s16 arg3, s16 arg4);

s32 ovl_11_func_8011BAFC(s32 *arg0, s32 arg1, s32 arg2, Ovl11BAFCParams arg3,
                       ovl_11_DD48_arg0 *arg8) {
    s32 result;
    StructD548 *record;

    if ((u32)(*arg0 - arg1) >= 6U) {
        return -1;
    }
    record = arg3.record;
    result = 0;
    switch (*arg0 - arg1) {
    case 0:
        Ovl11SetInteractionStepsOffset(3, 0, arg1);
        break;
    case 1:
        if (GetMessageWindowStatus() == 2) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 2:
        if (GetMessageWindowStatus() == 5) {
            Ovl11ResumeInteractionStep();
        }
        break;
    case 3:
        SetMessagePortraitEnabled(0);
        SetVal8005E2BC(0);
        RequestMessageWindow(2, 0x4D);
        ovl_11_func_8011DD48(arg8, 0, 0,
            (s16)ovl_11_func_800D6014((u16)Ovl11GetSelectionEntryId(*record)), 0);
        gOvl11InteractionDrawMode = arg2;
        Ovl11SetInteractionStepsOffset(2, 4, arg1);
        break;
    case 4:
        switch (ovl_11_func_8011CF40(0x63, (u16 *)&arg8->field_8)) {
        case 0:
            break;
        case 1:
            result = 1;
            CloseMessageWindow();
            break;
        case 2:
            CloseMessageWindow();
            RequestMessageWindow(2, Ovl11GetSelectionMessageId(*record));
            Ovl11SetInteractionStepsOffset(1, 3, arg1);
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(arg3.field_C, arg3.field_E);
            Ovl11SetInteractionStepsOffset(1, 5, arg1);
            break;
        }
        break;
    case 5:
        result = 3;
        break;
    }
    return result;
}
