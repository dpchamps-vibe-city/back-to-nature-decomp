#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_11_func_80120F30(s16 arg0, s16 arg1, Ovl11TextureCoordinates *arg2) {
    s32 byte;
    s16 bit;
    Ovl11GridTextureEntry *entry;
    u32 header;
    u32 kind;
    s32 flags;
    s32 forward;

    byte = arg0 / 8;
    bit = arg0 % 8;
    flags = D_801287C4[byte] & (0x80 >> bit);
    forward = flags != 0;
    entry = ((Ovl11GridTextureTableView *)&D_8007AFF0)->entries[arg1 * 45 + arg0];
    header = entry->header;
    if (header & 8) {
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            if (forward) {
                setUV4(entry, arg2->u0, arg2->v0, arg2->u1, arg2->v1,
                       arg2->u2, arg2->v2, arg2->u3, arg2->v3);
            } else {
                setUV4(entry, arg2->u3, arg2->v3, arg2->u2, arg2->v2,
                       arg2->u1, arg2->v1, arg2->u0, arg2->v0);
            }
            entry->tpage = 0x1F;
            entry->clut = arg2->clut;
            return 1;
        }
        if (kind == 0x2D010000) {
            if (forward) {
                setUV4(entry, arg2->u0, arg2->v0, arg2->u1, arg2->v1,
                       arg2->u2, arg2->v2, arg2->u3, arg2->v3);
            } else {
                setUV4(entry, arg2->u3, arg2->v3, arg2->u2, arg2->v2,
                       arg2->u1, arg2->v1, arg2->u0, arg2->v0);
            }
            entry->tpage = 0x1F;
            entry->clut = arg2->clut;
            return 1;
        }
    }
    return 0;
}
