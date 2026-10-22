#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/libapi.h"

void *ovl_11_func_800D7144(s16 arg0, s16 arg1, s16 arg2, s32 *ot, SPRT *sprt, s16 arg5, s16 arg6, s32 arg7) {
    Ovl11A04B8Entry *entry;
    s16 u;
    s16 v;
    s16 w;
    s16 h;
    s32 tpage;
    DR_MODE *tpagep;

    entry = &D_800A04B8[arg1][arg0];
    setSprt(sprt);
    sprt->clut = getClut(entry->unk18, entry->unk1A);
    setRGB0(sprt, 0x80, 0x80, 0x80);
    setSemiTrans(sprt, 0);

    if (arg2 != 0) {
        u = entry->unk4;
        v = entry->unk6;
        w = entry->unkC;
        h = entry->unkE;
        if (arg7 & 2) {
            s32 dx;
            s32 dy;
            dx = entry->unk14;
            dy = entry->unk16;
            arg5 = arg5 + dx;
            arg6 = arg6 + dy;
        }
    } else {
        u = entry->unk0;
        v = entry->unk2;
        w = entry->unk8;
        h = entry->unkA;
        if (arg7 & 2) {
            s32 dx;
            s32 dy;
            dx = entry->unk10;
            dy = entry->unk12;
            arg5 = arg5 + dx;
            arg6 = arg6 + dy;
        }
    }

    setXY0(sprt, arg5, arg6);
    setWH(sprt, w, h);
    setUV0(sprt, (u8)(u * 4), (u8)v);
    AddPrim(ot, sprt);

    do {
        tpagep = (DR_MODE *)(sprt + 1);
        sprt = 0;
    } while (0);

    tpage = 0x340;
    if (entry->unk18 >= 0x3F0) {
        tpage = 0x3C0;
    }
    setDrawTPage(tpagep, 1, 1, tpage >> 6);
    AddPrim(ot, tpagep);

    return tpagep + 1;
}
