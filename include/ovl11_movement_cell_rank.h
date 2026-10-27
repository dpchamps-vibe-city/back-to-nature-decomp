#ifndef OVL11_MOVEMENT_CELL_RANK_H
#define OVL11_MOVEMENT_CELL_RANK_H

#include "common.h"
#include "game_types.h"

/* Compare raw X/Z words at +38/+40 against +58/+60. Axis ranks are 0 equal,
 * 1 increasing, 2 decreasing; Z is ranked only when 3*abs(dZ) >= abs(dX).
 * Return the indexed unsigned-halfword movement code (0..8 in the original
 * table). If BOTH distances are strictly below gOvl11MovementRankThreshold
 * and EITHER is below 50, instead return 0 and clear flag 0x2000 at +34.
 * Thus zero alone does not distinguish the short-move and equal-cell paths.
 * The threshold is a signed shared setting, reset/changed by other services;
 * this function never changes it or either position. Arithmetic and input
 * range follow the original signed-word contract (no overflow checks). */
s32 Ovl11RankMovementCell(Recon_ovl_11_func_800D2594_A0View *object);

#endif
