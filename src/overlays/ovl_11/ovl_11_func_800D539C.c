#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sound_commands.h"
#include "psyq/memory.h"
#include "ovl11_record_selection.h"

typedef struct {
    char pad_00[0x24];
    s16 unk24;
    char pad_26[0x30 - 0x26];
    s16 unk30;
    char pad_32[0x7A - 0x32];
    u16 unk7A;
} Ovl11D539CStepObj;

void ovl_11_func_800D0408();
void ovl_11_func_800D05D0(Ovl11SetFieldsView *arg0, Ovl11PaddedVec3 v);
s32 ovl_11_func_800E2824(u16 *arg0, s32 arg1);
s32 ovl_11_func_800D3104(u16 *arg0, s32 arg1);
s32 ovl_11_func_800D04D4(Ovl11D539CStepObj *arg0, u8 *arg1, s16 *arg2, s32 arg3, u16 *arg4);
s32 ovl_11_func_800E0C24(Ovl11SetFieldsView *arg0, s32 arg1);
s32 ovl_11_func_800DF010(Ovl11SetFieldsView *arg0, s32 arg1);

typedef struct {
    char pad_00[2];
    u16 index;
    char pad_04[0x16 - 4];
    s16 pool;
    char pad_18[0x22 - 0x18];
    u16 direction;
    char pad_24[4];
    s16 state;
    u16 step;
    s16 timer;
    char pad_2E[2];
    s16 map;
    char pad_32[2];
    s32 flags;
    s32 x;
    s32 y;
    s32 z;
} Ovl11D539CObject;

s32 ovl_11_func_800D539C(Ovl11D539CObject *arg0) {
    s32 sp18[4];
    Ovl11SetFieldsView *var_s0;
    s16 temp_v1;
    s32 (*var_s2)(Ovl11SetFieldsView *, s32);
    s32 var_v0;
    s32 result;
    u8 *far_base;
    s32 x;

    temp_v1 = arg0->state;
    result = 0;
    switch (temp_v1) {
    case 0:
        ovl_11_func_800D0408(arg0->direction, (Recon800D0408A1View *)&sp18, 0x7D0);
        sp18[0] += arg0->x;
        sp18[1] += arg0->y;
        sp18[2] += arg0->z;
        if ((arg0->pool) == 0) {
            var_s0 = (Ovl11SetFieldsView *) ((u8 *) &gTaggedRecordBase + ((arg0->index) * 0xB8));
            if (var_s0->field_34 & 0x10000) {
                var_s2 = ovl_11_func_800E0C24;
            } else {
                var_s2 = ovl_11_func_800DF010;
            }
        } else {
            var_s2 = 0;
            var_s0 = (Ovl11SetFieldsView *) ((u8 *) &gSmallTaggedRecordBase + ((arg0->index) * 0xB4));
        }
        if (var_s0->field_30 == 1) {
            ovl_11_func_800D05D0(var_s0, *(Ovl11PaddedVec3 *)&sp18);
            (*(u16 *) ((u8 *) var_s0 + 0x22)) = (arg0->direction);
            if (var_s2 != 0) {
                var_s2(var_s0, 3);
            } else {
                ovl_11_func_800E2824((u16 *) var_s0, 3);
            }
        }
        (arg0->state) = 1;
        (arg0->flags) |= 0x800;
        far_base = (u8 *)&D_8007AFF0;
        if ((*(s16 *) (far_base + 0x25476)) == (arg0->map)) {
            PlaySoundEffect(0x1C);
        }
        break;
    case -1:
        result = 1;
        (arg0->timer) = 0x12C;
        (arg0->pool) = 0;
        (arg0->flags) &= ~0x800;
        var_v0 = Ovl11ChooseEligibleTaggedRecord(0);
        if (var_v0 < 0) {
            var_v0 = Ovl11ChooseEligibleTaggedRecord(1);
            (arg0->pool) = 1;
            if (var_v0 < 0) {
                ovl_11_func_800D3104((u16 *)arg0, 0x13);
                break;
            }
        }
        (arg0->index) = var_v0;
        ovl_11_func_800D3104((u16 *)arg0, 0x14);
        break;
    case 1:
    default:
        if (ovl_11_func_800D04D4((Ovl11D539CStepObj *) arg0, &D_80123C88, (s16 *) ((u8 *)arg0 + 0x28), -1, (u16 *)((u8 *)arg0 + 0x2A)) != 0) {
            (arg0->state) = -1;
        }
        break;
    }
    return result;
}
