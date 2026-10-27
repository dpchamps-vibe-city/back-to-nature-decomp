#ifndef OVL11_TAGGED_RECORD_HANDLERS_H
#define OVL11_TAGGED_RECORD_HANDLERS_H

#include "common.h"
#include "game_types.h"

/* Construction/allocation is published separately; these APIs operate on
 * existing 0xB8-byte tagged records. None checks a supplied pointer or index. */

/* First/second pool passes: -1 for an inactive record or flag 0x400, else 0.
 * Activity updates the +AC latch, flags, and signed-low-halfword quantity.
 * Counters consumes the shared +4AC0 latch array, saturates the quantity to
 * 0..255 and counters to 7/31, updates flags/stamps, randomizes the idle
 * timer and resets the timed-sprite effect. Units are calls to each pass. */
s32 Ovl11UpdateTaggedRecordActivity(UnkStruct800E109C *record);
s32 Ovl11UpdateTaggedRecordCounters(Ovl11Deb00Object *record);

/* handlerId must be 0..19. Invoke the request callback when present; -1
 * refuses. An accepted ID with a step callback resets +28/+2A/+2C and the
 * displacement at +48. The same busy ID (flag 0x800) is refused. Always
 * clear the pending recipient at +B6, even on refusal. Returns the request
 * callback's result, or 0 when there is no request callback. */
s32 Ovl11DispatchTaggedRecordHandler(Ovl11ActionDispatchView *record,
                                    s32 handlerId);

/* -1 for tag zero. Otherwise lazily initialize sprite state, call the
 * selected step callback, and pick an idle handler after 300 updates when
 * not busy. The +2C timer increments even when a callback completes. */
s32 Ovl11StepTaggedRecordHandler(Recon_ovl_11_func_8010CE80_A0View *record);

/* Accumulate displacement +48 into position +38, then clear displacement.
 * recordAddress retains the existing word-sized address interface. */
void Ovl11ApplyTaggedRecordDisplacement(s32 recordAddress);

/* Descriptor row is +24 (0..5), column +22 (0..3). Select animation when
 * changed; frame zero means retain the existing frame. Draw with the shared
 * marker service; rows 0/5 also set sprite flag 0x100 after drawing. */
void Ovl11SelectAndDrawTaggedRecordSprite(u8 *record);

/* Unchecked descriptor lookup. animationOut is required; frameOut may be
 * null. Outputs are raw halfwords, not validated sprite IDs. Returns 0. */
s32 Ovl11GetTaggedRecordSpriteDescriptor(u16 row, u16 column,
                                       u16 *animationOut, u16 *frameOut);

#endif
