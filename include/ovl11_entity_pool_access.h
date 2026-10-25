#ifndef OVL11_ENTITY_POOL_ACCESS_H
#define OVL11_ENTITY_POOL_ACCESS_H

#include "common.h"

/* Available while ovl_11 is resident and work-area +0xDDD0 points to its
 * scene-entity pool. Indices are unchecked signed halfwords; callers must
 * supply valid entity/record indices and value slots 0..19. All return 1.
 * Coordinates use the engine's integer position units, not pixels or Q12.
 * These calls preserve unrelated record fields and flag bits. */

/* Copy the current source tag (far work +0x25476), set the position and
 * activate bit 0. Height is sign-extended from 16 bits; x and z are 32-bit.
 * Original identity: ovl_11:0x800E8960. */
s32 Ovl11ActivateEntityAtPosition(s16 entityIndex, s16 height, s32 x, s32 z);

/* Set/clear activation according to the LOW 16 bits of active.
 * Original identity: ovl_11:0x800E89CC. */
s32 Ovl11SetEntityActive(s16 entityIndex, s32 active);

/* Activate using the position in D_80076280[recordIndex] (stride 0x1D4),
 * lowered by 100 and offset horizontally by 100: direction 0=-z, 1=-x,
 * 2=+z, 3=+x. A nonzero classifyDirection selects the direction using the
 * position at work +0x52C8 instead of direction's low 16 bits. The selected
 * direction MUST be 0..3; there is no default initialization of the offset.
 * Original identity: ovl_11:0x800E8A24. */
s32 Ovl11ActivateEntityNearRecord(s16 entityIndex, s16 recordIndex,
                                 s32 direction, s32 classifyDirection);

/* Zero-extend record +2 into gOvl11ValueSlots[slotIndex]. Its meaning is
 * consumer-defined; it is not established to be an entity ID.
 * Original identity: ovl_11:0x800E8BA0. */
s32 Ovl11CopyEntityValueToSlot(s16 entityIndex, s16 slotIndex);

#endif
