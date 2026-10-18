#include "common.h"

void func_80021B90(s32 arg0);
void ovl_11_func_800D93C8(void);
void ovl_11_func_80111D94(void);
void ovl_11_func_80111B74(void);
void ovl_11_func_800D9F1C(void);
void ovl_11_func_800DDBA0(void);
void ovl_11_func_800DD4D8(void);
void ovl_11_func_800DD6F0(void);
void ovl_11_func_800DD904(void);
void ovl_11_func_800DDA60(void);
void ovl_11_func_800DDD18(void);

#define STATE ((struct Ovl11EventDispatchState *)D_8006C838)
#define HEADER ((struct Ovl11SceneHeader *)((Ovl11FarInitView *)&D_8007AFF0)->data)

void ovl_11_func_800DC148(void) {
    s32 sound;
    s32 i;

    switch ((s16)((u16)HEADER->kind - 1)) {
    case 0:
        if (STATE->flags & 0x4000) {
            sound = -1;
            STATE->flags &= ~0x4000;
            for (i = 0; i < 10; i++) {
                if (((struct Ovl11EventDispatchState *)&D_8006C838[i * 3])->records[0].type == 0x109 &&
                    ((struct Ovl11EventDispatchState *)&D_8006C838[i * 3])->records[0].active == 0) {
                    sound = 0x13;
                    break;
                }
            }
            if (STATE->flags & 0x8000) {
                sound = 0x14;
            }
            if (sound != -1) {
                func_80021B90(sound);
            }
        }
        ovl_11_func_800D93C8();
        break;
    case 1:
        if (!(STATE->flags & 0x10000)) {
            STATE->flags |= 0x10000;
        }
        break;
    case 14:
        if (HEADER->id == 0xE5) {
            ovl_11_func_80111D94();
        } else {
            ovl_11_func_80111B74();
        }
        break;
    case 5:
        ovl_11_func_800D9F1C();
        break;
    case 43:
        ovl_11_func_800DDBA0();
        break;
    }
    if (STATE->mode == 0xC && !(STATE->flags & 0x40000)) {
        STATE->flags |= 0x40000;
        func_80021B90(0x15);
    }
    if (STATE->mode == 0x12 && !(STATE->flags & 0x80000)) {
        STATE->flags |= 0x80000;
        func_80021B90(0x15);
    }
    ovl_11_func_800DD4D8();
    ovl_11_func_800DD6F0();
    ovl_11_func_800DD904();
    ovl_11_func_800DDA60();
    ovl_11_func_800DDD18();
}
