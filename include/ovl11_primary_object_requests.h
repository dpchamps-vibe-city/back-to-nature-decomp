#ifndef OVL11_PRIMARY_OBJECT_REQUESTS_H
#define OVL11_PRIMARY_OBJECT_REQUESTS_H

#include "common.h"

/* Resident ovl_11 and initialized primary-object/item-table state required.
 * These services mutate the shared object at EXE:80071A00, not a caller object.
 * State/animation codes below are raw protocol values, not gameplay names.
 * Helpers retain their existing external linkage and return contracts. */

/* Select state 1 / animation 0; clear s16 +0x26, clear flag 0x20000000 and
 * set 0x04000000. Does NOT clear the +F0 request. Always returns zero. */
s32 Ovl11ResetPrimaryObjectRequest(void);

/* Copy three halfwords to object +F0 after clearing that triplet. The first
 * input halfword indexes the item table as s16 (unchecked). Item flag 0x100
 * selects state 0xE and animation base 0x25, otherwise state 6 / base 0xB;
 * add object's unsigned +0x38 halfword. Run the ID helper and set 0x8004 in
 * object flags. Always returns zero. Aliasing +0xF0 observes the clear first. */
s32 Ovl11InstallPrimaryObjectRequest(u16 *request);

/* Without flag 0x10000000, use the reset above. Otherwise compact all eight
 * three-halfword slots at +0xC0, then index by signed +0x20 minus one
 * (unchecked). If that slot's first halfword is zero, play sound 8, select
 * state 0xC / animation +0x38 + 0x1D, and clear flag 0x8000. */
void Ovl11CheckPrimaryObjectRequestSlot(void);

/* Same clear/copy protocol, but select state 0x30 / animation +0x38 + 0x21.
 * No item-table test, ID helper or explicit flag update. As above, input
 * aliasing the destination observes its clearing. */
void Ovl11InstallPrimaryObjectState30Request(u16 *request);

#endif
