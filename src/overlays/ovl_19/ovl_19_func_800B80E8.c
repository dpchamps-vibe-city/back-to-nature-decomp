#include "common.h"
#include "gte_scene_state.h"

s32 func_800225C4(void);
void ovl_19_func_800BB358(void);
void ovl_19_func_800BB470(void);
void ovl_19_func_800BB4D4(void);

void ovl_19_func_800B80E8(void) {
    func_800225C4();
    ovl_19_func_800BB358();
    ovl_19_func_800BB470();
    ApplyGteSceneState();
    ovl_19_func_800BB4D4();
    D_800BCEA8[D_800BF4C4]();
}
