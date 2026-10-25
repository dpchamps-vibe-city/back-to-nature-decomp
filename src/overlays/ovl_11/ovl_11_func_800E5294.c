#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "game_flags.h"

/* These calls explicitly zero the unused trailing argument slots. */
s32 ovl_11_func_800E63C8();
u16 ovl_11_func_800EAF5C();
void ovl_11_func_80107DD0(s16 *arg0);

void ovl_11_func_800E5294(void) {
    struct Ovl11E5294State *state;

    SetGameFlag(3U, 1U);
    if (!(((struct Ovl11E5294State *) D_8006C838)->status & 0x20000010)) {
        ovl_11_func_800E63C8(0, 0, 0, 0);
    }
    if (!(((u16 *) ((struct Ovl11SpriteWork *) &D_8007AFF0)->header)[4] & 0x1000)) {
        if ((u32) (((struct Ovl11E5294State *) D_8006C838)->current[1] - 0x15F) >= 0x32U) {
            ovl_11_func_800EAF5C(0, 0, 0, 0);
        }
    }
    ovl_11_func_80107DD0(D_80071CCC);
    /* D_80071CCC is the halfword pair at D_8006C838 + 0x5494. */
    state = (struct Ovl11E5294State *) ((u8 *) D_80071CCC - 0x5494);
    state->state = 1;
    state->flags &= ~0x2000;
}
