#ifndef OVL11_RECORD_SPRITE_OFFSET_DECAY_H
#define OVL11_RECORD_SPRITE_OFFSET_DECAY_H

#include "common.h"

/* Service the installed current record once after drawing. With record flag
 * 0x100 set and shared-work +C bit 0x08000000 clear, decrement a nonzero
 * gOvl11RecordSpriteYOffsetIndex, or clear flag 0x100 if it is already zero.
 * Suppression leaves both unchanged. There is no clamping or pointer check;
 * normal renderer use requires a valid record and index in 0..3.
 */
void Ovl11DecayCurrentRecordSpriteOffset(void);

#endif
