/* Purchase callbacks for selection-table entries 4..14. Each call advances
 * the shared transaction state once; no two requests may run concurrently.
 * Catalog IDs stay numeric because their gameplay identities are unverified.
 * Membership and boundaries: notes/file-groupings.md. */

#include "common.h"
#include "message_window.h"
#include "game_types.h"
#include "ovl11_purchase_actions.h"
#include "ovl11_purchase_transactions.h"

/* The dispatcher consumes the transaction helper's returned completion word.
 * Explicit returns preserve the original v0 pass-through, not a void ABI. */
s32 Ovl11PurchaseCatalogEntry99(void) {
    return Ovl11StepSingleItemPurchase(0x99, 2, 0x1DD, 0);
}

s32 Ovl11PurchaseCatalogEntry9A(void) {
    return Ovl11StepSingleItemPurchase(0x9A, 2, 0x1DE, 0);
}

s32 Ovl11PurchaseCatalogEntryA5(void) {
    return Ovl11StepSingleItemPurchase(0xA5, 2, 0xD9, 0);
}

s32 Ovl11PurchaseCatalogEntryA6(void) {
    return Ovl11StepSingleItemPurchase(0xA6, 2, 0xDA, 0);
}

s32 Ovl11PurchaseCatalogEntryA7(void) {
    return Ovl11StepSingleItemPurchase(0xA7, 2, 0xDB, 0);
}

s32 Ovl11PurchaseFlagSelectedEntry(void) {
    if (gProgressDerivedFlags & 0x40) {
        return Ovl11StepSingleItemPurchase(0x6D, 2, 0xD4, 1);
    }
    if (gProgressDerivedFlags & 0x80) {
        return Ovl11StepSingleItemPurchase(0x6E, 2, 0x248, 1);
    }
    return 1;
}

s32 Ovl11PurchaseGatedEntry64(void) {
    if (((struct Ovl11CallbackGateState *)D_8006C838)->flags & 0x20) {
        if (((struct Ovl11CallbackGateState *)D_8006C838)->status & 0x800) {
            RequestMessageWindow(2, 0xD6);
            return 1;
        }
        return Ovl11StepSingleItemPurchase(0x64, 2, 0xD5, 2);
    }
    return 1;
}

/* Keep descriptor stores in their original order. The four requests share
 * selection storage, but use different prompt/cancellation message pairs. */
s32 Ovl11PurchaseQuantityOption0(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0xC0;
    next[0] = 2;
    next[1] = 0xBB;
    cancel[0] = 2;
    cancel[1] = 0xC2;
    params.unk4 = 2;
    params.unk6 = 0xBE;
    params.unk8 = 2;
    params.unkA = 0xBF;
    params.unkC = 2;
    params.unkE = 0xBA;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    if (((Ovl11FB908State *)D_8006C838)->field_44D4 == 0) {
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 0) {
            return 1;
        }
    } else {
        params.unk4 = 2;
        params.unk6 = 0x275;
    }
    return Ovl11StepQuantityPurchase(start, next, cancel, params);
}

s32 Ovl11PurchaseQuantityOption1(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0xC0;
    next[0] = 2;
    next[1] = 0xBB;
    cancel[0] = 2;
    cancel[1] = 0xC2;
    params.unk4 = 2;
    params.unk6 = 0x273;
    params.unk8 = 2;
    params.unkA = 0xBF;
    params.unkC = 2;
    params.unkE = 0xBA;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    if (((Ovl11FB908State *)D_8006C838)->field_44D4 == 0) {
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 1) {
            return 1;
        }
    } else {
        params.unk4 = 2;
        params.unk6 = 0x275;
    }
    return Ovl11StepQuantityPurchase(start, next, cancel, params);
}

s32 Ovl11PurchaseQuantityOption2(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0xC0;
    next[0] = 2;
    next[1] = 0xBB;
    cancel[0] = 2;
    cancel[1] = 0xC2;
    params.unk4 = 2;
    params.unk6 = 0x274;
    params.unk8 = 2;
    params.unkA = 0xBF;
    params.unkC = 2;
    params.unkE = 0xBA;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    if (((Ovl11FB908State *)D_8006C838)->field_44D4 == 0) {
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 2) {
            return 1;
        }
    } else {
        params.unk4 = 2;
        params.unk6 = 0x275;
    }
    return Ovl11StepQuantityPurchase(start, next, cancel, params);
}

s32 Ovl11PurchaseQuantityFixed(void) {
    s16 start[2];
    s16 next[2];
    s16 cancel[2];
    Ovl11D8012D7B8 params;

    start[0] = 2;
    start[1] = 0x16E;
    next[0] = 2;
    next[1] = 0x170;
    cancel[0] = 2;
    cancel[1] = 0x171;
    params.unk4 = 2;
    params.unk6 = 0x164;
    params.unk8 = 2;
    params.unkA = 0x16D;
    params.unkC = 2;
    params.unkE = 0x16F;
    params.unk10 = 4;
    params.unk0 = gOvl11PrimarySelectionRecord;
    return Ovl11StepQuantityPurchase(start, next, cancel, params);
}
