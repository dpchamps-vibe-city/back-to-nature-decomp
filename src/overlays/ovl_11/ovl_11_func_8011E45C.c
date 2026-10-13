#include "common.h"

s32 ovl_11_func_8011E1E8(s16 arg0, s16 arg1, s16 arg2);

void ovl_11_func_8011E45C(void) {
    Ovl11D124Entry *entry;
    s16 textureIndex;
    u32 row;
    u16 textureCode;
    u32 column;
    u8 variant;

    entry = &D_80074124[0][0];
    row = 0;
    do {
        column = 0;
        do {
            textureCode = (u16) entry->unk2;
            textureIndex = 0;
            switch (textureCode) {
            case 0x167:
                textureIndex = 0;
                break;
            case 0x168:
                textureIndex = 1;
                break;
            case 0x169:
                textureIndex = 2;
                break;
            case 0x16A:
                textureIndex = 3;
                break;
            case 0x16B:
                textureIndex = 4;
                break;
            }
            if ((u16) entry->unk0 == 0x36) {
                if (entry->unk4 != 0) {
                    variant = entry->unk4;
                    textureIndex = ((variant - 1) * 8) | 5;
                    textureIndex += (u16) entry->unk6 & 0xF;
                }
            }
            ovl_11_func_8011E1E8((s16) column, (s16) row, textureIndex);
            column++;
            entry++;
        } while (column < 7U);
        row++;
    } while (row < 7U);
}
