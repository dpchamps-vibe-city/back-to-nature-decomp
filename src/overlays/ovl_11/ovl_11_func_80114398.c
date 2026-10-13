#include "common.h"
#include "psyq/stddef.h"

void ovl_11_func_80111F10(void);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_80114398(void) {
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    ovl_11_func_80111F10();
    p = D_8012D548;
    ovl_11_func_8011D400((s32 *) p);
    *(s8 *) (p + 0x5A) = 0xC;
    *(s8 *) (p + 0x5B) = 0x17;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *(s16 *) (p + 0x5C) = 1;
    } else {
        *(s16 *) (p + 0x5C) = 0;
    }

    q = (void **) D_8012D548;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = NULL;
    } else {
        *q = (void *) ((s32) D_80051CF4 + D_80054BBC[1]);
    }
    q++;
    *a++ = 0x67;
    *c++ = 2;
    *b++ = -1;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = (void *) ((s32) D_80051D0C + D_80054BBC[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x185;
    *c++ = 2;
    *b++ = -1;
    D_8012D52C = 0;
    *q++ = (void *) ((s32) D_80051C7A + D_80054BBC[1]);
    *a++ = 0xF7;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x12 + D_80054BBC[1]);
    *a++ = 0xF8;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x24 + D_80054BBC[1]);
    *a++ = 0xF9;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x36 + D_80054BBC[1]);
    *a++ = 0x9E;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x50 + D_80054BBC[1]);
    *a++ = 0x9C;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x6A + D_80054BBC[1]);
    *a++ = 0x5C;
    *c++ = 2;
    *b++ = -1;
    *q = (void *) ((s32) D_80051C7A + 0xA8 + D_80054BBC[1]);
    *a = 0x60;
    *c = 2;
    *b = -1;
    return 1;
}
