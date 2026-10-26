#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "ovl11_area_resource_load.h"

void ovl_11_func_800F3FF4(s16 arg0);
void ovl_11_func_800F4CA0(void);

s32 ovl_11_func_800DE2EC(void) {
    Ovl11LoadOffsetSpriteSet(((struct struct_8006C838_button *)D_8006C838)->field_44BA);
    ovl_11_func_800F3FF4(((struct struct_8006C838_button *)D_8006C838)->field_44BA);
    ovl_11_func_800F4CA0();
    return 1;
}
