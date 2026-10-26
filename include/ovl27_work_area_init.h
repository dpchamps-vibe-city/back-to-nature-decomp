#ifndef OVL27_WORK_AREA_INIT_H
#define OVL27_WORK_AREA_INIT_H
#include "common.h"

/* Resident title-overlay initialization services. They write shared engine
 * work state; they allocate nothing and do not reset the whole work image. */

/* Fill 25 rows of 45 eight-byte entries at work+0x55C4 from a resident
 * halfword-ID table. 1/2/3 choose those tables; every other index chooses 0.
 * IDs 0x22..0x30 resolve a byte through the initialized work+0x20/+0x2C pools
 * and use alternate ID 0x168. Other entries retain ID 0x167 in their second
 * halfword. Requires those pools to be bound; table contents are unchecked. */
void Ovl27InitSpriteFrameRecords(s32 tableIndex);

/* Install the primary object's field/slot defaults, including 100/100,
 * word +0x5C = 500, zeroed request triples and -1 sentinels. This is a selective
 * initializer, not memset of the complete object. */
void Ovl27InitPrimaryObject(void);

/* Write work+E4CC/E4CE/E4D0/E4D2 halfwords to 0/1/1/1 and select stereo. */
void Ovl27InitWorkOptions(void);

/* Copy five halfword-aligned six-byte records to work+0x468A (30 bytes). */
void Ovl27CopyInitialWorkRecords(void);
#endif
