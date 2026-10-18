#include "common.h"
#include "psyq/libsnd.h"

void func_8001B100(void);
void func_80011EF0(s32);
void func_800132B8(s32, s32, s32);
void func_800132F0(s32, s32, s32);
s32 func_80013394(void);
void func_80017A08(s32, s32);
void func_80017A38(s16, s16);
void func_8001FE00(s32);
void func_8001FE34(s32);
/* The original callers supply an additional, unread fade-time argument. */
s32 func_8002029C();
void func_8002238C(s32);
void func_800226F0(void);
void func_8002495C(s16);
void ovl_15_func_8012E15C(void);
void ovl_15_func_801301A0(void);
void ovl_15_func_801372A0(s16);

void ovl_15_func_8012E520(void) {
    s32 sprite;
    Ovl21MainStateView *state;

    sprite = D_8005E3C0->field_D8 + 0x2C;
    state = (Ovl21MainStateView *)D_8006C838;
    if (state->state == 0) {
        func_8002029C(0, 0, 0, 0, 0);
        SsUtAllKeyOff(0);
        func_8001B100();
        func_800226F0();
        ovl_15_func_8012E15C();
        D_80137584 = 0;
        ovl_15_func_801372A0(0);
        func_800132B8(0x3C, 0, 2);
        func_8001FE00(0x3C);
        func_8002495C(-1);
        state->state = 1;
    } else if (state->state == 1) {
        func_8002238C(sprite);
        if (func_80013394() == 1) {
            state->state = 2;
        }
    } else if (state->state == 2) {
        D_80137586 = 2;
        func_80017A08(0, (s32)&D_801375A8[D_8013758C]);
        func_8002238C(sprite);
        func_8002495C(0);
        if (D_80137584 != D_80137585) {
            D_8013759A = 0;
            D_80137585 = D_80137584;
        } else if (D_8013759A < 0xFF) {
            D_8013759A++;
        }
        func_80017A38(0, 2);
        ovl_15_func_801301A0();
        func_80017A38(0, 0);
        switch (D_80137586) {
        case 2:
            break;
        case -1:
            ((Ovl21MainStateView *)D_8006C838)->state = 4;
            func_800132F0(0xA, 0, 2);
            func_8001FE34(0xA);
            break;
        case 1:
            ((Ovl21MainStateView *)D_8006C838)->state = 3;
            func_800132F0(0xA, 0, 2);
            func_8001FE34(0xA);
            break;
        }
    } else if ((unsigned)(state->state - 3) < 2) {
        func_8002238C(sprite);
        if (func_80013394() == 1) {
            func_8002029C(0, 0, 0, 0x50, 0xA);
            state->state = state->state == 3 ? 3 : 4;
            if (state->state == 4) {
                if (state->flags & 0x4000) {
                    func_80011EF0(0xD);
                } else {
                    func_80011EF0(0x13);
                }
            } else {
                if (state->flags & 0x4000) {
                    state->flags &= ~0x4000;
                }
                func_80011EF0(0xE);
            }
        }
    }
}
