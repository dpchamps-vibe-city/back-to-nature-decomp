#include "common.h"
#include "game_types.h"

/* Preserve the names already used by generated function context. */
typedef Ovl11SetFieldsView Ov11SetFields;
typedef Ovl11PaddedVec3 Ov11SetVec;

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v) {
    arg0->field_58 = v.vx;
    arg0->field_5C = v.vy;
    arg0->field_60 = v.vz;
    arg0->field_34 = (arg0->field_34 | 0x2000) & ~0x4000;
}
