/*
 * Halfword strings terminated by 0xFFFF, original exe:80017E34-80017F88.
 * These operations neither allocate storage nor consult text-renderer state.
 * The unused copy entry remains in its original position between append and
 * compare. The neighboring drawing wrappers are not string operations.
 */
#include "common.h"
#include "u16_string.h"
#include "include_asm.h"

/* Return the destination's new terminator, not its start. Keep one value
 * variable across the first copy and loop; the repeated source read is part
 * of the original copy-loop construction. */
u16 *AppendU16String(u16 *destination, u16 *source) {
    u16 terminator;
    u16 value;

    while (*destination != 0xFFFF) {
        destination++;
    }
    *destination = *source;
    value = *source;
    if (value != 0xFFFF) {
        terminator = 0xFFFF;
        source++;
        do {
            value = *source;
            *++destination = value;
        } while (*source++ != terminator);
    }
    return destination;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/u16_string", CopyU16String);

/* The second pointer advances after the terminator test. This also lets the
 * compiler rotate the loop tail into the original entry-jump layout. */
s32 CompareU16String(u16 *left, u16 *right) {
    for (;;) {
        if (*left < *right) {
            return -1;
        }
        if (*right < *left) {
            return 1;
        }
        left++;
        if (*right == 0xFFFF) {
            break;
        }
        right++;
    }
    return 0;
}

/* Compare each input position with either reference at that position.
 * On a mismatch the ordering result is against primary, never alternate.
 * All three pointers advance together, and an accepted 0xFFFF ends the scan.
 * Keep the initial jump past the advance/test tail and the reused raw value. */
s32 CompareU16StringAgainstPair(u16 *input, u16 *primary, u16 *alternate) {
    s32 raw;
    s32 current;
    s32 primaryValue, alternateValue;
    u32 maskedValue;
    s32 result;
    u16 terminator = 0xFFFF;

    goto compare;
    for (;;) {
    advance:
        if (current == terminator) goto done;
        input++;
        primary++;
        alternate++;
    compare:
        raw = *input;
        maskedValue = raw & 0xFFFF;
        current = raw;
        primaryValue = *primary;
        if (maskedValue == primaryValue) {
            result = 0;
            goto advance;
        }
        alternateValue = *alternate;
        if (maskedValue == alternateValue) {
            result = raw = 0;
            goto advance;
        }
        result = (maskedValue < primaryValue) ? -1 : 1;
        break;
    }
done:
    return result;
}
