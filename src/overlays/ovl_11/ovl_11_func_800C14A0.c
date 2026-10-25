#include "common.h"
#include "gte_scene_state.h"
#include "cd_audio.h"

void ovl_11_func_800C188C(void);
void ovl_11_func_800C162C(void);
void ovl_11_func_800BBC74(void);
void ovl_11_func_800E3AA4(void);
void ovl_11_func_80120A2C(void);
void ovl_11_func_800DB674(void);
void ovl_11_func_800DB78C(void);
void ovl_11_func_800DCC5C(void);
void ovl_11_func_800DB904(void);

void ovl_11_func_800C14A0(void) {
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x18000) {
        D_8005E290 = 1;
    }
    switch (((struct Ovl11FrameWork *) &D_8007AFF0)->dispatch) {
    case 0:
        ovl_11_func_800C188C();
        if ((((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 5) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x18) && (((struct Ovl11FrameWork *) &D_8007AFF0)->mode != 0x19)) {
            ovl_11_func_800C162C();
        }
        ovl_11_func_800BBC74();
        ovl_11_func_800E3AA4();
        PollCdAudio();
        break;
    case 1:
    case 2:
        break;
    case 3:
        ovl_11_func_80120A2C();
        ovl_11_func_800DB674();
        ovl_11_func_800DB78C();
        ApplyGteSceneState();
        if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
            ovl_11_func_800DCC5C();
            ovl_11_func_800DB904();
        }
        break;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x40000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags &= 0xFF7BFFFF;
    }
    if (((struct Ovl11FrameState *) D_8006C838)->flags & 0x20000) {
        ((struct Ovl11FrameState *) D_8006C838)->flags = (((struct Ovl11FrameState *) D_8006C838)->flags & 0xFFFDFFFF) | 0x800000;
    }
}
