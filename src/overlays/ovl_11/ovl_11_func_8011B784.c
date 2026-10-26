#include "common.h"
#include "ovl11_halfword_triplets.h"
#include "ovl11_primary_object_requests.h"

s32 ovl_11_func_8011B784(s32 *arg0, s32 arg1, s16 arg2) {
    s16 sp10[3];
    s32 var_s2;
    u32 var_a1;

    var_s2 = 0;
    var_a1 = *arg0 - arg1;
    if (var_a1 >= 2U) {
        return 0;
    }
    switch (var_a1) {
    case 0:
        Ovl11SetHalfwordTriplet(sp10, 0xA4, arg2, 1);
        Ovl11InstallPrimaryObjectRequest((u16 *) sp10);
        var_a1++;
        break;
    case 1:
        if (gPrimaryObjectFlags & 0x04000000) {
            var_s2 = 1;
        }
        break;
    }
    *arg0 = arg1 + var_a1;
    return var_s2;
}
