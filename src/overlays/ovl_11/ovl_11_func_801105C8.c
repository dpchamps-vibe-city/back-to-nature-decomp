#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_geometry.h"
#include "ovl11_grid_candidate_search.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);

s32 ovl_11_func_801105C8(Ovl11SetFieldsView *arg0, s32 arg1, s32 arg2) {
    Ovl11PaddedVec3 sp18;
    s16 sp28;
    s16 sp2A;
    s32 temp;

    temp = arg1 != 1;
    if (Ovl11FindCandidateGridCell(&sp28, &sp2A, arg1, arg2, 1) != 0) {
        return 1;
    }
    Ovl11GetCellCornerPosition(sp28, sp2A, (Vec3 *) &sp18, temp);
    ovl_11_func_800D05D0(arg0, sp18);
    return 0;
}
