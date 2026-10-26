#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_requests.h"

void ovl_11_func_800BD168(void);

void ovl_11_func_800CD08C(Recon_ovl_11_func_800CD08C_A0View *arg0) {
    arg0->unk6C = arg0->unk6C & -0x1001;
    ovl_11_func_800BD168();
    Ovl11ResetPrimaryObjectRequest();
}
