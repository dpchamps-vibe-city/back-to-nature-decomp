#include "common.h"
#include "game_types.h"
#include "screen_fade.h"

#define SELECTION (*(u16 *)&D_801273D8)

void SetVal8005E334(s32);
void SetVal8005E2BC(s32);
s32 func_8002261C(s32, s32);
void func_80022580(u32 *, s32, s32, s32, s16, s16);
void func_80017A38(s16, s16);
void func_80017B3C(s32, s32, s32, s32);
s32 func_800226B0(void);
s32 func_8001FABC(s16);
void func_800248B0(s32, s16, s16);
void func_80022738(void);
void func_8001AF70(u16, u16);
void ovl_11_func_800CE210(Ovl11C7010Object *);
void func_8001FE34(s32);

s32 ovl_11_func_800FFB08(void) {
    s32 ready;
    s32 buttons;
    s16 row;

    DrawScreenSubtractOverlay(0x40000, 0x40000, 0x40000);
    SetVal8005E334(0);
    SetVal8005E2BC(0);
    func_8002261C(3, 0x1B7);
    func_80022580((u32 *)(gActiveRenderContext->field_D8 + 0x58), 1, 0x10, 0x10, 0xAC, 0x58);
    func_80017A38(0, 2);
    func_80017B3C(gActiveRenderContext->field_D8 + 0x54, (s32)(D_80053A22 + D_80054BBC[0]), 0x12, 0x12);
    func_80017A38(0, 0);
    ready = func_800226B0();
    if (ready) {
        buttons = *(s32 *)D_8005E3A8;
        if (buttons & 0x4000) {
            func_8001FABC(5);
            SELECTION++;
        } else if (buttons & 0x1000) {
            func_8001FABC(5);
            SELECTION += 3;
        }
        SELECTION &= 3;
    }
    if (SELECTION == 0) {
        row = SELECTION;
    } else if (SELECTION == 1) {
        row = SELECTION + 1;
    } else {
        row = SELECTION + 2;
    }
    func_800248B0(gActiveRenderContext->field_D8 + 0x50, 0x16, (s16)(row * 14 + 0x18));
    if (ready) {
        buttons = *(s32 *)((u8 *)D_8005E3A8 + 8);
        if (buttons & 0x820) {
            func_8001FABC(1);
            func_80022738();
            return -1;
        } else if (buttons & 0x40) {
            func_8001FABC(0);
            func_80022738();
            switch (SELECTION) {
            case 1:
                func_8001AF70(0x48, 1);
            case 0:
                ovl_11_func_800CE210((Ovl11C7010Object *)&D_80071A00);
                break;
            case 2:
                ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_254A0 = 0xF;
            case 3:
                if (SELECTION == 3) {
                    ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_254A0 = 0x11;
                }
                D_80070CC4 = 0;
                BeginScreenFadeIncrease(0xA, 0, 2);
                func_8001FE34(0xA);
                break;
            }
            return SELECTION + 1;
        }
    }
    return 0;
}
