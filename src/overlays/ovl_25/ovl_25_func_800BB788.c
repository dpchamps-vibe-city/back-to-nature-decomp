#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void ovl_25_func_800BB788(Ovl25SpriteDesc *arg0) {
    RECT rect;
    u16 temp_t0;

    rect.x = arg0->unkE;
    rect.y = arg0->unk10;
    rect.w = arg0->unk12;
    rect.h = arg0->unk14 - arg0->unkA;
    MoveImage(&rect, arg0->unk4, arg0->unk6 + arg0->unkA);
    temp_t0 = arg0->unkA;
    if (temp_t0 != 0) {
        rect.h = temp_t0;
        rect.x = arg0->unkE;
        rect.y = (arg0->unk10 + arg0->unk14) - temp_t0;
        rect.w = arg0->unk12;
        MoveImage(&rect, arg0->unk4, arg0->unk6);
    }
    if (arg0->unkC == arg0->unk8) {
        arg0->unkA += 2;
        arg0->unkC = 0;
        if (arg0->unkA >= (s16) arg0->unk14) {
            arg0->unkA = 0;
        }
    } else {
        arg0->unkC += 1;
    }
}
