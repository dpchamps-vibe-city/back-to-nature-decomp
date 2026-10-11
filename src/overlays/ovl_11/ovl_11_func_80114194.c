#include "common.h"

void ovl_11_func_80111F10(void);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_80114194(void) {
    s32 flags;
    u8 *p;
    u8 *p2;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    ovl_11_func_80111F10();
    flags = D_800719F8;
    p = D_8012D548;
    ovl_11_func_8011D400((s32 *) p);
    *(s8 *) (p + 0x5A) = 0xD;
    *(s8 *) (p + 0x5B) = 0xD;
    *(s16 *) (p + 0x5C) = 0;
    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051DD6 + D_80054BBC[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xDF;
    q++;
    a++;
    c++;
    b++;
    *q = (void *) ((s32) D_80051DD6 + 0xE + D_80054BBC[1]);
    *a = 0;
    *c = 1;
    *b = 0xE0;

    p2 = D_8012D5A8;
    ovl_11_func_8011D400((s32 *) p2);
    *(s8 *) (p2 + 0x5A) = 0xD;
    *(s8 *) (p2 + 0x5B) = 0x14;
    *(s16 *) (p2 + 0x5C) = 0;
    q = (void **) p2;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051DD6 + 0x28 + D_80054BBC[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x9B;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0xE1;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) D_80051DD6 + 0x3C + D_80054BBC[1]);
    *a++ = 0x9D;
    *c++ = 0;
    *b++ = 0xE3;
    if (flags & 1) {
        *q = (void *) ((s32) D_80051DD6 - 0x10C + D_80054BBC[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x9C;
    *c++ = 0;
    *b++ = 0xE2;
    if (flags & 2) {
        *q = (void *) ((s32) D_80051CB0 + D_80054BBC[1]);
    } else {
        *q = 0;
    }
    *a = 0x9E;
    *c = 0;
    *b = 0xE4;
    D_8012D52C = 0;
    return 1;
}
