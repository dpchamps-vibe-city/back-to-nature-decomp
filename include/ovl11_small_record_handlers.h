#ifndef OVL11_SMALL_RECORD_HANDLERS_H
#define OVL11_SMALL_RECORD_HANDLERS_H

#include "common.h"
#include "game_types.h"

/* Original ovl_11:800E1F9C. Accept codes 0x109/0x10A only, otherwise NULL.
 * mainPool == 1 selects the ten-entry gSmallTaggedRecordBase pool: index -1
 * finds a free record, other indices are unchecked. The resulting record is
 * initialized and row placement is applied. An explicit occupied slot is NOT
 * rejected by the factory (the initializer's failure is ignored).
 * Other mainPool values scan three auxiliary 0xB4 records for a zero tag.
 * Returns borrowed storage, NULL when no slot is available. No allocation. */
s16 *Ovl11AllocateSmallRecord(s32 code, s32 index, s32 mainPool);

/* Original E2904/E2968/E2934. Clear zeroes 0xB4 bytes. Init returns -1
 * without writing if the tag is nonzero, else clears and seeds counters,
 * date/state, sprite source and effect, returning 0. Codes are not checked
 * by Init. GetIndex recognizes exact starts of the ten main-pool records,
 * returning 0..9 or -1 (including auxiliary-pool addresses).
 * Address-valued s32 parameters retain the existing 32-bit ABI. */
void Ovl11ClearSmallRecord(s32 recordAddress);
s32 Ovl11InitSmallRecord(s16 *record, s32 code);
s32 Ovl11GetSmallRecordIndex(void *record);

/* Original E20F0/E21E4. Lifecycle/status updates: return -1 for inactive
 * records (subject to the state updater's flag-02000000 exception), else 0.
 * Kind1 handles code-109 +30==1 counters/energy and time-window sequence
 * release. State updates energy, age/probability flags, sprite/effect state
 * and row placements, and may publish a spawn record. These are distinct
 * update passes, not interchangeable per-frame animation stepping. */
s32 Ovl11UpdateSmallRecordKind1(u16 *record);
s32 Ovl11UpdateSmallRecordState(E21E4Object *record);

/* Original E276C/E2824. Step calls the +26 slot, may select a new handler
 * after 300 elapsed updates, then increments the s16 timer. Returns -1 for a
 * zero tag, otherwise 0. Dispatch requires a valid 0..19 id, calls its enter
 * slot, and on a non--1 result with a step slot resets +26/+28/+2A/+2C and
 * clears the displacement vector. Re-entering the same locked handler gives
 * -1. No bounds checks: caller must provide a full initialized 0xB4 record. */
s32 Ovl11StepSmallRecordHandler(s16 *record);
s32 Ovl11DispatchSmallRecordHandler(u16 *record, s32 handlerId);

/* Original E1EC0/E1F70/E2A30/E2AAC. Draw selects the descriptor's animation
 * and optional nonzero frame, draws with marker, then sets sprite flag 0100
 * for rows 0/5. Displacement adds the +48 vector to +38 and clears +48.
 * Refresh derives code from +B0 and installs the fixed-id sprite source;
 * returns -1 for an unsupported code, otherwise 0. Descriptor lookup takes
 * row 0..6 and direction 0..3, requires animationOut and optionally writes
 * frameOut; returns 0. All indices/pointers are unchecked. */
void Ovl11SelectAndDrawSmallRecordSprite(u8 *record);
void Ovl11ApplySmallRecordDisplacement(s32 recordAddress);
s32 Ovl11RefreshSmallRecordSprite(s16 *record);
s32 Ovl11GetSmallRecordSpriteDescriptor(u16 row, u16 direction,
                                      u16 *animationOut, u16 *frameOut);

#endif
