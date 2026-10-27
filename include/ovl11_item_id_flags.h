#ifndef OVL11_ITEM_ID_FLAGS_H
#define OVL11_ITEM_ID_FLAGS_H

#include "common.h"

/* record must provide an aligned u16 item ID at +0 and u32 flags at +0x34.
 * IDs 0x162/0x163 gain bit 0x80000000. IDs 0x113..0x11A and
 * 0x153..0x157 gain bit 0x1000. Other IDs leave the word unchanged.
 * Existing bits are never cleared; changing an ID requires any old flags
 * to be cleared separately. No other record fields or state are changed. */
void Ovl11ApplyItemIdFlags(void *record);

#endif
