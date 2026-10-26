#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "ovl11_tagged_record_creation.h"


s8 *ovl_11_func_800DE924 (s32 arg0, s32 arg1);
void ovl_11_func_800E2904 (s32 arg0);
u16 *ovl_11_func_800CE744 (s32 arg0, s32 arg1);
void ovl_11_func_800D2D54 (void *arg0);
s32 ovl_11_func_800DF128 (char *arg0, s32 arg1);
s32 ovl_11_func_800E0D3C (UnkStruct800E109C *arg0, s32 arg1);

void ovl_11_func_800D09E0(s32 arg0) {
    s16 temp_a0;
    s32 temp_s0;
    s32 temp_a1;
    s32 offset;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v1;
    s32 x, y;
    Ovl11D09E0Object *temp_a0_2;
    u8 *var_v0;
    u8 *work;
    u8 *state;
    u8 *record_base;
    u8 *work2;
    u8 *record_base2;

    if (arg0 == 1) {
        arg0 = 0;
        state = (u8 *) D_8006C838;
        work = D_80074838;
        record_base = work + 0x1F0;
loop_2:
        var_v1 = arg0 * 4;
        var_v0 = work + var_v1;
loop_3:
        temp_a0 = ((struct Ovl11D09E0SelectionView *) var_v0)->kind;
        var_v1 = (s32)var_v0;
        var_v0 = record_base + temp_a0 * 0xB8;
        do {
            temp_a1 = *(s32 *)var_v0;
        } while (0);
        temp_s0 = ((struct Ovl11D09E0SelectionView *) var_v1)->slot;
        if (temp_a1 & 0x200) {
            if (temp_a1 & 0x10000) {
                Ovl11InitOffsetRecordCopy((s32) temp_a0, (s32) temp_s0);
                arg0 += 1;
            } else {
                ovl_11_func_800DE924((s32) temp_a0, (s32) temp_s0);
                goto block_7;
            }
        } else {
block_7:
            arg0 += 1;
        }
        if (arg0 < 2) {
            var_v1 = arg0 * 4;
            if (arg0 == 1) {
                if (((Ovl11Status99C8View *) state)->field_44D0 != 0) {
                    goto loop_2;
                }
                goto done;
            }
            goto loop_2;
        }
    } else {
        arg0 = 2;
        work2 = D_80074838;
        record_base = (u8 *) D_8006C838;
        record_base2 = record_base + 0x7AB4;
        work = record_base + 0x99D2;
        x = -0x640;
        y = -0xC8;
loop_13:
        var_v0_2 = arg0 * 4;
        temp_s0 = ((struct Ovl11D09E0SelectionView *) (work2 + var_v0_2))->slot;
        offset = temp_s0 * 0xB4;
        temp_a1 = offset;
        if (((struct Ovl11PoolResetView *) (offset + (s32)record_base))->records_7AB4[0].type == 0x10A) {
            ovl_11_func_800E2904(temp_a1 + (s32) record_base2);
            temp_a0_2 = (Ovl11D09E0Object *) ovl_11_func_800CE744(0x10A, (s32) temp_s0);
            if (*(s16 *)work == 2) {
                temp_a0_2->flags |= 0x40000000;
            }
            temp_a0_2->state = 3;
            var_v0_3 = 0x514;
            if (arg0 == 2) {
                var_v0_3 = -0x44C;
            }
            temp_a0_2->position.x = x;
            temp_a0_2->position.y = y;
            temp_a0_2->position.z = var_v0_3;
        }
        arg0 += 1;
        work += 4;
        if ((arg0 < 4) && ((var_v0_2 = arg0 * 4, (arg0 != 3)) || (var_v0_2 = arg0 * 4, (((Ovl11Status99C8View *) record_base)->field_44D2 != 0)))) {
            goto loop_13;
        }
    }
done:
    return;
}
