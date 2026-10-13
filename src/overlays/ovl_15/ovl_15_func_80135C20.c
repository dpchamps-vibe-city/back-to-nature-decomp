#include "common.h"

INCLUDE_ASM("build/ovl_15/asm/nonmatchings/ovl_15_func_80135C20", ovl_15_func_80135C20);


/* PARKED by /auto_decompilation_loop on 2026-10-13T17:04:03.809Z.
 * Reason: blocked.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_15_func_80135C20.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "psyq/libmcrd.h"
#include "psyq/libmcx.h"

s32 ovl_15_func_80135C20(s32 arg0) {
    long command;
    long result;
    s32 channel;
    s32 status;

    arg0 = (s16) arg0;
    if (((u32) arg0 << 16) == 0) {
        channel = arg0;
        goto accept_init;
    }
    if (arg0 == 1) {
        channel = 0x10;
    } else {
        return 0xC;
    }
accept_init:
    Ovl15CardRetries = 0;
    MemCardSync(0, &command, &result);
retry:
    if (MemCardAccept(channel) != 1) {
        return 0xC;
    }
    MemCardSync(0, &command, &result);
    switch (result) {
    case 1:
        if ((s16) Ovl15CardRetries >= 8) {
no_card:
            return 4;
        }
        Ovl15CardRetries++;
        goto retry;
    case 2:
        if ((s16) Ovl15CardRetries >= 8) {
            return 8;
        }
        Ovl15CardRetries++;
        goto retry;
    case 4:
        if ((s16) Ovl15CardRetries >= 8) {
        Ovl15CardRetries = 0;
        arg0 *= 0x10;
retry_type_changed:
        status = McxCardType(arg0);
        if (status != 1) {
            return 0xC;
        }
        McxSync(0, &command, &result);
        switch (result) {
        case 0:
            return 7;
        case 1:
            goto no_card;
        case 2:
            if ((s16) Ovl15CardRetries >= 8) {
                return 6;
            }
            Ovl15CardRetries++;
            goto retry_type_changed;
        case 3:
            return 5;
        default:
            return 0xC;
        }
        } else {
            Ovl15CardRetries++;
            goto retry;
        }
    case 3:
        return 5;
    case 0:
        Ovl15CardRetries = 0;
        arg0 *= 0x10;
retry_type:
        status = McxCardType(arg0);
        if (status != 1) {
            return 0xC;
        }
        McxSync(0, &command, &result);
        switch (result) {
        case 0:
            return 1;
        case 1:
            goto no_card;
        case 3:
            return 5;
        case 2:
            if ((s16) Ovl15CardRetries >= 8) {
                return 0;
            }
            Ovl15CardRetries++;
            goto retry_type;
        default:
            return 0xC;
        }
    default:
        return 0xC;
    }
}
#endif
