/*
 * tagged_record_choice — reconstructed translation unit.
 *
 * Paired tagged-record selection facades: expose the family-local random
 * index and apply the code-0x15B gated value adjustment to a selected record.
 * Both use the same specialized two-family selector; the record banks remain
 * external.
 *
 * Original ovl_11:800D3230-800D3390. These facades share the specialized
 * two-bank selector; they do not own either record bank. Selection consumes
 * RNG state. The adjustment deliberately ignores selected index zero.
 * Membership and conservative boundaries are recorded in notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_tagged_record_choice.h"
#include "ovl11_record_selection.h"

s32 Ovl11ChooseTaggedRecordIndex(u16 family) {
    s32 selectedIndex;
    selectedIndex = Ovl11ChooseEligibleTaggedRecord(family);
    return selectedIndex;
}

s32 Ovl11ApplyTaggedRecordValueAdjustment(u16 *recordCode) {
    s32 second;
    s32 index;
    u8 *state;
    struct Ovl11SmallTaggedValueView *base;
    u16 *value;
    s32 current;

    if (*recordCode != 0x15B) {
        return -1;
    }
    if (Ovl11IsClockAdvanceActive() == 1) {
        state = (u8 *)D_8006C838;
        if (*(s32 *)(state + 0x44F8) & 0x01000000) {
            if (*(s16 *)(state + 0x44C0) >= 7) {
                second = 0;
                index = Ovl11ChooseEligibleTaggedRecord(0);
                if (index < 0) {
                    second = 1;
                    index = Ovl11ChooseEligibleTaggedRecord(1);
                }
                /* Zero is a valid selector result, but neither bank's
                 * first record is changed by this operation. */
                if (second == 0) {
                    if (index > 0) {
                        s32 scale;
                        value = (u16 *)((u8 *)&gTaggedRecordValueBase + index * 0xB8);
                        scale = (s16)gTaggedRecordValueBase.scale;
                        current = *value - 10;
                        current += scale / 50;
                        *value = current;
                    }
                } else if (index > 0) {
                    s32 scale;
                    base = (struct Ovl11SmallTaggedValueView *)(state + 0x7ACA);
                    value = (u16 *)((u8 *)base + index * 0xB4);
                    scale = (s16)base->scale;
                    current = *value - 10;
                    current += scale / 50;
                    *value = current;
                }
            }
        }
    }
    return 0;
}
