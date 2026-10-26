#ifndef OVL11_PRIMARY_OBJECT_SPRITE_RECORDS_H
#define OVL11_PRIMARY_OBJECT_SPRITE_RECORDS_H

#include "common.h"

/* Preload sprite data for the ID halfword of each six-byte record: two at
 * primary-object +84, eight at +C0, then eight at +90. Record order and all
 * calls, including empty IDs, are preserved; no table entries are changed. */
void Ovl11LoadPrimaryObjectRecordSprites(void);

/* Clear collision-output/substitute-input state; under scene flag 0x8000,
 * seed the default +90 record set. Preload all three record sets, install the
 * scene-selected spawn position, and clear three shared action words. Uses
 * shared primary-object/work RAM and the active asset staging buffer. */
void Ovl11InitPrimaryObjectRecordSprites(void);

#endif
