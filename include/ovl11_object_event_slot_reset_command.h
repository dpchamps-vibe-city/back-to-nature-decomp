#ifndef OVL11_OBJECT_EVENT_SLOT_RESET_COMMAND_H
#define OVL11_OBJECT_EVENT_SLOT_RESET_COMMAND_H

#include "common.h"

/* ovl_11: reset one unchecked signed slot in the 0x1D4-stride event pool.
 * Set +2C/+2E to 0x36; clear halfwords +08/+0A/+0C/+0E/+10/+14/+16/+18/
 * +1A/+1C/+22; set flags +1E to (old | 0x8000) & 0x9E48. Reset the timed
 * sprite effect at +E0. Fill all thirty 8-byte schedule entries at +E4 with
 * halfwords {0,0,0x36,0x36}. This is NOT a memset: all other fields survive.
 * Offsets above are hexadecimal. Always returns 1; last three operands
 * unused. Initialize pool storage first. Original identity: ovl_11:800E9B68. */
s32 Ovl11ResetObjectEventSlotCommand(s16 slot, s32 unused1, s32 unused2, s32 unused3);

#endif
