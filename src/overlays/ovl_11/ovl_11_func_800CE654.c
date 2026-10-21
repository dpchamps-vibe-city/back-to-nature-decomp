#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

#define STATE ((struct Ovl11CE654State *) D_8006C838)
#define MODE ((struct Ovl11SelectionResetMode *) &D_8007AFF0)


s32 ovl_11_func_800CE654(void) {
    u32 temp_v1;

    if (STATE->flags & 0x10000) {
        temp_v1 = STATE->input->index;
        if (temp_v1 < 0x738U) {
            STATE->input->current = STATE->input->samples[temp_v1];
            ++STATE->input->index;
        } else if (MODE->mode == 0) {
            MODE->mode = 0x1A;
        }
    } else if (STATE->flags & 0x8000) {
        STATE->flags = (STATE->flags & ~0x8000) | 0x10000;
        D_801231F4 = 0;
        D_801231F8 = 0;
        STATE->input->current = STATE->input->samples[STATE->input->index];
        ++STATE->input->index;
        return 1;
    }
    return 0;
}
