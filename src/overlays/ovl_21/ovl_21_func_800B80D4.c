#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"

void ovl_21_func_800BB138(void);
void ovl_21_func_800BB250(void);
void ovl_21_func_800BB2B4(void);
void ovl_21_func_800BA4C0(void);
void func_8001FE34(s32 arg0);

void ovl_21_func_800B80D4(void) {
    ovl_21_func_800BB138();
    ovl_21_func_800BB250();
    ApplyGteSceneState();
    ovl_21_func_800BB2B4();
    ovl_21_func_800BA4C0();
    BeginScreenFadeFeedback(0xA);
    func_8001FE34(0xA);
    ((Ovl21MainStateView *) D_8006C838)->state++;
}
