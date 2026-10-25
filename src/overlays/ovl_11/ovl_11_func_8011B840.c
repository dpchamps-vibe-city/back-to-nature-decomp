#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_interaction_steps.h"

void SetVal8005E2BC(s32 arg0);
s32 ovl_11_func_8011D734(Ovl11D548Arg *arg0);
s16 ovl_11_func_8011D9B4(StructD548 x);

s32 ovl_11_func_8011B840(s32 *arg0, s32 arg1, Ovl11D8012D7B8 arg2) {
    s32 result;
    StructD548 *record;
    s16 startKind;
    s16 startId;
    s16 cancelKind;
    s16 cancelId;
    s32 state;

    if ((u32)(*arg0 - arg1) >= 6U) {
        return -1;
    }
    result = 0;
    record = (StructD548 *)arg2.unk0;
    startKind = arg2.unk4;
    startId = arg2.unk6;
    cancelKind = arg2.unk8;
    cancelId = arg2.unkA;
    state = arg2.unk10;
    switch (*arg0 - arg1) {
    case 0:
        if (startKind != -1) {
            RequestMessageWindow(startKind, startId);
            Ovl11SetInteractionStepsOffset(1, 3, arg1);
        } else {
            Ovl11SetInteractionStepsOffset(3, 0, arg1);
        }
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
        if (gOvl11InteractionDrawMode == 7) {
            gOvl11InteractionDrawMode = state;
            if (RequestMessageWindow(2, 0x5F) == 0) {
                Ovl11SetInteractionStepsOffset(2, 4, arg1);
            }
        } else {
            gOvl11InteractionDrawMode = state;
            if (RequestMessageWindow(2, 0x46) == 0) {
                Ovl11SetInteractionStepsOffset(2, 4, arg1);
            }
        }
        break;
    case 4:
        switch (ovl_11_func_8011D734((Ovl11D548Arg *)record)) {
        case 0:
            break;
        case 1:
            result = 1;
            CloseMessageWindow();
            break;
        case 2:
            if (ovl_11_func_8011D9B4(*record) != -1) {
                CloseMessageWindow();
                RequestMessageWindow(2, ovl_11_func_8011D9B4(*record));
                Ovl11SetInteractionStepsOffset(1, 3, arg1);
            }
            break;
        case 3:
            CloseMessageWindow();
            RequestMessageWindow(cancelKind, cancelId);
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
