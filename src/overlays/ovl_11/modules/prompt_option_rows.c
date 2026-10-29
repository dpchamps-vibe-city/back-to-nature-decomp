/*
 * prompt_option_rows — reconstructed translation unit.
 *
 * Complete handler run building quantity-prompt and primary/secondary option
 * records, with gated text rows and draw-mode completion. Callback variants
 * select five/eight/single rows; Bit2/3/4/23 name the demonstrated progress
 * mask bits, not presumed gameplay categories.
 *
 * The unrolled writers walk parallel arrays: q=text pointers (+0), a=row
 * values (+28), c=row types (+3C), b=auxiliary halfwords (+46). A gated text
 * store advances q after its branch, then the other stores advance their
 * cursors. Keep the complete final-row append and that order: truncating the
 * cursor lifecycle lets this compiler fold the walks into base+offset stores.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_prompt_option_rows.h"
#include "ovl11_cell_grid_lookup.h"
#include "random.h"
#include "ovl11_selection_record_access.h"
#include "ovl11_selection_record_reset.h"

s32 Ovl11ResetOptionDrawMode(void) {
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11InitQuantityPrompt6C(void) {
    Ovl11QuantityPromptView *prompt;

    prompt = (Ovl11QuantityPromptView *)&gOvl11QuantityPrompt;
    prompt->primaryTextAddress = gTextBankOffsets[1] + (s32)&gQuantityPromptDefaultTextAnchor + 0x10;
    prompt->secondaryTextAddress = gTextBankOffsets[1] + (s32)&gQuantityPromptDefaultTextAnchor;
    prompt->unitValue = Ovl11GetItemPrice(0x6C);
    prompt->quantity = 1;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11ResetOptionDrawModeA(void) {
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11ResetOptionDrawModeB(void) {
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11FillFivePromptRowsBit2(void) {
    s32 temp_s4;
    s8 var_v0;
    u8 *p;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    temp_s4 = gProgressDerivedFlags & 4;
    gOvl11QuantityPrompt.unk0 = 0;
    gOvl11QuantityPrompt.unk4 = (void *) ((u8 *) &gQuantityPromptDefaultTextAnchor + gTextBankOffsets[1]);
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x6CU);
    gOvl11QuantityPrompt.unk8 = 1;
    p = gOvl11PrimarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p);
    if (temp_s4 != 0) {
        *(s8 *) (p + 0x5A) = 0x10;
        *(s8 *) (p + 0x5B) = 0x10;
    } else {
        *(s8 *) (p + 0x5A) = 0xE;
        *(s8 *) (p + 0x5B) = 0xE;
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
    *q++ = (void *) ((s32) &D_80051A90 - 0x252 + gTextBankOffsets[1]);
    *a++ = 0x22;
    *c++ = 1;
    *b++ = 0xC3;
    *q++ = (void *) ((s32) &D_80051A90 - 0x238 + gTextBankOffsets[1]);
    *a++ = 0x23;
    *c++ = 1;
    *b++ = 0xC4;
    *q++ = (void *) ((s32) &D_80051A90 - 0x21E + gTextBankOffsets[1]);
    *a++ = 0x24;
    *c++ = 1;
    *b++ = 0xC5;
    if (temp_s4 != 0) {
        *q = (void *) ((s32) &D_80051A90 - 0x200 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x25;
    *c++ = 1;
    *b++ = 0xC6;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11FillFivePromptRowsBit3(void) {
    s32 mask;
    u8 *p;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    gOvl11QuantityPrompt.unk0 = 0;
    gOvl11QuantityPrompt.unk4 = (void *) ((u8 *) &gQuantityPromptDefaultTextAnchor + gTextBankOffsets[1]);
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x6CU);
    gOvl11QuantityPrompt.unk8 = 1;
    mask = gProgressDerivedFlags & 8;
    p = gOvl11PrimarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p);

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
    q++;
    *a++ = 0x2A;
    *c++ = 1;
    *b++ = 0xCA;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11FillFivePromptRowsBit4(void) {
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    gOvl11QuantityPrompt.unk0 = 0;
    gOvl11QuantityPrompt.unk4 = (void *) ((u8 *) &gQuantityPromptDefaultTextAnchor + gTextBankOffsets[1]);
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

    *q = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x276 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x36;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xCF;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x138 + gTextBankOffsets[1]);
    *a++ = 0x2C;
    *c++ = 1;
    *b++ = 0xCB;
    *q++ = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x156 + gTextBankOffsets[1]);
    *a++ = 0x2D;
    *c++ = 1;
    *b++ = 0xCC;
    *q++ = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x170 + gTextBankOffsets[1]);
    *a++ = 0x2E;
    *c++ = 1;
    *b++ = 0xCD;
    if (gProgressDerivedFlags & 0x10) {
        *q = (void *) ((s32) &gQuantityPromptDefaultTextAnchor + 0x196 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x2F;
    *c++ = 1;
    *b++ = 0xCE;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11FillEightPromptRowsBit23(void) {
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

s32 Ovl11FillSinglePromptRow(void) {
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5A)) = 0xB;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5B)) = 0x10;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x28)) = 0x9F;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5C)) = 0;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3C)) = 0;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3D)) = 0;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x46)) = 0xB2;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x2A)) = 0xA0;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x48)) = 0xB3;
    gOvl11InteractionDrawMode = 0;
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 0)) = (void *) ((u8 *) D_80051AA8 + *D_80054BC0);
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 4)) = (void *) ((u8 *) (D_80051AA8 + 0xA) + *D_80054BC0);
    return 1;
}

s32 Ovl11FillPrimaryAndSecondaryPromptRows(void) {
    s32 flags;
    u8 *p;
    u8 *p2;
    u8 *r;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    flags = gProgressDerivedFlags;
    p = gOvl11PrimarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p);
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
    Ovl11ClearSelectionRows((s32 *) p2);
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

        selected = Ovl11FindOccupiedSelectionIndex(*(Ovl11D548Arg *) gOvl11SecondarySelectionRecord, 1);
        record = gOvl11SecondarySelectionRecord;
        *(s16 *) (record + 0x5C) = selected;
    }
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11ResetOptionDrawModeTail(void) {
    gOvl11InteractionDrawMode = 0;
    return 1;
}
