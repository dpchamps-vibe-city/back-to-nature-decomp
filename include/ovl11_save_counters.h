#ifndef OVL11_SAVE_COUNTERS_H
#define OVL11_SAVE_COUNTERS_H

#include "common.h"

/* Names use the stable EXE script-field IDs, not speculative item meanings.
 * Additions STORE the u16 sum before capping to 999: overflow wraps before
 * the cap, so these are not wide saturating adds. kind selects the increment:
 *   field 19: kind 2 -> 6, kind 3 -> amount, otherwise 1;
 *   field 18: kind 2 -> amount, otherwise 1;
 *   field 21: kind 1 -> amount, otherwise 1;
 *   field 20: kind 1 -> 10, kind 2 -> amount, otherwise 1.
 * Take returns 1 and subtracts when sufficient, else returns 0 unchanged.
 * Taking zero succeeds. Counter storage is shared resident save data. */
void Ovl11AddSaveCounter19(u16 kind, u16 amount);
s32 Ovl11TakeSaveCounter19(u16 amount);
void Ovl11AddSaveCounter18(u16 kind, u16 amount);
s32 Ovl11TakeSaveCounter18(u16 amount);
void Ovl11AddSaveCounter21(u16 kind, u16 amount);
s32 Ovl11TakeSaveCounter21(u16 amount);
void Ovl11AddSaveCounter20(u16 kind, u16 amount);
s32 Ovl11TakeSaveCounter20(u16 amount);

#endif
