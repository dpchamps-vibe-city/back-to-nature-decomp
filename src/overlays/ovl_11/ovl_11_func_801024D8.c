#include "common.h"
#include "integer_power.h"
#include "psyq/memory.h"
#include "ovl11_texture_cell_cache.h"

void func_80024810(s32 arg0, s16 arg1, s16 arg2);
void func_80024860(s32 arg0, s16 arg1, s16 arg2);
s8 ovl_11_func_800D583C(s16 arg0);
void ovl_11_func_800D6F78(s16 arg0, s16 arg1, s32 *arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800F6F7C(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F7010(s32 arg0, s16 arg1, s16 arg2);
void ovl_11_func_800F7094(s32 arg0, s16 arg1, s16 arg2, s32 arg3);

void ovl_11_func_801024D8(void) {
    u16 x;
    u16 y;
    s32 i;
    s32 j;
    s32 count;
    s32 type;
    s32 table;
    s32 *context0;
    s32 *context1;
    s32 *context2;
    Ovl11ItemState *slot;

    context0 = (s32 *)(gActiveRenderContext->field_D8 + 0x60);
    context2 = (s32 *)(gActiveRenderContext->field_D8 + 0x68);
    context1 = (s32 *)(gActiveRenderContext->field_D8 + 0x64);
    x = 0x84;
    y = 0x5E;
    if (D_801273EA_table == 0) {
        slot = (Ovl11ItemState *)&Ovl11Items_80071AC0;
        count = pow_int(2, slot[-27].id + 1);
    } else {
        table = D_801273EA_table - 1;
        slot = D_80070D42_slots[table];
        count = 8;
    }
    func_80024810((s32)context0, 0x75, 0x90);
    func_80024860((s32)context0, 0x10C, 0x90);
    for (i = 0; i < 8; i++, slot++, x += 0x22) {
        if (x >= 0xEBU) {
            x = 0x84;
            y += 0x22;
        }
        if (i >= count) {
            ovl_11_func_800F7010((s32)context2, x, y);
        } else {
            if (D_801273EA_table == 0) {
                if (slot->id == 0) {
                    ovl_11_func_800F7010((s32)context2, x, y);
                } else {
                    ovl_11_func_800D6F78(slot->id, 0, context2, x, y, 8);
                }
            } else {
                Ovl11DrawTextureCell(context2, (u16)slot->id, x, y);
            }
            if (slot->id != 0) {
                type = ovl_11_func_800D583C(slot->id);
                if (D_801273EA_table == 0) {
                    if (type == 2) {
                        ovl_11_func_800F7094((s32)context1, x, y, slot->unk4);
                    }
                } else {
                    if (type == 0 || type == 2) {
                        ovl_11_func_800F7094((s32)context1, x, y, slot->unk4);
                    }
                }
                for (j = 0; j < 8; j++) {
                    if (D_8012CF00_ids[j] == slot->id) {
                        break;
                    }
                }
                if (j != 8 && (u16)slot->id != 0) {
                    ovl_11_func_800F6F7C((s32)context0, x, y);
                }
            }
        }
    }
}
