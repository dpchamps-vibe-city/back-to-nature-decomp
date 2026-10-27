#ifndef OVL11_INTERACTION_HIT_ACTION_H
#define OVL11_INTERACTION_HIT_ACTION_H

#include "common.h"
#include "game_types.h"

/* Dispatch the shared hit flags/record, using object's unsigned +3C code
 * only for the category-800 placement calculation. Flag priority is
 * 80,400,2,4,8,100,200,800 (hex); this is not a bitwise broadcast.
 * mode indexes the ten-halfword action table on applicable paths: caller
 * must keep it in range. Mode zero may increment the shared action count
 * up to 15, play audio, or initiate a record action. Other modes may invoke
 * handlers/set states. The shared hit result is not cleared.
 * Returns 1 for a consumed category, even when that mode performs no action;
 * category 800 returns 0 unless its specific mode/record guards succeed.
 * Requires a current valid hit result; non-reentrant shared state. */
s32 Ovl11DispatchInteractionHitAction(
    Recon_ovl_11_func_800CCFF8_A0View *object, s32 mode);

#endif
