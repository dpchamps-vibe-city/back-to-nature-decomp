#ifndef OVL11_STATE_COMMAND_DISPATCH_H
#define OVL11_STATE_COMMAND_DISPATCH_H

#include "common.h"

/* Execute one ONE-based state command. Object programs also invoke this
 * through their command table; direct callers use command 13's pair queries.
 *
 * arg1/arg2/arg3 have command-specific meanings, NOT a common operand schema.
 * Value-slot indices, record IDs and shift counts are unchecked. -1 suppresses
 * a slot output only where that command explicitly tests it. Slot reads may
 * consume a full signed word or only its low halfword. Do not replace them
 * with one generic resolver or normalize all results to boolean.
 *
 * Commands (numbers here are the public IDs, not the zero-based switch):
 *  1  arg1==1 sets partition-6 enable from arg2==1; otherwise count -> slot arg2.
 *  2  Record/flag operations selected by arg1 (0..4); some use optional slots.
 *  3  Set event flags and queue dated requests for qualifying records.
 *  4  Set the selected record's +99EC value from arg1 and mark its +9A06 flags.
 *  5  arg1 0/1 sets primary/selected-record fields and associated game flags.
 *  6  arg2!=0 tests the input mask arg1; otherwise OR arg1 into work +4A86.
 *  7  arg1 0..8 sets a work flag and clears a corresponding game flag.
 *  8  arg3==0 starts the interaction selected by arg1; otherwise query readiness.
 *  9  Set game flag 66.
 * 10  Queue a dated event with arg1 and narrowed arg2/arg3.
 * 11  Set work +44F8 bit 1.
 * 12  arg1 0..12 selects card-deal, interaction, scene or record-state operations.
 * 13  arg1 0/1/2/3 restores, tests, edits or clears selected record pairs.
 * 14  arg1==0 tests record flags; arg1==1 retires then FALLS THROUGH to 15.
 * 15  arg1 0/1 sets work +44F8 bit 400/400000 then FALLS THROUGH to 16.
 * 16  Scan twenty B8-byte records; mark the first eligible record if >=240.
 * 17  Compare arg1 to the current +30 context ID; optionally export to slot arg2.
 * 18  arg3 selects immediate/slot arg1; arg2 selects set/test of work +99D8.
 * 19  arg1!=0 sets game flag 8; otherwise tests that it is not 1.
 * 20  arg1 0/1/2 invokes the progress request service (1 exports to slot arg2).
 *     Other arg1 values FALL THROUGH to command 21.
 * 21  Forward arg1 to the record request service and test its low-halfword result.
 * 22  Increment work +523A only while below 15.
 * 23  Set work +44F8 bit 02000000.
 * 24  Wait for drawing, clear ordering tables and optionally load calendar sprites.
 * 25  Set work +44F8 bit 200000.
 * 26  Export record arg1's +9A6C value to slot arg2.
 * 27  Set work +51C0 bit 800000.
 * 28  Enable/disable animation/work flags according to arg1==0.
 * 29  Optionally export work +E77C to slot arg1; compare it using arg2/arg3.
 * 30  Apply operation arg3 with operand arg2/2 to value slot arg1.
 * 31  Apply operation arg3/operand arg2 to work +91C2; clamp to 0..255.
 *
 * Mutation-only paths generally return 1; query paths can return a raw state,
 * flag or delegated result. Unknown commands/subcommands often return 0 but
 * the documented fall-throughs still have side effects. The object-program
 * interpreter takes its success edge ONLY for result==1, not any nonzero value.
 * State, record pools, value slots and called services must be initialized.
 * Operations are synchronous here but may initiate asynchronous services;
 * repeated execution can enqueue requests or increment fields again.
 */
s32 Ovl11DispatchStateCommand(s16 command, s16 arg1, s32 arg2, s32 arg3);

#endif
