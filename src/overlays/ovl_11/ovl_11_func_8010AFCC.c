#include "common.h"
#include "game_types.h"
#include "ovl11_spawn_record_access.h"
#include "ovl11_direction_vector.h"

void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_8010AE64(Recon_ovl_11_func_800D2594_A0View *arg0);

s32 ovl_11_func_8010AFCC(Ovl11SetFieldsView *arg0) {
    Ovl11PaddedVec3 sp18;
    u16 sp28;

    if (arg0->field_34 & 0x2000) {
        if (Ovl11ReadSpawnRecord(0x65, &sp18.field_0, &sp28, 0) == 0) {
            return 2;
        }
        ovl_11_func_800D05D0(arg0, sp18);
        ovl_11_func_8010AE64((Recon_ovl_11_func_800D2594_A0View *) arg0);
        return 0;
    }
    Ovl11SetDirectionVector(4, (Recon800D0408A1View *) ((u8 *) arg0 + 0x48), 0);
    return 1;
}
