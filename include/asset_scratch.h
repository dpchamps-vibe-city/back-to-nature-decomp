#ifndef ASSET_SCRATCH_H
#define ASSET_SCRATCH_H

#include "common.h"

/*
 * Read-only queries for the asset-loading scratch interval below the graphics
 * heap. All three take no arguments and read the current boot/loader state;
 * they neither allocate memory nor reserve it against another user.
 *
 * Start/end are byte addresses carried as s32, preserving the existing ABI.
 * The interval is [start, end). Callers must initialize the graphics heap and
 * load-layout state first, check capacity before use, and coordinate reuse.
 * No bounds clamping, alignment adjustment or persistent-content guarantee is
 * provided. In particular, a negative capacity is not converted to zero.
 */

/* Available byte count. Original identity: exe:0x8001205C. */
s32 GetAssetScratchCapacity(void);

/* First usable address, after the loaded region and fixed reservation.
 * Original identity: exe:0x80012098. */
s32 GetAssetScratchStart(void);

/* Exclusive end address (the graphics-heap base).
 * Original identity: exe:0x800120BC, formerly GetVal8005E3B0. */
s32 GetAssetScratchEnd(void);

#endif
