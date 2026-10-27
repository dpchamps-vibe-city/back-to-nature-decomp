#ifndef OVL11_AREA_SELECTION_COMMAND_H
#define OVL11_AREA_SELECTION_COMMAND_H

#include "common.h"

/* Post area-selection mode 0x18; return 1 immediately, not after a load.
 * selectionCode is a signed selection or one of these state-derived selectors:
 *  -1: default-area selection minus the current season;
 *  -2: 0x50 + four times the current variant;
 *  -3: 0x6C/68/64/60 (status bits 100/200, game flag 3C, fallback);
 *  -4: 0x78/74/70 (status bit 100000, game flag 3A, fallback).
 * All other codes, including other negative values, are used unchanged.
 *
 * absoluteSelection==0 ADDS the current signed season after this selection;
 * any nonzero value suppresses that addition. Thus -1 yields the actual
 * default only in relative mode. The two final arguments independently
 * set (nonzero) or clear (zero) bits 2000/4000 in work +4450; other bits stay.
 * Their wider gameplay meanings are not established.
 *
 * Writes selection to far work +25480 and mode to +254A0; a later call can
 * overwrite an outstanding request. Shared area state must be initialized.
 */
s32 Ovl11RequestAreaSelectionCommand(s16 selectionCode, s16 absoluteSelection,
                                    s32 flag2000, s32 flag4000);

#endif
