#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

s32 ovl_11_func_80120CC4(Ovl11TextureCoordinates *arg0, s32 arg1) {
    s32 var_a0;
    s32 row;
    u16 temp_a1;
    s32 temp_a0;
    u16 temp_v1;
    s32 temp_a0_2;
    s32 temp_a2;
    s32 temp_a3 = 0;
    s32 temp_v1_2;
    u32 u_left;
    s32 v_top;
    s32 v_top_coord;
    u32 clut_shift;
    s32 season = D_80070CF2;

    switch (arg1) {
    case 0x26:
        var_a0 = 0;
        break;
    case 0x25:
        var_a0 = 1;
        break;
    case 0x24:
        var_a0 = 2;
        break;
    case 0x31:
        var_a0 = 3;
        break;
    case 0x34:
        var_a0 = 4;
        break;
    case 0x23:
        var_a0 = 5;
        break;
    case 0x22:
        var_a0 = 6;
        break;
    case 0x35:
        var_a0 = 7;
        break;
    case 0x2A:
        var_a0 = 8;
        break;
    case 0x2B:
        var_a0 = 9;
        break;
    case 0x32:
        var_a0 = 0xA;
        break;
    case 0x29:
        var_a0 = 0xB;
        break;
    case 0x27:
        var_a0 = 0xC;
        break;
    case 0x28:
        var_a0 = 0xD;
        break;
    case 0x33:
    case 0x53:
    case 0x178:
        var_a0 = 0xE;
        break;
    case 0x54:
    case 0x179:
        var_a0 = 0xF;
        break;
    case 0x2F:
        var_a0 = 0x10;
        break;
    case 0x2C:
        var_a0 = 0x11;
        break;
    case 0x2D:
        var_a0 = 0x12;
        break;
    case 0x30:
        var_a0 = 0x13;
        break;
    case 0x2E:
        var_a0 = 0x14;
        break;
    case 0x3A:
        var_a0 = 0x15;
        if (season == 3) {
            var_a0 = 0x25;
        }
        break;
    case 0x40:
        var_a0 = 0x16;
        if (season == 2) {
            var_a0 = 0x17;
        }
        break;
    case 0x3E:
        var_a0 = 0x18;
        if (season == 3) {
            var_a0 = 0x26;
        }
        break;
    case 0x3F:
        var_a0 = 0x19;
        if (season == 3) {
            var_a0 = 0x24;
        }
        break;
    case 0x175:
        var_a0 = 0x1A;
        if (season == 3) {
            var_a0 = 0x23;
        }
        break;
    case 0x3B:
    case 0x3C:
        var_a0 = 0x1B;
        if (season == 3) {
            var_a0 = 0x27;
        }
        break;
    case 0x16C:
    case 0x16F:
        var_a0 = 0x1C;
        if (season == 3) {
            var_a0 = 0x28;
        }
        break;
    case 0x16D:
    case 0x170:
        var_a0 = 0x1E;
        if (season == 3) {
            var_a0 = 0x2A;
        }
        break;
    case 0x16E:
    case 0x171:
        var_a0 = 0x1D;
        if (season == 3) {
            var_a0 = 0x29;
        }
        break;
    case 0x3D:
        var_a0 = 0x1F;
        if (season == 3) {
            var_a0 = 0x2B;
        }
        break;
    case 0x172:
        var_a0 = 0x20;
        if (season == 3) {
            var_a0 = 0x2C;
        }
        break;
    case 0x173:
        var_a0 = 0x22;
        if (season == 3) {
            var_a0 = 0x2E;
        }
        break;
    case 0x174:
        var_a0 = 0x21;
        if (season == 3) {
            var_a0 = 0x2D;
        }
        break;
    case 0:
    default:
        return 0;
    }
    temp_a1 = D_80128764[var_a0];
    temp_a0 = 1;
    temp_v1 = (u32) (temp_a1 & 0xE00) >> 9;
    temp_a2 = temp_v1;
    temp_a2 <<= 5;
    temp_a2 |= 0x1F;
    row = (temp_a1 & 0x1C0) / 2;
    temp_a3 = row + 0x11F;
    arg0->u0 = temp_a2;
    arg0->v0 = temp_a3;
    u_left = temp_v1 << 5;
    arg0->u1 = u_left;
    arg0->u3 = u_left;
    v_top_coord = row + 0x100;
    arg0->v1 = temp_a3;
    arg0->u2 = temp_a2;
    arg0->v2 = v_top_coord;
    arg0->v3 = v_top_coord;
    clut_shift = (u32) temp_a1 >> 1;
    v_top = temp_a1 & 0x1F;
    temp_v1_2 = (clut_shift & 0x10) + 0x3C0;
    temp_a0_2 = v_top + 0x1E0;
    arg0->clut = (temp_v1_2 >> 4) | (temp_a0_2 << 6);
    return temp_a0;
}
