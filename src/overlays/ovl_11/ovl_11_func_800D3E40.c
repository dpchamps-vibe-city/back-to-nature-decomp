#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800D3E40", ovl_11_func_800D3E40);


/* PARKED by /auto_decompilation_loop on 2026-10-11T07:56:55.808Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800D3E40.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D3E40(Ovl11Func801096FCArg x) {
    register s32 *flag asm("$14");
    register s32 **out asm("$15");
    register s32 result asm("$11");
    register u8 *p asm("$5");
    s32 lo1;
    s32 lo0;
    s16 sel;
    s32 i;
    u8 *base;
    s32 r;
    s32 hi0;
    s32 hi1;

    result = -1;
    i = 0;
    base = (u8 *)&D_800749F4;
    flag = x.unk18;
    out = x.unk1C;
    sel = x.unk10;
    r = x.unk14;
    lo0 = x.unk0 - r;
    hi0 = x.unk0 + r;
    lo1 = x.unk8 - r;
    hi1 = x.unk8 + r;
    do {
        p = base + i * 0xB8;
        if (*(u16 *)p != 0 && sel == *(s16 *)(p + 0x30) && (*(s32 *)(p + 0x34) & 0x20000) == 0 &&
            lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0 &&
            lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1) {
            result = i;
            if (*(s32 *)(p + 0x34) & 0x10000) {
                *flag = 0;
                *out = (s32 *)p;
            } else {
                *flag = 1;
                *out = (s32 *)p;
            }
            break;
        }
        i++;
    } while (i < 0x14);
    if (result == -1) {
        p = (u8 *)&D_800742EC;
        i = 0;
        do {
            if (*(u16 *)p != 0 && sel == *(s16 *)(p + 0x30) && (*(s32 *)(p + 0x34) & 0x20000) == 0 &&
                lo0 < *(s32 *)(p + 0x38) && *(s32 *)(p + 0x38) < hi0 &&
                lo1 < *(s32 *)(p + 0x40) && *(s32 *)(p + 0x40) < hi1) {
                result = i;
                *flag = 2;
                *out = (s32 *)p;
                break;
            }
            i++;
            p += 0xB4;
        } while (i < 0xA);
    }
    return result;
}
#endif
