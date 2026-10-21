#include "common.h"

s32 ovl_11_func_80113208(s16 arg0, s16 arg1, s16 arg2) {
    Ovl11GridTextureEntry *entry;
    Ovl11GridUvRecord *uv;
    s32 header;
    s32 kind;

    entry = ((Ovl11GridTextureRowsView *)&D_8007AFF0)->entries[arg1][arg0];
    header = entry->header;
    if (header & 8) {
        uv = (Ovl11GridUvRecord *)((u8 *)&D_801281D0 + arg2 * 0xA);
        kind = header & 0xFDFF0000;
        if (kind == 0x3D010000) {
            entry->u0 = uv->u0;
            entry->v0 = uv->v0;
            entry->u1 = uv->u1;
            entry->v1 = uv->v1;
            entry->u2 = uv->u2;
            entry->v2 = uv->v2;
            entry->u3 = uv->u3;
            entry->v3 = uv->v3;
            entry->clut = uv->clut;
            return 0;
        }
        if (kind == 0x2D010000) {
            entry->u0 = uv->u0;
            entry->v0 = uv->v0;
            entry->u1 = uv->u1;
            entry->v1 = uv->v1;
            entry->u2 = uv->u2;
            entry->v2 = uv->v2;
            entry->u3 = uv->u3;
            entry->v3 = uv->v3;
            entry->clut = uv->clut;
            return 0;
        }
    }
    return 1;
}
