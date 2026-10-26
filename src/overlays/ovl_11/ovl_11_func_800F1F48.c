#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"

void *ovl_11_func_800F213C(u16 *arg0, s32 arg1);

void ovl_11_func_800F1F48(s32 arg0) {
    struct_80076220 *var_s0;
    u16 *source;
    u16 *stream;
    u16 *field2;
    u32 index;
    u32 j;
    u32 value;

    var_s0 = &gObjectEventSlotBase;
    for (index = 0; index < 0x25U; index++, var_s0++) {
        *(u16 *)var_s0->unk0 = index;
        var_s0->unk2C = 0x36;
        var_s0->unk2E = 0x36;
        *(s16 *)((u8 *)var_s0 + 8) = 0;
        var_s0->unkA = 0;
        var_s0->unkC = 0;
        var_s0->unkE = 0;
        *(s16 *)((u8 *)var_s0 + 0x10) = 0;
        *(s16 *)((u8 *)var_s0 + 0x14) = 0;
        *(s16 *)((u8 *)var_s0 + 0x16) = 0;
        *(s16 *)((u8 *)var_s0 + 0x18) = 0;
        *(s16 *)((u8 *)var_s0 + 0x1A) = 0;
        *(s16 *)((u8 *)var_s0 + 0x1C) = 0;
        var_s0->unk22 = 0;
        *(s32 *)var_s0->unk28 = 0;
        var_s0->unk1E = (var_s0->unk1E | 0x8000) & 0x9E48;
        *(u16 *)var_s0->unk20 &= 0xDFFF;
        Ovl11ResetTimedSpriteEffect((s16 *)&var_s0->unk30[0xB0]);
        field2 = (u16 *)&var_s0->unkE4[0].unk2;
        for (j = 0; j < 0x1EU; j++) {
            var_s0->unkE4[j].unk0 = 0;
            field2[j * 4] = 0;
            var_s0->unkE4[j].unk4 = 0x36;
            var_s0->unkE4[j].unk6 = 0x36;
        }
        stream = D_80126BF0[index];
        if (stream != NULL) {
            source = ovl_11_func_800F213C(stream, arg0);
            if (source == NULL) {
                *(s32 *)var_s0->unk28 = 0;
            } else {
                *(s32 *)var_s0->unk28 = (s32)source - arg0;
            }
            if (source != NULL) {
                for (j = 0; j < 0x1EU; j++, source += 4) {
                    var_s0->unkE4[j].unk0 = source[0];
                    field2[j * 4] = source[1];
                    var_s0->unkE4[j].unk4 = source[2];
                    var_s0->unkE4[j].unk6 = source[3];
                }
            }
            value = *(u16 *)&var_s0->unkE4[0].unk2 >> 8;
            var_s0->unk2C = var_s0->unkE4[0].unk4;
            var_s0->unk2E = var_s0->unkE4[0].unk6;
            *(s16 *)((u8 *)var_s0 + 0x14) = value;
            *(s16 *)((u8 *)var_s0 + 0x16) = value;
        }
    }
}
