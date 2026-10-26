#ifndef OVL11_OBJECT_RECORD_INIT_H
#define OVL11_OBJECT_RECORD_INIT_H

#include "common.h"
#include "game_types.h"

/* Claim an untagged 0xB0-byte record. Return -1 and leave it untouched if its
 * tag is nonzero. Otherwise clear all 0xB0 bytes, store the low 16 bits of tag,
 * set marker +4 to 0xFFFF, refresh the tag-dependent sprite source and reset
 * the timed effect at +0xA8. Returns 0. Tag zero is permitted by this routine
 * and leaves the record looking unclaimed; callers supply a meaningful tag.
 * record must address the complete writable object, not just the named view. */
s32 Ovl11InitObjectRecord(Ovl11ObjectInitRecord *record, s32 tag);

/* Store the low 16 bits of tag and refresh the full object's sprite source.
 * Always returns 0. The narrow signed view preserves the original store ABI;
 * storage must still be large enough for the tag-specific source initializer. */
s32 Ovl11SetObjectRecordTag(Ovl11ObjectTagView *record, s32 tag);

#endif
