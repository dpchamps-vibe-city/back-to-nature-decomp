#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_clock_calendar.h"
#include "ovl11_area_record_claims.h"


s32 ovl_11_func_8010CAD4 (u8 *arg0);

void ovl_11_func_8010C95C(void) {
    s32 var_s1;
    u8 *var_s0;

    var_s0 = D_8007A638;
    var_s1 = 6;
    do {
        if (!((*(u16 *) ((u8 *) var_s0 + 0xB8)) & 0x20)) {
            ovl_11_func_8010CAD4(var_s0);
        }
        var_s1 -= 1;
        var_s0 += 0xF8;
    } while (var_s1 >= 0);
}
