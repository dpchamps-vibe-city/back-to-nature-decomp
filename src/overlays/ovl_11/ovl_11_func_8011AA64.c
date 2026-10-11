#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


s32 ovl_11_func_800D6014 (u16 arg0);
void ovl_11_func_8011D400 (s32 *arg0);

s32 ovl_11_func_8011AA64(void) {
    s32 temp_s4;
    s8 var_v0;
    u8 *p;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    temp_s4 = D_800719F8 & 4;
    D_8012D538.unk0 = 0;
    D_8012D538.unk4 = (void *) ((u8 *) &D_8005181A + D_80054BBC[1]);
    D_8012D538.unkA = ovl_11_func_800D6014(0x6CU);
    D_8012D538.unk8 = 1;
    p = D_8012D548;
    ovl_11_func_8011D400((s32 *) p);
    if (temp_s4 != 0) {
        *(s8 *) (p + 0x5A) = 0x10;
        *(s8 *) (p + 0x5B) = 0x10;
    } else {
        *(s8 *) (p + 0x5A) = 0xE;
        *(s8 *) (p + 0x5B) = 0xE;
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
    *q++ = (void *) ((s32) &D_80051A90 - 0x252 + D_80054BBC[1]);
    *a++ = 0x22;
    *c++ = 1;
    *b++ = 0xC3;
    *q++ = (void *) ((s32) &D_80051A90 - 0x238 + D_80054BBC[1]);
    *a++ = 0x23;
    *c++ = 1;
    *b++ = 0xC4;
    *q++ = (void *) ((s32) &D_80051A90 - 0x21E + D_80054BBC[1]);
    *a++ = 0x24;
    *c++ = 1;
    *b++ = 0xC5;
    if (temp_s4 != 0) {
        *q = (void *) ((s32) &D_80051A90 - 0x200 + D_80054BBC[1]);
    } else {
        *q = NULL;
    }
    *a = 0x25;
    *c = 1;
    *b = 0xC6;
    D_8012D52C = 0;
    return 1;
}
