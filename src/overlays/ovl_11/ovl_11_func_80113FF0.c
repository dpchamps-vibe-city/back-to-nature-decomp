#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void ovl_11_func_80111F10(void);
s32 ovl_11_func_800D6014(u16 arg0);
void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_80113FF0(void) {
    s32 temp_s3;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    ovl_11_func_80111F10();
    D_8012D538.unk0 = (s32) ((void *) ((u8 *) &D_80051E26 + D_80054BBC[1]));
    *(s32 *) ((u8 *) &D_8012D538 + 4) = (s32) D_80051E26 - 0x60C + D_80054BBC[1];
    temp_s3 = D_800719F8;
    D_8012D538.unkA = ovl_11_func_800D6014(0x69U);
    D_8012D538.unk8 = 1;
    ovl_11_func_8011D400((s32 *) D_8012D548);

    p = D_8012D548;
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;

    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_80051E26 + D_80054BBC[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x69;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0x47;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051E26 + 0x1A + D_80054BBC[1]);
    *a++ = 0x109;
    *c++ = 0;
    *b++ = 0x4A;
    *q++ = (void *) ((s32) &D_80051E26 + 0x32 + D_80054BBC[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x49;
    if (temp_s3 & 0x4000) {
        *q = (void *) ((s32) &D_80051E26 + 0x4C + D_80054BBC[1]);
    } else {
        *q = NULL;
    }
    *a = 0x1E;
    *c = 0;
    *b = 0x48;
    D_8012D52C = 0;
    return 1;
}
