/*
 * Category-sensitive comparison codes, ovl_11:800EE000–800EE240.
 * Three slot low-halfwords select a code; only the output slot is changed.
 * Code meanings beyond the mapping below are not established.
 */
#include "common.h"
#include "ovl11_slot_comparison_code.h"

s32 Ovl11WriteSlotComparisonCode(s16 categorySlot, s16 leftSlot,
                                 s32 rightSlot, s32 outputSlot) {
    s16 rightValue;
    s16 categoryId;
    s16 leftValue;
    s16 resultCode;
    s16 categoryIndex;
    s32 referenceValue;

    categoryId = *(s16 *)&gOvl11ValueSlots[categorySlot];
    leftValue = *(s16 *)&gOvl11ValueSlots[leftSlot];
    rightValue = *(s16 *)&gOvl11ValueSlots[rightSlot];
    switch (categoryId) {
    case 19:
        categoryIndex = 0;
        referenceValue = 0x14;
        break;
    case 20:
        categoryIndex = 1;
        referenceValue = 0x14;
        break;
    case 16:
        categoryIndex = 2;
        referenceValue = 9;
        break;
    case 5:
        categoryIndex = 3;
        referenceValue = 7;
        break;
    case 21:
    case 22:
        referenceValue = 0x16;
        categoryIndex = 4;
        break;
    case 34:
        categoryIndex = 5;
        referenceValue = 9;
        break;
    case 9:
        categoryIndex = 6;
        referenceValue = 9;
        break;
    case 11:
        referenceValue = 0x16;
        categoryIndex = 7;
        break;
    case 6:
        categoryIndex = 8;
        referenceValue = 7;
        break;
    case 7:
        categoryIndex = 9;
        referenceValue = 7;
        break;
    case 25:
        referenceValue = 0x14;
        categoryIndex = 0xA;
        break;
    default:
        categoryIndex = 0xB;
        referenceValue = -1;
        break;
    }
    /* -1 in the right slot selects the single-value code family. */
    if (rightValue == -1) {
        if (categoryIndex < 5) {
            if (categoryId == 0x16) {
                resultCode = categoryIndex + 0x70E;
            } else {
                resultCode = categoryIndex + 0x70D;
            }
        } else {
            resultCode = categoryIndex + 0x70E;
        }
    } else if (leftValue == rightValue) {
        categoryIndex *= 2;
        switch (categoryId) {
        case 5:
        case 11:
        case 16:
        case 25:
            if (rightValue == referenceValue) {
                resultCode = categoryIndex + 0x548;
            } else {
                resultCode = categoryIndex + 0x549;
            }
            break;
        default:
            if (rightValue == referenceValue) {
                resultCode = categoryIndex + 0x549;
            } else {
                resultCode = categoryIndex + 0x548;
            }
            break;
        }
    } else {
        categoryIndex *= 3;
        switch (categoryId) {
        case 5:
        case 11:
        case 16:
        case 25:
            if (rightValue == referenceValue) {
                resultCode = categoryIndex + 0x526;
            } else if (leftValue == referenceValue) {
                resultCode = categoryIndex + 0x527;
            } else {
                resultCode = categoryIndex + 0x528;
            }
            break;
        default:
            if (rightValue == referenceValue) {
                resultCode = categoryIndex + 0x528;
            } else if (leftValue == referenceValue) {
                resultCode = categoryIndex + 0x527;
            } else {
                resultCode = categoryIndex + 0x526;
            }
            break;
        }
    }
    gOvl11ValueSlots[outputSlot] = resultCode;
    return 1;
}
