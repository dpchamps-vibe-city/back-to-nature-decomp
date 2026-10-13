#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_800DAC08", ovl_11_func_800DAC08);


/* PARKED by /auto_decompilation_loop on 2026-10-13T21:00:03.558Z.
 * Reason: escalation-exhausted.
 * Escalation reached: gpt-6.1-sol.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_800DAC08.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_8012135C();

s32 ovl_11_func_800DAC08(u16 arg0, s16 arg1, s16 arg2, s16 arg3) {
    VECTOR position;
    s32 value;
    s32 origin;
    s32 id;

    id = arg0;
    if (arg1 == 0) {
        value = arg2 * 400 - 0x1068;
        position.vx = value;
        value = arg3 * 400;
        origin = 0x578;
    } else {
        value = arg2 * 400 - 0x578;
        position.vx = value;
        value = arg3 * 400;
        origin = 0x3E8;
    }
    origin -= value;
    position.vy = 0;
    position.vz = origin;
    __asm__ volatile("" : "=r"(id) : "0"(id) : "memory");
    id = (s16)id;
    return ovl_11_func_8012135C(id, position);
}
#endif
