/* Read-only record-action gate, ovl_11:800EDF3C–800EE000.
 * Shared value slots, flags and season are inputs, not privately owned state.
 * The public command ABI and selector-specific rules are documented in
 * ovl11_record_action_predicate.h; membership evidence is in file-groupings.md.
 */
#include "common.h"
#include "game_flags.h"
#include "ovl11_record_action_predicate.h"

/* The three bindings and address prefix retain the previously authorized
 * parked recovery. They are not evidence of original source organization;
 * see notes/asm-exception-evidence.md for the outstanding clean-C limitation.
 * EB85C explicitly supplies both unread command operands; retain that ABI. */
s32 Ovl11IsRecordActionAllowed(s16 recordOrSlot, s32 fromValueSlot,
                              s32 unused2, s32 unused3) {
    s16 recordId;
    s32 flagResult;
    register char *workPage asm("$3");
    register s32 offset asm("$4");
    register u32 statusByte asm("$5");

/* Keep the existing address-prefix macro local to its one authorized user.
 * The early-clobber scratch is written before base is consumed; dst may
 * overlap base. The non-volatile pair is not a memory barrier or SDK idiom. */
#define MATCH_OFFSET(dst, base, scratch, imm) \
    __asm__("ori %1,$0,%3\n\taddu %0,%2,%1" \
            : "=r"(dst), "=&r"(scratch) : "r"(base), "i"(imm))

    if ((fromValueSlot << 0x10) != 0) {
        recordId = gOvl11ValueSlots[recordOrSlot];
    } else {
        recordId = recordOrSlot;
    }
    if (recordId == 0xB) {
        goto checkRecordB;
    }
    if (recordId == 0x14) {
        flagResult = GetGameFlag(0x25U);
        if (flagResult == 1) {
            if (gCalendarSeason == flagResult) {
                goto allowed;
            }
            goto rejected;
        }
    }
    goto allowed;
rejected:
    return 0;
allowed:
    return 1;
checkRecordB:
    if (GetGameFlag(0x129U) == 1) {
        goto rejected;
    }
    workPage = (char *) &D_8006C838;
    MATCH_OFFSET(workPage, workPage, offset, 0x8000);
    statusByte = *(u8 *) (workPage + 0x6647);
    offset = 0xFF;
    if (statusByte != offset) {
        goto rejected;
    }
    goto allowed;
#undef MATCH_OFFSET
}
