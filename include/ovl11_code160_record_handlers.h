#ifndef OVL11_CODE160_RECORD_HANDLERS_H
#define OVL11_CODE160_RECORD_HANDLERS_H

#include "game_types.h"

/* Lifecycle/handler bank for resident 0xB8-byte records, including codes
 * 0x160..0x162. These are partial views of the same record representation;
 * callers must provide the full record and initialized work/sprite tables.
 * Mutating services share work/pool state and provide no synchronization. */

/* Pool pass one and pass two respectively: liveness/condition gates, then
 * opacity, counters, one-shot effects and flag updates. Return -1 when the
 * code is zero or +34 bit 0x10000 suppresses activity. Opacity is a 0..255
 * halfword; counters are update counts, not milliseconds. */
s32 Ovl11UpdateCode160RecordLiveness(UnkStruct800DF4F0 *record);
s32 Ovl11StepCode160RecordFrame(Ovl11Deb00Object *record);

/* Invoke the selected animation callback, refresh sprite source as required,
 * and service the 300-update handler timer. Returns -1 for inactive records,
 * otherwise 0. Loading tables and handler indices are unchecked. */
s32 Ovl11TickCode160Record(UnkStruct800DF4F0 *record);

/* handlerId indexes both 20-entry action/animation banks. A repeated action
 * with flag 0x800 is suppressed (-1); otherwise return its action result
 * (zero when no action exists). A present animation slot and non--1 result
 * record the handler, zero +28/+2A/+2C, and clear the +48 movement vector.
 * Always writes -1 at +B6, including suppression. It does not call the
 * animation callback here; the tick service does that separately. */
s32 Ovl11DispatchCode160Handler(Ovl11Code160HandlerRecord *record, s32 handlerId);

/* Clear all 0xB8 bytes, regardless of activity; the address uses the existing
 * integer ABI. Initialization refuses an active record (-1), otherwise clears
 * and seeds code, opacity 10, copied work tags, sprite source and effect.
 * The initializer accepts the existing s32 code, narrowed in the record. */
void Ovl11ClearCode160Record(s32 recordAddress);
s32 Ovl11InitCode160Record(char *record, s32 code);

/* Select the source from the record's code/age state, including texture
 * offsets and one-shot source flags. Returns zero (s16), not the source
 * index. The resident blob and offset table must already be loaded. */
s16 Ovl11SelectCode160RecordSpriteSource(Recon_ovl_11_func_800DEEE0_A0View *record);

/* Pure nine-row/four-variant descriptor lookup. Indices unchecked; animation
 * is required, frame may be NULL. Returns zero. The presentation caller
 * interprets frame zero as "leave current frame", not this reader. */
s32 Ovl11ReadCode160SpriteDescriptor(u16 state, u16 variant,
                                   u16 *animation, u16 *frame);

#endif
