#include "common.h"

INCLUDE_ASM("build/ovl_11/asm/nonmatchings/ovl_11_func_80110E98", ovl_11_func_80110E98);


/* PARKED by /auto_decompilation_loop on 2026-10-10T22:12:55.297Z.
 * Reason: escalation-exhausted.
 * Escalation reached: deepseek-v4-1-flash.
 * The best non-matching attempt is preserved verbatim below, disabled.
 * Findings and the decision needed: notes/human-needed-approvals/ovl_11_func_80110E98.md
 */

#if 0
/* Best non-matching attempt, preserved for the next session. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"

typedef struct {
    char pad_00[0x24];
    /* 0x24 */ s16 unk24;
    /* 0x26 */ s16 unk26;
    char pad_28[0x2C - 0x28];
    /* 0x2C */ s16 unk2C;
    char pad_2E[0x30 - 0x2E];
    /* 0x30 */ s16 unk30;
    char pad_32[0x34 - 0x32];
    /* 0x34 */ s32 unk34;
    char pad_38[0xAC - 0x38];
    /* 0xAC */ u16 unkAC;
    /* 0xAE */ s16 unkAE;
    /* 0xB0 */ s16 unkB0;
    /* 0xB2 */ s16 unkB2;
    char pad_B4[0xB8 - 0xB4];
    /* 0xB8 */ u16 unkB8;
} Ovl11698Obj;

struct struct_8006C838_80110E98 {
    char pad_000[0x44BA];
    /* 0x44BA */ s16 field_44BA;
    /* 0x44BC */ s16 field_44BC;
};

extern s16 D_80127F50[];

void ovl_11_func_80110E98(Ovl11698Obj *arg0, s32 *arg1, s32 *arg2) {
    s32 var_v0;
    s16 coord;
    s32 var_v0_2;

    coord = 0;
    if (arg0->unk30 == 0x28) {
        if (((struct struct_8006C838_80110E98 *) D_8006C838)->field_44BA == 3) {
            if (((struct struct_8006C838_80110E98 *) D_8006C838)->field_44BC >= 0x1C) {
                var_v0 = 2;
                goto store;
            }
        }
        if (((struct struct_8006C838_80110E98 *) D_8006C838)->field_44BA != 0) {
            *arg1 = 0;
            *arg2 = 0;
            goto tail;
        }
        var_v0 = 2;
    } else if (arg0->unkB8 & 0x40) {
        switch (arg0->unk26) {
        case 2:
            *arg1 = 0;
            *arg2 = 0xA;
            arg0->unk2C = 0x12C;
            arg0->unk24 = 0;
            arg0->unk34 &= ~0x800;
            goto tail;
        case 3:
            var_v0 = 0xC;
            break;
        case 4:
        case 7:
            var_v0 = 8;
            break;
        default:
            var_v0 = 8;
            break;
        }
    } else {
        if (arg0->unkB8 & 1) {
            var_v0_2 = arg0->unkAE < 0xC8;
        } else if (arg0->unkB8 & 2) {
            var_v0_2 = arg0->unkB0 < 0xC8;
        } else if (arg0->unkB8 & 4) {
            var_v0_2 = arg0->unkB2 < 0xC8;
        } else {
            var_v0_2 = coord < 0xC8;
        }
        if (var_v0_2 == 0) {
            var_v0 = 6;
        } else {
            var_v0 = 4;
        }
    }
store:
    *arg1 = 0;
    *arg2 = var_v0;
tail:
    *arg2 += D_80127F50[arg0->unkAC];
    if (arg0->unkB8 & 8) {
        *arg2 = *arg2 + 1;
    }
}
#endif
