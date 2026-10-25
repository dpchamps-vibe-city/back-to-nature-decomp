#ifndef GAME_FLAGS_H
#define GAME_FLAGS_H

#include "common.h"

/* Access the flag bank in D_8006C838 at +0x38; state memory must be initialized.
 * IDs select word id/32, bit id%32. Neither operation checks the bank's extent:
 * callers must supply a valid ID. The getter uses only the low 16 bits of its
 * u32 argument and returns exactly 0 or 1. The setter's existing u16 ABI treats
 * zero as clear and any nonzero value as set, preserving all other bits.
 * No allocation, persistence or side-effect notification is performed. */
s32 GetGameFlag(u32 flagId);
void SetGameFlag(u16 flagId, u16 enabled);

#endif
