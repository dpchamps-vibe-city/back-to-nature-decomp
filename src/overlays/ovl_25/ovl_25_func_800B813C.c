#include "common.h"
#include "gte_scene_state.h"

s32 func_800225C4(void);
void ovl_25_func_800BAAAC(void);
void ovl_25_func_800BABC4(void);
void ovl_25_func_800BAC28(void);
void ovl_25_func_800BB580(void);
void ovl_25_func_800BBA7C(void);

void ovl_25_func_800B813C(void) {
    func_800225C4();
    ovl_25_func_800BAAAC();
    ovl_25_func_800BABC4();
    ApplyGteSceneState();
    ovl_25_func_800BAC28();
    ovl_25_func_800BB580();
    D_800BCBEC[D_800BFE44_view.selector]();
    ovl_25_func_800BBA7C();
}
