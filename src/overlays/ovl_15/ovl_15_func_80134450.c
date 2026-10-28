#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ui_sprites.h"

void ovl_15_func_80134450(s16 arg0, s16 arg1, s16 arg2) {
    s32 i;
    s16 v;

    v = arg1;
    for (i = 0; i < arg0; i++) {
        DrawUiSprite2A(gActiveRenderContext->field_D8 + 4, v, arg2);
        v -= 8;
    }
}
