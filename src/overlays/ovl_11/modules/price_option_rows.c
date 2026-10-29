/*
 * price_option_rows — reconstructed translation unit.
 *
 * Complete price/quantity and paired-selection row-initializer run; each
 * callback clears interaction draw mode and returns handled. Trade/quantity
 * builders expose a C interface; the reset and paired/flag variants are
 * callbacks. All overwrite shared selection state, not a caller-owned buffer.
 *
 * q/a/c/b walk text pointers, values, row types and auxiliary halfwords at
 * +0/+28/+3C/+46. In the gated final quantity row, advance q at the branch
 * join and the other cursors on their stores, completing the same append as
 * earlier rows. Its order preserves the compiler's pointer-walk form.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_price_option_rows.h"
#include "ovl11_progress_status.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_selection_record_reset.h"
#include "ovl11_item_slot_commands.h"

s32 Ovl11BuildTradePriceRows(void) {
    s32 flags;
    Ovl11D8012D538 *header;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    header = &gOvl11QuantityPrompt;
    header->unk0 = (s32) D_80051E92 + gTextBankOffsets[1];
    *(s32 *) ((u8 *) header + 4) = (s32) D_80051E92 - 0x678 + gTextBankOffsets[1];
    flags = gProgressDerivedFlags;
    header->unkA = Ovl11GetItemPrice(0x68U);
    header->unk8 = 1;
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);

    p = gOvl11PrimarySelectionRecord;
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;
    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051E92 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x68;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0x63;
    q++;
    a++;
    c++;
    b++;
    if (flags & 0x200000) {
        *q = (void *) ((s32) D_80051E92 + 0xE + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x1F;
    *c++ = 0;
    *b++ = 0x65;
    if (flags & 0x400000) {
        *q = (void *) ((s32) D_80051EB8 + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x20;
    *c++ = 0;
    *b++ = 0x66;
    *q++ = (void *) ((s32) D_80051ED0 + gTextBankOffsets[1]);
    *a++ = 0x161;
    *c++ = 0;
    *b++ = 0x67;
    *q++ = (void *) ((s32) D_80051ED0 + 0x10 + gTextBankOffsets[1]);
    *a++ = 0x165;
    *c++ = 0;
    *b++ = 0x68;
    *q++ = (void *) ((s32) D_80051ED0 + 0x24 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x69;
    *q++ = (void *) ((s32) D_80051ED0 + 0x36 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x6A;
    if (flags & 0x4000) {
        *q = (void *) ((s32) D_80051ED0 - 0x5E + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x1E;
    *c++ = 0;
    *b++ = 0x64;
    if (Ovl11MatchItemSlotsCommand(0x1D, 0, 2, 0) != 1) {
        *q = (void *) ((s32) D_80051F1C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    *a = 0x1D;
    *c = 0;
    *b = 0x281;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11BuildQuantityPriceRows(void) {
    s32 progressFlags;
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    gOvl11QuantityPrompt.unk0 = (s32) ((void *) ((u8 *) &D_80051E26 + gTextBankOffsets[1]));
    *(s32 *) ((u8 *) &gOvl11QuantityPrompt + 4) = (s32) D_80051E26 - 0x60C + gTextBankOffsets[1];
    progressFlags = gProgressDerivedFlags;
    gOvl11QuantityPrompt.unkA = Ovl11GetItemPrice(0x69U);
    gOvl11QuantityPrompt.unk8 = 1;
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);

    p = gOvl11PrimarySelectionRecord;
    *(s8 *) (p + 0x5A) = 0xF;
    *(s8 *) (p + 0x5B) = 0x15;
    *(s16 *) (p + 0x5C) = 0;

    q = (void **) p;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);

    *q = (void *) ((s32) &D_80051E26 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x69;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0x47;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) &D_80051E26 + 0x1A + gTextBankOffsets[1]);
    *a++ = 0x109;
    *c++ = 0;
    *b++ = 0x4A;
    *q++ = (void *) ((s32) &D_80051E26 + 0x32 + gTextBankOffsets[1]);
    *a++ = 0;
    *c++ = 3;
    *b++ = 0x49;
    if (progressFlags & 0x4000) {
        *q = (void *) ((s32) &D_80051E26 + 0x4C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x1E;
    *c++ = 0;
    *b++ = 0x48;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11ResetPriceRowDrawMode(void) {
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11BuildPairedPriceRows(void) {
    s32 flags;
    u8 *p;
    u8 *p2;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
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
    *q = (void *) ((s32) D_80051DD6 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0;
    *(s8 *) ((u8 *) q + 0x3C) = 1;
    *(s16 *) ((u8 *) q + 0x46) = 0xDF;
    q++;
    a++;
    c++;
    b++;
    *q = (void *) ((s32) D_80051DD6 + 0xE + gTextBankOffsets[1]);
    *a = 0;
    *c = 1;
    *b = 0xE0;

    p2 = gOvl11SecondarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p2);
    *(s8 *) (p2 + 0x5A) = 0xD;
    *(s8 *) (p2 + 0x5B) = 0x14;
    *(s16 *) (p2 + 0x5C) = 0;
    q = (void **) p2;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    *q = (void *) ((s32) D_80051DD6 + 0x28 + gTextBankOffsets[1]);
    *(s16 *) ((u8 *) q + 0x28) = 0x9B;
    *(s8 *) ((u8 *) q + 0x3C) = 0;
    *(s16 *) ((u8 *) q + 0x46) = 0xE1;
    q++;
    a++;
    c++;
    b++;
    *q++ = (void *) ((s32) D_80051DD6 + 0x3C + gTextBankOffsets[1]);
    *a++ = 0x9D;
    *c++ = 0;
    *b++ = 0xE3;
    if (flags & 1) {
        *q = (void *) ((s32) D_80051DD6 - 0x10C + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    q++;
    *a++ = 0x9C;
    *c++ = 0;
    *b++ = 0xE2;
    if (flags & 2) {
        *q = (void *) ((s32) D_80051CB0 + gTextBankOffsets[1]);
    } else {
        *q = 0;
    }
    *a = 0x9E;
    *c = 0;
    *b = 0xE4;
    gOvl11InteractionDrawMode = 0;
    return 1;
}

s32 Ovl11BuildRecordFlagOptionRows(void) {
    u8 *p;
    void **q;
    s16 *a;
    s8 *c;
    s16 *b;

    Ovl11RefreshProgressStatus();
    p = gOvl11PrimarySelectionRecord;
    Ovl11ClearSelectionRows((s32 *) p);
    *(s8 *) (p + 0x5A) = 0xC;
    *(s8 *) (p + 0x5B) = 0x17;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *(s16 *) (p + 0x5C) = 1;
    } else {
        *(s16 *) (p + 0x5C) = 0;
    }

    q = (void **) gOvl11PrimarySelectionRecord;
    a = (s16 *) ((u8 *) q + 0x28);
    c = (s8 *) ((u8 *) q + 0x3C);
    b = (s16 *) ((u8 *) q + 0x46);
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = NULL;
    } else {
        *q = (void *) ((s32) D_80051CF4 + gTextBankOffsets[1]);
    }
    q++;
    *a++ = 0x67;
    *c++ = 2;
    *b++ = -1;
    if (((struct Ovl11RecordFlagsView *) D_8006C838)->field_4A86 & 0x10) {
        *q = (void *) ((s32) D_80051D0C + gTextBankOffsets[1]);
    } else {
        *q = NULL;
    }
    q++;
    *a++ = 0x185;
    *c++ = 2;
    *b++ = -1;
    gOvl11InteractionDrawMode = 0;
    *q++ = (void *) ((s32) D_80051C7A + gTextBankOffsets[1]);
    *a++ = 0xF7;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x12 + gTextBankOffsets[1]);
    *a++ = 0xF8;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x24 + gTextBankOffsets[1]);
    *a++ = 0xF9;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x36 + gTextBankOffsets[1]);
    *a++ = 0x9E;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x50 + gTextBankOffsets[1]);
    *a++ = 0x9C;
    *c++ = 2;
    *b++ = -1;
    *q++ = (void *) ((s32) D_80051C7A + 0x6A + gTextBankOffsets[1]);
    *a++ = 0x5C;
    *c++ = 2;
    *b++ = -1;
    *q = (void *) ((s32) D_80051C7A + 0xA8 + gTextBankOffsets[1]);
    *a = 0x60;
    *c = 2;
    *b = -1;
    return 1;
}
