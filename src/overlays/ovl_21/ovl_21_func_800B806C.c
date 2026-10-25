#include "common.h"
#include "gte_scene_state.h"

s32 func_800225C4(void);
void ovl_21_func_800BB138(void);
void ovl_21_func_800BB250(void);
void ovl_21_func_800BB2B4(void);

void ovl_21_func_800B806C(void) {
    func_800225C4();
    ovl_21_func_800BB138();
    ovl_21_func_800BB250();
    ApplyGteSceneState();
    ovl_21_func_800BB2B4();
    D_800BCB6C[(s16) *D_800C0448]();
}
