#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"

s32 ovl_11_func_80112A84 (void);
s32 ovl_11_func_800FEB14 (void);
s32 ovl_11_func_800FEB68 (void);
s32 ovl_11_func_800FEBB0 (void);
s32 ovl_11_func_800FEAD4 (void);
s32 ovl_11_func_800F3C9C (u16 arg0);


void ovl_11_func_800FB908(void) {
    s32 var_s3;
    s32 var_s4;
    s32 var_s5;
    s32 var_s0;
    s32 var_s7;
    /* Template byte offset, then the name's byte address. */
    s32 text_position;
    s16 *counts;
    u16 temp_a0;
    u32 clamp_value;
    s32 var_a1;
    s32 var_a2;
    s32 var_a2_2;
    s32 var_t0;
    u8 *text;
    s32 var_a0_2;

    D_8012720C = 0;
    D_8012720E = 0;
    D_80127210 = 0;
    D_80127212 = 0;
    for (var_s0 = 0; var_s0 < 14; var_s0++) {
        text_position = var_s0 * 2;
        temp_a0 = *(u16 *) ((u8 *) &D_80070D18 + text_position);
        text = D_800535C6;
        if (temp_a0 != 0xFFFF) {
            D_8012CDC8[var_s0] = temp_a0;
        } else {
            D_8012CDC8[var_s0++] = 0xFFD;
            text_position = (s32) &text[*D_80054BBC];
            for (; var_s0 < 14; text_position += 2, var_s0++) {
                D_8012CDC8[var_s0] = *(u16 *) text_position;
                if (D_8012CDC8[var_s0] == 0xFFFF) {
                    goto complete;
                }
            }
        }
    }
complete:
    var_a1 = 2;
    D_8012CDC8[0xD] = 0xFFFF;
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44CE < 3U) {
        var_a1 = ((Ovl11FB908State *) D_8006C838)->field_44CE;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D0 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D0;
    } else {
        var_a1 = var_a1 + 1;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D2 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D2;
    } else {
        var_a1 = var_a1 + 1;
    }
    if ((u16) ((Ovl11FB908State *) D_8006C838)->field_44D4 < 2U) {
        var_a1 = var_a1 + ((Ovl11FB908State *) D_8006C838)->field_44D4;
    } else {
        var_a1 = var_a1 + 1;
    }

    var_a1 *= 20;
    D_8012CDE6 = var_a1;
    D_8012CDE8 = 0;
    for (var_s0 = 0; var_s0 < 80; var_s0++) {
        if (((Ovl11FB908State *) D_8006C838)->records[var_s0][0] != 0) {
            D_8012CDE8 = (u16) D_8012CDE8 + 1;
        }
    }
    if (((Ovl11FB908State *) D_8006C838)->field_E776 >= 0) {
        var_s7 = (((Ovl11EF0E8StatusView *) ((u8 *) D_8006C838 + (((Ovl11FB908State *) D_8006C838)->field_E778 * 0x1D4)))->field_99EC / 6000) & 0xFFFF;
        D_80127234->unk0 = var_s7;
        D_80127234->unk4 = ovl_11_func_80112A84();
        D_80127234->unk9 = (u8) ((Ovl11FB908State *) D_8006C838)->field_E776 + 1;
    } else {
        var_s7 = 0;
        D_80127234->unk4 = 0;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_E780 == 1) {
        D_80127234[1].unk4 = (s32) &D_8007AFBC;
        var_s5 = (s16) ((Ovl11FB908State *) D_8006C838)->field_DBBA / 25;
        D_80127234[1].unk0 = (s32) var_s5;
    } else {
        var_s5 = 0;
        D_80127234[1].unk4 = NULL;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_91AC != 0) {
        D_80127234[2].unk4 = (s32) &D_800759E8;
        var_s4 = (s16) ((Ovl11FB908State *) D_8006C838)->field_91C2 / 25;
        D_80127234[2].unk0 = (s32) var_s4;
    } else {
        var_s4 = 0;
        D_80127234[2].unk4 = NULL;
    }
    if (((Ovl11FB908State *) D_8006C838)->field_929C != 0) {
        D_80127234[3].unk4 = (s32) &D_80075AD8;
        var_s3 = (s16) ((Ovl11FB908State *) D_8006C838)->field_92B2 / 25;
        D_80127234[3].unk0 = (s32) var_s3;
    } else {
        var_s3 = 0;
        D_80127234[3].unk4 = NULL;
    }
    D_80127220->unk2 = ovl_11_func_800FEB14();
    (*(u16 *) ((u8 *) D_80127220 + 6)) = ovl_11_func_800FEB68();
    (*(u16 *) ((u8 *) D_80127220 + 0xA)) = ovl_11_func_800FEBB0();
    (*(s16 *) ((u8 *) D_80127220 + 0xE)) = ovl_11_func_800FEAD4();
    var_a0_2 = 0x63;
    if ((u8) ((Ovl11FB908State *) D_8006C838)->field_44CD < 0x64U) {
        var_a0_2 = ((Ovl11FB908State *) D_8006C838)->field_44CD;
    }
    var_a2 = 0x3E7;
    (*(s16 *) ((u8 *) D_80127220 + 0x12)) = (s16) var_a0_2;
    counts = D_80127264;
    var_a0_2 = ((Ovl11FB908State *) D_8006C838)->field_4508;
    if (var_a0_2 < 0x3E8U) {
        var_a2 = var_a0_2;
    }
    var_a0_2 = 0x3E7;
    counts[1] = var_a2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->field_4502;
    if (clamp_value < 0x3E8U) {
        var_a0_2 = clamp_value;
    }
    var_a2_2 = 0x3E7;
    counts[3] = var_a0_2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->field_4506;
    if (clamp_value < 0x3E8U) {
        var_a2_2 = clamp_value;
    }
    var_t0 = 0x3E7;
    counts[5] = var_a2_2;
    clamp_value = ((Ovl11FB908State *) D_8006C838)->field_4500;
    if (clamp_value < 0x3E8U) {
        var_t0 = clamp_value;
    }
    counts[7] = var_t0;
    D_8012CDE4 = (*(u16 *) ((u8 *) D_80127220 + 0xA)) + ((*(u16 *) ((u8 *) D_80127220 + 6)) + ((u16) D_80127220->unk2 + ((D_8012CDE6 / 10) + (D_8012CDE8 / 8) + var_s7 + var_s5 + var_s4 + var_s3)));
    for (var_s0 = 0; (u32) var_s0 < 21; var_s0++) {
        D_80127274[var_s0].unk2 = ((Ovl11FB908State *) D_8006C838)->values[ovl_11_func_800F3C9C(*(u16 *) &D_80127274[var_s0])];
    }
}
