#include "common.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_8011AFB4(void) {
    s32 flags;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    D_8012D538.unk0 = 0;
    *(s32 *) ((u8 *) &D_8012D538 + 4) = (s32) &D_8005181A + gTextBankOffsets[1];
    flags = D_800719F8;
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
    *q = (void *) ((s32) &D_8005181A + 0x98 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x26;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0x165;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_8005181A + 0x118 + gTextBankOffsets[1]);
    *a++ = 0x2B;
    *c++ = 1;
    *b++ = 0x166;
    *q++ = (void *) ((s32) &D_8005181A + 0x1B2 + gTextBankOffsets[1]);
    *a++ = 0x30;
    *c++ = 1;
    *b++ = 0x167;
    if (flags & 0x800000) {
        *q = (void *) ((s32) &D_8005181A + 0x1D8 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x31;
    *c++ = 1;
    *b++ = 0x168;
    D_8012D52C = 0;
    *q++ = (void *) ((s32) D_80051A10 + gTextBankOffsets[1]);
    *a++ = 0x32;
    *c++ = 1;
    *b++ = 0x169;
    *q++ = (void *) ((s32) D_80051A10 + 0x1E + gTextBankOffsets[1]);
    *a++ = 0x33;
    *c++ = 1;
    *b++ = 0x16A;
    *q++ = (void *) ((s32) D_80051A10 + 0x3E + gTextBankOffsets[1]);
    *a++ = 0x34;
    *c++ = 1;
    *b++ = 0x16B;
    *q = (void *) ((s32) D_80051A10 + 0x60 + gTextBankOffsets[1]);
    *a = 0x35;
    *c = 1;
    *b = 0x16C;
    return 1;
}
