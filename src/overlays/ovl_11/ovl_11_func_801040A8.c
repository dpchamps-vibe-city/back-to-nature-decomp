#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ui_sprites.h"


extern s32 D_8012742C;

void ovl_11_func_801040A8(void) {
    s16 var_a2;
    s32 var_a1;

    var_a2 = (D_8012742C + 1) * 0xE + 0x32;
    var_a1 = -1;
    if (D_80127428 == 1) {
        var_a1 = 0xD6;
        if (D_8012742C != 6) {
            var_a1 = 0x22;
        }
    } else if (D_80127428 == 2) {
        var_a1 = 0xC4;
    }
    if (var_a1 != -1) {
        DrawUiSprite0BFrame1(gActiveRenderContext->field_D8 + 0x54, var_a1 + 6, var_a2 + 6);
    }
}
