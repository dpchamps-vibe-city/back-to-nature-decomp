#include "common.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_8011ADF4(void) {
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    D_8012D538.unk0 = 0;
    D_8012D538.unk4 = (void *) ((u8 *) &D_8005181A + gTextBankOffsets[1]);
    D_8012D538.unkA = ovl_11_func_800D6014(0x6CU);
    D_8012D538.unk8 = 1;
    ovl_11_func_8011D400((s32 *) D_8012D548);

    p = D_8012D548;

    *(s8 *) (p + 0x5A) = 0x12;
    *(s8 *) (p + 0x5B) = 0x12;
    *(s16 *) (p + 0x5C) = 0;

    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_8005181A + 0x276 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x36;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xCF;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_8005181A + 0x138 + gTextBankOffsets[1]);
    *a++ = 0x2C;
    *c++ = 1;
    *b++ = 0xCB;
    *q++ = (void *) ((s32) &D_8005181A + 0x156 + gTextBankOffsets[1]);
    *a++ = 0x2D;
    *c++ = 1;
    *b++ = 0xCC;
    *q++ = (void *) ((s32) &D_8005181A + 0x170 + gTextBankOffsets[1]);
    *a++ = 0x2E;
    *c++ = 1;
    *b++ = 0xCD;
    if (D_800719F8 & 0x10) {
        *q = (void *) ((s32) &D_8005181A + 0x196 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    *a = 0x2F;
    *c = 1;
    *b = 0xCE;
    D_8012D52C = 0;
    return 1;
}
