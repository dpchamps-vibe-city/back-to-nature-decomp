#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_11_func_8011E1E8(s16 arg0, s16 arg1, s16 arg2) {
    u32 header;
    u32 kind;
    Ovl11GridTextureEntry *entry;
    Ovl11TextureCoordinates *coords;
    Ovl11TextureStateView *state;

    state = (Ovl11TextureStateView *)&D_8007AFF0;
    if (state->mode != 6) {
        return 0;
    }
    entry = state->entries[arg1 * 45 + arg0];
    header = entry->header;
    if (header & 8) {
        coords = &((Ovl11TextureCoordinates *)D_801282FC)[arg2];
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            if (arg1 != 0) {
                setUV4(entry, coords->u0, coords->v0, coords->u1, coords->v1,
                       coords->u2, coords->v2, coords->u3, coords->v3);
            } else {
                setUV4(entry, coords->u3, coords->v3, coords->u2, coords->v2,
                       coords->u1, coords->v1, coords->u0, coords->v0);
            }
            if (arg2 >= 5) {
                entry->tpage = 0x1D;
            } else {
                entry->tpage = 9;
            }
            entry->clut = coords->clut;
            return 0;
        }
        if (kind == 0x2D010000) {
            if (arg1 != 0) {
                setUV4(entry, coords->u0, coords->v0, coords->u1, coords->v1,
                       coords->u2, coords->v2, coords->u3, coords->v3);
            } else {
                setUV4(entry, coords->u3, coords->v3, coords->u2, coords->v2,
                       coords->u1, coords->v1, coords->u0, coords->v0);
            }
            if (arg2 >= 5) {
                entry->tpage = 0x1D;
            } else {
                entry->tpage = 9;
            }
            entry->clut = coords->clut;
            return 0;
        }
    }
    return 1;
}
