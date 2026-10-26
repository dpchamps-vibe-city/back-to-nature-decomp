#ifndef OVL11_SELECTED_RECORD_INTERACTION_H
#define OVL11_SELECTED_RECORD_INTERACTION_H

#include "common.h"

/* Drive the shared interaction-step/message/confirmation flow. Return 1 only
 * at terminal step 7, otherwise 0. Interaction 0x109 asks for confirmation;
 * accepting adds 2000 (selected record's flag 0x40 set) or 500 through the
 * common value-accounting routine, clears the entire selected 0xB4-byte record,
 * then waits for message/record-transition completion. 0x10A only reports a
 * message.
 *
 * The pool has ten entries; bit 0x20000 selects one, with LAST match winning.
 * At least one flagged entry is required before the 0x109 path: the flag-0x40
 * helper does not guard its -1 no-match index. Requires initialized message,
 * render and shared interaction-step state. Each call clears work words
 * +522C/+5230 and sets flag 0x800000 at +5234; step 7 clears that flag.
 * Other step/message/portrait and last-record effects are shared engine state;
 * not reentrant. */
s32 Ovl11UpdateSelectedRecordRelease(void);

#endif
