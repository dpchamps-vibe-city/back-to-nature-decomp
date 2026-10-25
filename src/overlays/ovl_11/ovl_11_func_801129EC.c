#include "common.h"
#include "game_flags.h"

/* Preserve the original caller's pointer arguments with period-style
 * declarations. D12C28 and D12B60 do not consume arguments; their original
 * parameter lists cannot be determined from their bodies alone. */
void ovl_11_func_80112C28();
void ovl_11_func_80112AB4();
void ovl_11_func_80112C98();
void ovl_11_func_80112B60();

void ovl_11_func_801129EC(void) {
    s32 shifted;
    s32 product;
    u32 sum;
    s32 upper;
    s16 idx;
    u32 base;
    s32 active;
    struct_80076220 *temp_s0;

    active = GetGameFlag(0x41);
    if (active != 0) {
        base = (s32)&D_8006C838;
        do {
            upper = 0x8000;
            base += upper;
        } while (0);
        idx = *(s16 *)(base + 0x6778);
        base = (s32)&gObjectEventSlotBase;
        product = (u32)idx << 3;
        product -= idx;
        shifted = (u32)product << 2;
        sum = shifted + idx;
        shifted = (u32)sum << 2;
        sum = shifted + idx;
        shifted = (u32)sum << 2;
        temp_s0 = (struct_80076220 *)(base + shifted);
        ovl_11_func_80112C28(temp_s0);
        ovl_11_func_80112AB4(temp_s0);
        ovl_11_func_80112C98(temp_s0);
        if (GetGameFlag(0x4F) != 0) {
            ovl_11_func_80112B60(&D_8007A3F0);
        }
    }
}
