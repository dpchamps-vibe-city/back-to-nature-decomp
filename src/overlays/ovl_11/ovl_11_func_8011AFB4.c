#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_selection_record_reset.h"

s32 ovl_11_func_8011AFB4(void) {
    s32 flags;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    gOvl11QuantityPrompt.unk0 = 0;
    *(s32 *) ((u8 *) &gOvl11QuantityPrompt + 4) = (s32) &gQuantityPromptDefaultTextAnchor + gTextBankOffsets[1];
    flags = gProgressDerivedFlags;
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x6CU);
    gOvl11QuantityPrompt.unk8 = 1;
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);

    p = gOvl11PrimarySelectionRecord;
    *(s8 *) (p + 0x5A) = 0x12;
    *(s8 *) (p + 0x5B) = 0x12;
    *(s16 *) (p + 0x5C) = 0;

    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x98 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x26;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0x165;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x118 + gTextBankOffsets[1]);
    *a++ = 0x2B;
    *c++ = 1;
    *b++ = 0x166;
    *q++ = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x1B2 + gTextBankOffsets[1]);
    *a++ = 0x30;
    *c++ = 1;
    *b++ = 0x167;
    if (flags & 0x800000) {
        *q = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x1D8 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x31;
    *c++ = 1;
    *b++ = 0x168;
    gOvl11InteractionDrawMode = 0;
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
