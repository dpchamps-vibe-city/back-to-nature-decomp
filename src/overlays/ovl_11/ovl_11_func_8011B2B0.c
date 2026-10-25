#include "common.h"
#include "game_types.h"
#include "random.h"

void ovl_11_func_8011D400(s32 *arg0);
s16 ovl_11_func_8011D890(Ovl11D548Arg x, s32 mode);

s32 ovl_11_func_8011B2B0(void) {
    s32 flags;
    u8 *p;
    u8 *p2;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    flags = D_800719F8;
    p = gOvl11PrimarySelectionRecord;
    ovl_11_func_8011D400((s32 *) p);
    *(s8 *) (p + 0x5A) = 0xD;
    *(s8 *) (p + 0x5B) = 0xD;
    *(s16 *) (p + 0x5C) = 0;
    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051B76 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0x87;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) D_80051B76 - 0xAC + gTextBankOffsets[1]);
    *a++ = D_801282DC[Rand(3)];
    *c++ = 1;
    *b++ = 0x88;
    if (flags & 0x10000) {
        *(s8 *) (p + 0x5A) = 0x10;
        *(s8 *) (p + 0x5B) = 0x10;
        *q = (void *) ((s32) D_80051B76 - 0x76 + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x13F;
    *c++ = 1;
    *b++ = 0x89;
    if (flags & 0x8000) {
        *q = (void *) ((s32) D_80051AE6 + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x140;
    *c++ = 1;
    *b++ = 0x8A;
    if (flags & 0x20000) {
        *q = (void *) ((s32) D_80051B22 + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x141;
    *c++ = 1;
    *b++ = 0x8B;
    *q = (void *) ((s32) D_80051B38 + gTextBankOffsets[1]);
    *a = 0;
    *c = 1;
    *b = 0x8C;

    p2 = gOvl11SecondarySelectionRecord;
    ovl_11_func_8011D400((s32 *) p2);
    *(s8 *) (p2 + 0x5A) = 0;
    *(s8 *) (p2 + 0x5B) = 0;
    *(s16 *) (p2 + 0x5C) = 0;
    q = (void **) p2;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    if (flags & 0x40000) {
        *q = (void *) ((s32) D_80051B38 + 0x12 + gTextBankOffsets[1]);
        *(s8 *) ((u8 *) q + 0x5A) = 5;
        *(s8 *) ((u8 *) q + 0x5B) = 10;
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x1C;
    *c++ = 0;
    *b++ = 0xAA;
    if (flags & 0x80000) {
        r = gOvl11SecondarySelectionRecord;
        *(s8 *) (r + 0x5A) = 6;
        *(s8 *) (r + 0x5B) = 12;
        *q = (void *) ((s32) D_80051B56 + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x1A;
    *c++ = 0;
    *b++ = 0xAB;
    if (flags & 0x100000) {
        u8 *entry;

        *q = (void *) ((s32) D_80051B64 + gTextBankOffsets[1]);
        entry = gOvl11SecondarySelectionRecord;
        *(s8 *) (entry + 0x5A) = 8;
        *(s8 *) (entry + 0x5B) = 14;
    } else {
        *q = 0;
    }
    *a = 0x1B;
    *c = 0;
    *b = 0xAC;
    if ((flags & 0x1C0000) && !(flags & 0x40000)) {
        s16 selected;
        u8 *record;

        selected = ovl_11_func_8011D890(*(Ovl11D548Arg *) gOvl11SecondarySelectionRecord, 1);
        record = gOvl11SecondarySelectionRecord;
        *(s16 *) (record + 0x5C) = selected;
    }
    gOvl11InteractionDrawMode = 0;
    return 1;
}
