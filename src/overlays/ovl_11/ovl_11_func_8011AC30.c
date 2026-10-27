#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"

void ovl_11_func_8011D400(s32 *arg0);

s32 ovl_11_func_8011AC30(void) {
    s32 mask;
    u8 *p;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    gOvl11QuantityPrompt.unk0 = 0;
    gOvl11QuantityPrompt.unk4 = (void *) ((u8 *) &D_8005181A + gTextBankOffsets[1]);
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x6CU);
    gOvl11QuantityPrompt.unk8 = 1;
    mask = gProgressDerivedFlags & 8;
    p = gOvl11PrimarySelectionRecord;
    ovl_11_func_8011D400((s32 *) p);

    if (mask != 0) {
        *(s8 *) (p + 0x5A) = 0xD;
        *(s8 *) (p + 0x5B) = 0xD;
    } else {
        *(s8 *) (p + 0x5A) = 0xC;
        *(s8 *) (p + 0x5B) = 0xC;
    }
    r = gOvl11PrimarySelectionRecord;
    *(s16 *) (r + 0x5C) = 0;

    q = (void **) r;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_80051A90 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x36;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xCF;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051A90 - 0x1C2 + gTextBankOffsets[1]);
    *a++ = 0x27;
    *c++ = 1;
    *b++ = 0xC7;
    *q++ = (void *) ((s32) &D_80051A90 - 0x1A8 + gTextBankOffsets[1]);
    *a++ = 0x28;
    *c++ = 1;
    *b++ = 0xC8;
    *q++ = (void *) ((s32) &D_80051A90 - 0x192 + gTextBankOffsets[1]);
    *a++ = 0x29;
    *c++ = 1;
    *b++ = 0xC9;
    if (mask != 0) {
        *q = (void *) ((s32) &D_80051A90 - 0x17A + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    *a = 0x2A;
    *c = 1;
    *b = 0xCA;
    gOvl11InteractionDrawMode = 0;
    return 1;
}
