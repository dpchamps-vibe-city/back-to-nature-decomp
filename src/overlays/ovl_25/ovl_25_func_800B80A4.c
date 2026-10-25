#include "common.h"
#include "screen_fade.h"
#include "gte_scene_state.h"

void ovl_25_func_800BAAAC(void);
void ovl_25_func_800BABC4(void);
void ovl_25_func_800BAC28(void);
s32 func_8001FE6C(void);
void ovl_25_func_800B8478(void);
void ovl_25_func_800B93E4(void);
void ovl_25_func_800BBA30(s16 arg0, s16 arg1, s16 arg2);

void ovl_25_func_800B80A4(void) {
    u8 var_s0;

    ovl_25_func_800BAAAC();
    ovl_25_func_800BABC4();
    ApplyGteSceneState();
    ovl_25_func_800BAC28();
    var_s0 = IsScreenFadeComplete() == 1;
    if (func_8001FE6C() == 0) {
        var_s0++;
    }
    ovl_25_func_800B8478();
    ovl_25_func_800B93E4();
    if (var_s0 == 2) {
        s32 *base;

        base = (s32 *)&D_8006C838;
        base[0x448C >> 2]++;
    }
    ovl_25_func_800BBA30(0, 0xA5, 0xD5);
}
