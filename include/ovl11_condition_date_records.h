#ifndef OVL11_CONDITION_DATE_RECORDS_H
#define OVL11_CONDITION_DATE_RECORDS_H

#include "common.h"

/* Overlay-11 dated-condition lifecycle. Slots 0..4 address the five records;
 * -1 selects the singleton for classify/snapshot. Indices are unchecked.
 * Dates are year / 0-based season (0..3) / 0-based day (0..29).
 *
 * Reset clears flags 0x26..0x2A, classifies all five slots and snapshots those
 * returning 1; it also performs the season-0/day-7 conditional state reset.
 * Classify returns 0 (not eligible), 1 (eligible) or 2 (alternate condition).
 * Snapshot copies all four current calendar halfwords, updates completion
 * flags, and marks the two referenced records for ordinary slots. Slot -1
 * instead updates the singleton flags and selected counter record.
 *
 * Assign uses bank 0 for ordinary slots, nonzero for the singleton. Selector
 * 0..4 maps to indices 3/7/9/14/20 unless bank is 2 (direct second index).
 * Selector 5 forces bank 0/slot 4 and sets two additional record bits.
 * Ordinary dates begin seven days ahead and skip calendar-table dates; slot 4
 * uses next year, season 0/day 29. The date predicate's return is NOT used by
 * that search. Singleton assignment also clears the chosen ordinary slot.
 * These operations change shared work state and game flags, not calendar time.
 */
void Ovl11ResetConditionDateRecords(void);
s32 Ovl11ClassifyConditionDateSlot(s16 slot);
void Ovl11SnapshotConditionDateSlot(s16 slot);
void Ovl11AssignConditionDateSlot(s16 bank, s16 selector);

#endif
