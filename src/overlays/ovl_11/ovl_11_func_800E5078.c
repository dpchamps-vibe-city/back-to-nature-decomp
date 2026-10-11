#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800C28CC(s16 arg0, SpriteSourceData *arg1);

void ovl_11_func_800E5078(void) {
    s32 i;

    for (i = 0; i < 0x25; i++) {
        *(SpriteSourceData *) (D_800762D0 + i * 0x1D4) =
            *(SpriteSourceData *) (D_800762D0 - 0x30 + i * 0x1D4);
        if (((struct Ovl11CurrentObjectView *) D_8006C838)->current[1] == 0x82) {
            ovl_11_func_800C28CC(i, (SpriteSourceData *) (D_800762D0 + i * 0x1D4));
        }
    }
}
