/*
 * Classify the X/Z displacement between two recorded object positions.
 * The 3x3 table maps signed axis ranks to a movement-cell code; a sufficiently
 * short move instead returns zero and clears the object's 0x2000 flag.
 * Public contract: ovl11_movement_cell_rank.h.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */
#include "common.h"
#include "game_types.h"
#include "ovl11_movement_cell_rank.h"

s32 Ovl11RankMovementCell(Recon_ovl_11_func_800D2594_A0View *object) {
    /* This is a real GCC nested definition in the original: its entry saves
     * the incoming static chain, and the caller supplies that chain. Keeping
     * it nested also preserves the parent's table-address formation. */
    s32 Ovl11CompareMovementAxis(s32 from, s32 to) {
        if (from == to) {
            return 0;
        }
        if (from < to) {
            return 1;
        }
        return 2;
    }
    s32 x0;
    s32 x1;
    s32 z0;
    s32 z1;
    s32 dx;
    s32 dz;
    s32 xRank;
    s32 zRank;
    u16 result;

    z0 = *(s32 *) ((u8 *) object + 0x40);
    z1 = *(s32 *) ((u8 *) object + 0x60);
    x0 = *(s32 *) ((u8 *) object + 0x38);
    x1 = *(s32 *) ((u8 *) object + 0x58);
    dx = __builtin_abs(x1 - x0);
    dz = __builtin_abs(z1 - z0);
    xRank = Ovl11CompareMovementAxis(x0, x1);
    zRank = 0;
    if (dz * 3 >= dx) {
        zRank = Ovl11CompareMovementAxis(z0, z1);
    }
    if (dx < gOvl11MovementRankThreshold && dz < gOvl11MovementRankThreshold && (dx < 50 || dz < 50)) {
        result = 0;
        object->unk34 &= ~0x2000;
    } else {
        result = gOvl11MovementCellRanks[xRank * 3 + zRank];
    }
    return result;
}
