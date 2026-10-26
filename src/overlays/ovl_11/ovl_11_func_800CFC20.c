#include "common.h"
#include "game_types.h"

void ovl_11_func_800DF0F8 (s32 arg0);
s32 ovl_11_func_800CB4E0 (void);
void ovl_11_func_800CBD78 (Ovl11C7010Object *arg0);
void ovl_11_func_800E2904 (s32 arg0);

void ovl_11_func_800CFC20(void) {
    struct Ovl11ResetB8 *var_s0;
    struct Ovl11ResetB4 *var_s1;
    s32 temp_v1_2;
    s32 var_s2;

    var_s0 = (struct Ovl11ResetB8 *) &gTaggedRecordBase;
    for (var_s2 = 19; var_s2 >= 0; var_s2--) {
        if (var_s0->flags & 0x100) {
            ovl_11_func_800DF0F8((s32) var_s0);
            if ((s16) ++((struct Ovl11CFC20Status *) D_8006C838)->count >= 0x10) {
                ((struct Ovl11CFC20Status *) D_8006C838)->count = 0xFU;
            }
            ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD++;
            ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD =
                ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD >= 100 ?
                99 : ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD;
        }
        var_s0++;
    }
    var_s1 = &((struct Ovl11CFC20Status *) D_8006C838)->records[0];
    for (var_s2 = 0; var_s2 < 10; var_s2++) {
        temp_v1_2 = var_s1->flags;
        if (temp_v1_2 & 0x100) {
            if (temp_v1_2 & 0x20000) {
                ((struct Ovl11CFC20Status *) D_8006C838)->flags &= 0xEFFF7FFF;
                ovl_11_func_800CB4E0();
                ovl_11_func_800CBD78((Ovl11C7010Object *) ((struct Ovl11CFC20Status *) D_8006C838)->object);
            }
            ovl_11_func_800E2904((s32) var_s1);
            if ((s16) ++((struct Ovl11CFC20Status *) D_8006C838)->count >= 0x10) {
                ((struct Ovl11CFC20Status *) D_8006C838)->count = 0xFU;
            }
            ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD++;
            ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD =
                ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD >= 100 ?
                99 : ((struct Ovl11CFC20Status *) D_8006C838)->field_44CD;
        }
        var_s1++;
    }
}
