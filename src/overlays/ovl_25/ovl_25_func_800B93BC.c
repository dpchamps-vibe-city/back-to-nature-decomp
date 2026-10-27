#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl25_sprite_set_service.h"

void ovl_25_func_800B93E4(void);

void ovl_25_func_800B93BC(void) {
    Ovl25DrawPrimarySprite();
    ovl_25_func_800B93E4();
}
