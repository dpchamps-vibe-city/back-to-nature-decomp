#include "common.h"
#include "game_types.h"
#include "ovl11_primary_object_requests.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"

void ovl_11_func_800CD08C(Recon_ovl_11_func_800CD08C_A0View *arg0) {
    arg0->unk6C = arg0->unk6C & -0x1001;
    Ovl11InitAreaSpriteSource();
    Ovl11ResetPrimaryObjectRequest();
}
