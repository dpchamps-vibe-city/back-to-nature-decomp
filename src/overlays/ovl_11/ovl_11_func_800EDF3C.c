#include "common.h"
#include "game_flags.h"

/* Existing address-prefix exception, kept local to its authorized caller.
 * Computes a byte-pointer offset using an unsigned 16-bit constant.
 * scratch is written before base is consumed, hence its early-clobber
 * constraint; dst may overlap base. Use side-effect-free operands.
 * Keep both instructions in one non-volatile template. The scratch output
 * is consumed internally even when dead afterward. Not a memory barrier,
 * an SDK primitive, or evidence of an original studio macro. */
#define MATCH_OFFSET(dst, base, scratch, imm) \
    __asm__("ori %1,$0,%3\n\taddu %0,%2,%1" \
            : "=r"(dst), "=&r"(scratch) : "r"(base), "i"(imm))

/* User-authorized parked recovery: three local register bindings and the
 * existing two-instruction address prefix, now named MATCH_OFFSET.
 * Calls and control are C. None of the seven pin-removal combinations
 * preserves the match in this source; this is not a clean-C impossibility
 * or proof of an original macro. See notes/asm-exception-evidence.md. */

s32 ovl_11_func_800EDF3C(s16 arg0, s32 arg1) {
    s16 var_a0;
    s32 temp_v0;
    register char *p asm("$3");
    register s32 off asm("$4");
    register u32 field asm("$5");

    if ((arg1 << 0x10) != 0) {
        var_a0 = gOvl11ValueSlots[arg0];
    } else {
        var_a0 = arg0;
    }
    if (var_a0 == 0xB) {
        goto block_b;
    }
    if (var_a0 == 0x14) {
        temp_v0 = GetGameFlag(0x25U);
        if (temp_v0 == 1) {
            if (gCalendarSeason == temp_v0) {
                goto ret1;
            }
            goto block_7;
        }
    }
    goto ret1;
block_7:
    return 0;
ret1:
    return 1;
block_b:
    if (GetGameFlag(0x129U) == 1) {
        goto block_7;
    }
    p = (char *) &D_8006C838;
    MATCH_OFFSET(p, p, off, 0x8000);
    field = *(u8 *) (p + 0x6647);
    off = 0xFF;
    if (field != off) {
        goto block_7;
    }
    goto ret1;
}