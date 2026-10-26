#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_item_table_queries.h"

s32 ovl_11_func_800F8404(s32 arg0);
void ovl_11_func_800F8428(u16 *arg0, u16 *arg1);

s32 ovl_11_func_800F8188(u16 *arg0, s32 arg1, u16 *arg2, s32 arg3) {
    s32 temp_v0;

    temp_v0 = ovl_11_func_800F8404(arg3);
    if (temp_v0 >= 0) {
        if (temp_v0 >= 2) {
            if (temp_v0 != 2) {
                return 0;
            }
            goto block_5;
        }
        if (Ovl11GetItemType((s16) *arg0) == 2) {
block_5:
            if (Ovl11GetItemType((s16) *arg0) != 1) {
                ovl_11_func_800F8428(arg0, arg2);
                return 1;
            }
            goto block_7;
        }
        /* Duplicate return node #8. Try simplifying control flow for better match */
        return 0;
    }
block_7:
    return 0;
}
