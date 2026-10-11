#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800D7144", ovl_11_func_800D7144);


/* PARKED by /auto_decompilation_loop on 2026-10-11T03:55:21.665Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800D7144.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
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
    DR_TPAGE *tpagep;

    entry = (Ovl11A04B8Entry *)((u8 *)D_800A04B8 + arg1 * 0x10E + arg0 * 0x1E);
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
            arg5 = arg5 + entry->unk14;
            arg6 = arg6 + entry->unk16;
        }
    } else {
        u = entry->unk0;
        v = entry->unk2;
        w = entry->unk8;
        h = entry->unkA;
        if (arg7 & 2) {
            arg5 = arg5 + entry->unk10;
            arg6 = arg6 + entry->unk12;
        }
    }

    setXY0(sprt, arg5, arg6);
    setWH(sprt, w, h);
    setUV0(sprt, (u8)(u * 4), (u8)v);
    AddPrim(ot, sprt);

    sprt = (SPRT *)((u8 *)sprt + sizeof(SPRT));
    tpagep = (DR_TPAGE *)sprt;

    tpage = 0x340;
    if (entry->unk18 >= 0x3F0) {
        tpage = 0x3C0;
    }
    setDrawTPage(tpagep, 1, 1, tpage >> 6);
    AddPrim(ot, tpagep);

    return (u8 *)sprt + 0xC;
}
#endif
