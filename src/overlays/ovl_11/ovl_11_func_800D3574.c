#include "common.h"
#include "game_types.h"
#include "psyq/libapi.h"

s32 ovl_11_func_800C28CC(s16 arg0, SpriteSourceData *arg1);
/* The matched initializer does not read its trailing argument slots. */
void func_80015704();

void ovl_11_func_800D3574(u16 *arg0, s32 arg1) {
    SpriteSourceData sp10;
    s32 *var_s1;
    s32 var_s0;
    u16 temp_v1;

    var_s1 = NULL;
    temp_v1 = *arg0;
    var_s0 = 0;
    switch (temp_v1) {
    case 0x154:
        var_s1 = &D_8009A3F8;
        var_s0 = D_80124FE4[5];
        break;
    case 0x156:
        var_s1 = &D_8009A3F8;
        var_s0 = D_80124FE4[4];
        break;
    case 0x157:
        var_s1 = &D_8009A3F8;
        var_s0 = D_80124FE4[3];
        break;
    case 0x15B:
        var_s1 = &D_8009CBF8;
        var_s0 = 0;
        break;
    case 0x10B:
    case 0x10C:
        var_s1 = &D_80098BF8;
        switch (D_80070CF2) {
        case 0:
            var_s0 = D_80124FD4[3];
            break;
        case 1:
            var_s0 = D_80124FD8[2];
            break;
        case 2:
            var_s0 = D_80124FDC[3];
            break;
        case 3:
            var_s0 = D_80124FE0[3];
            break;
        }
        break;
    case 0x10D:
        var_s1 = &D_80098BF8;
        switch (D_80070CF2) {
        case 0:
            var_s0 = D_80124FD4[1];
            break;
        case 1:
            var_s0 = D_80124FD8[1];
            break;
        case 2:
            var_s0 = D_80124FDC[2];
            break;
        case 3:
            SystemError(0x45, 1);
            break;
        }
        break;
    case 0x10E:
        var_s1 = &D_80098BF8;
        switch (D_80070CF2) {
        case 0:
            SystemError(0x45, 2);
            break;
        case 1:
            SystemError(0x45, 3);
            break;
        case 2:
            var_s0 = D_80124FDC[1];
            break;
        case 3:
            var_s0 = D_80124FE0[1];
            break;
        }
        break;
    case 0x10F:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 3) {
            var_s0 = D_80124FE0[2];
        } else {
            SystemError(0x45, 4);
        }
        break;
    case 0x110:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 0) {
            var_s0 = D_80124FD4[2];
        } else {
            SystemError(0x45, 5);
        }
        break;
    case 0x114:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 1) {
            var_s0 = D_80124FD8[3];
        } else {
            SystemError(0x45, 6);
        }
        break;
    case 0x115:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 1) {
            var_s0 = D_80124FD8[4];
        } else {
            SystemError(0x45, 7);
        }
        break;
    case 0x116:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 1) {
            var_s0 = D_80124FD8[5];
        } else {
            SystemError(0x45, 8);
        }
        break;
    case 0x117:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 2) {
            var_s0 = D_80124FDC[5];
        } else {
            SystemError(0x45, 9);
        }
        break;
    case 0x118:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 2) {
            var_s0 = D_80124FDC[4];
        } else {
            SystemError(0x45, 8);
        }
        break;
    case 0x11A:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 0) {
            var_s0 = D_80124FD4[5];
        } else {
            SystemError(0x45, 0xB);
        }
        break;
    case 0x119:
        var_s1 = &D_80098BF8;
        if (D_80070CF2 == 0) {
            var_s0 = D_80124FD4[4];
        } else {
            SystemError(0x45, 0xC);
        }
        break;
    case 0x108:
    case 0x112:
    case 0x113:
    case 0x153:
    case 0x155:
    case 0x159:
    case 0x162:
    case 0x165:
    case 0x17A:
    case 0x17B:
        break;
    default:
        SystemError(0x45, 0);
        break;
    }
    switch (*arg0) {
    case 0x162:
        if (D_80070CF8_word == 6 || ovl_11_func_800C28CC(0x28, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x165:
        if (D_80070CF8_word == 6 || ovl_11_func_800C28CC(0x29, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x108:
        if (D_80070CF8_word == 6 || ovl_11_func_800C28CC(0x2B, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x17A:
        if (D_80070CF8_word == 6 || ovl_11_func_800C28CC(0x2A, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    case 0x17B:
        if (D_80070CF8_word == 6 || ovl_11_func_800C28CC(0x2C, &sp10) == 0) {
            *arg0 = 0;
        } else if (sp10.field_14 != *(s32 *) ((u8 *) arg0 + 0x8C)) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) sp10.field_14);
        }
        break;
    default:
        if (*(s32 *) ((u8 *) arg0 + 0x8C) == 0) {
            func_80015704((SpriteSourceData *) ((u8 *) arg0 + 0x78), (SpriteDataHeader *) ((u8 *) var_s1 + var_s0));
        }
        break;
    }
}
