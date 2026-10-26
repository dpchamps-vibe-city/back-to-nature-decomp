#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl23_record_control_fields.h"


void ovl_23_func_800BA610 (Ovl23Func800BB0C8Arg *arg0);

extern Ovl23Func800BB0C8Arg D_800BFB08;
extern s16 D_800BFB0E;

void ovl_23_func_800B8B94(void) {
    s16 var_a2;
    s32 temp_v1;
    s32 var_a1;

    temp_v1 = *(s32 *) ((u8 *) gPad0Input + 4);
    var_a1 = 2;
    if (temp_v1 & 0x20) {
        var_a1 = 3;
    }
    var_a2 = 2;
    if (!(temp_v1 & 0x8000)) {
        var_a2 = 6;
        if (!(temp_v1 & 0x2000)) {
            if (var_a1 == 2) {
                var_a1 = 1;
            }
            var_a2 = D_800BFB0E;
        }
    }
    Ovl23SetAnchorRecordControl(&D_800BFB08, var_a1, (s32) var_a2, 0);
    ovl_23_func_800BA610(&D_800BFB08);
}
/* Warning: struct GfxObj is not defined (only forward-declared) */
