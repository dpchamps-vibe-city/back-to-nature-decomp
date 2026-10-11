#include "common.h"
#include "psyq/stddef.h"

s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_8011AC30(void) {
    s32 mask;
    u8 *p;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    D_8012D538.unk0 = 0;
    D_8012D538.unk4 = (void *) ((u8 *) &D_8005181A + D_80054BBC[1]);
    D_8012D538.unkA = ovl_11_func_800D6014(0x6CU);
    D_8012D538.unk8 = 1;
    mask = D_800719F8 & 8;
    p = D_8012D548;
    ovl_11_func_8011D400((s32 *) p);

    if (mask != 0) {
        *(s8 *) (p + 0x5A) = 0xD;
        *(s8 *) (p + 0x5B) = 0xD;
    } else {
        *(s8 *) (p + 0x5A) = 0xC;
        *(s8 *) (p + 0x5B) = 0xC;
    }
    r = D_8012D548;
    *(s16 *) (r + 0x5C) = 0;

    q = (void **) r;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_80051A90 + D_80054BBC[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x36;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xCF;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051A90 - 0x1C2 + D_80054BBC[1]);
    *a++ = 0x27;
    *c++ = 1;
    *b++ = 0xC7;
    *q++ = (void *) ((s32) &D_80051A90 - 0x1A8 + D_80054BBC[1]);
    *a++ = 0x28;
    *c++ = 1;
    *b++ = 0xC8;
    *q++ = (void *) ((s32) &D_80051A90 - 0x192 + D_80054BBC[1]);
    *a++ = 0x29;
    *c++ = 1;
    *b++ = 0xC9;
    if (mask != 0) {
        *q = (void *) ((s32) &D_80051A90 - 0x17A + D_80054BBC[1]);
    } else {
        *q = NULL;
    }
    *a = 0x2A;
    *c = 1;
    *b = 0xCA;
    D_8012D52C = 0;
    return 1;
}
