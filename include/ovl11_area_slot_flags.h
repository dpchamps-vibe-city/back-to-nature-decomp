#ifndef OVL11_AREA_SLOT_FLAGS_H
#define OVL11_AREA_SLOT_FLAGS_H

#include "common.h"

/* Indexed byte flags in the shared game-state work area, used by area-4/5
 * callers. bank == 1 selects the 22-byte bank at +0x4AC0; ALL other values
 * select the 10-byte bank at +0x4AD6. The slot must fit the selected bank;
 * neither entry checks it. No allocation or other persistent effects.
 * The setter writes 1; the query returns exactly 0 or 1 for any byte value. */
void Ovl11SetAreaSlotFlag(s32 slot, s32 bank);
s32 Ovl11IsAreaSlotFlagSet(s32 slot, s32 bank);

#endif
