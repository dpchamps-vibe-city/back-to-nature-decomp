#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "ovl11_small_record_handlers.h"
#include "ovl11_item_record_allocation.h"
#include "ovl11_item_id_flags.h"

typedef struct __attribute__((packed)) {
    s32 unk0;
    s32 unk4;
    s32 unk8;
    s32 unkC;
    s8 unk10;
    s8 unk11;
} Ovl11Rec800E8614;

s32 ovl_11_func_800E8614(s32 arg0) {
    s32 *var_s1_2;
    s32 var_s0_2;
    s32 i;
    s32 mask;
    u16 *temp_v0;
    u8 *temp_v1;
    u8 *var_s2;
    char *base;

    if ((arg0 << 0x10) == 0) {
        for (i = 0; i < 5; i++) {
            temp_v0 = Ovl11AllocateItemRecord(0x109, -1);
            if (temp_v0 != NULL) {
                *(s32 *)((u8 *)temp_v0 + 0x34) |= 0x40000;
                temp_v1 = (u8 *)D_8005175C + (gTextBankOffsets[1] + D_8012490C[i]);
                *(Ovl11Rec800E8614 *)((u8 *)temp_v0 + 4) = *(Ovl11Rec800E8614 *)temp_v1;
            }
        }
    } else {
        mask = 0x40000;
        base = (char *)&D_8006C838;
        var_s2 = (u8 *)(base + 0x7AB4);
        var_s1_2 = (s32 *)(base + 0x7AE8);
        var_s0_2 = 9;
        do {
            if (*var_s1_2 & mask) {
                Ovl11ClearSmallRecord((s32)var_s2);
            }
            var_s2 += 0xB4;
            var_s0_2 -= 1;
            var_s1_2 += 0x2D;
        } while (var_s0_2 >= 0);
    }
    return 1;
}
