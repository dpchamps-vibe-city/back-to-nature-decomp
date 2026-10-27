/*
 * Pure packed-field decoders for the condition-record evaluator.
 * The bounds and flag-ID words use different bit orders and sentinel rules;
 * keep those formats distinct. The switches retain the original pooled-table
 * order; the flag-ID selector's two identical branches are intentional.
 * Public contracts: ovl11_condition_record_fields.h.
 */
#include "common.h"
#include "ovl11_condition_record_fields.h"

void Ovl11DecodeConditionBounds(u32 packed, u16 *bounds) {
    s32 fieldIndex;
    u16 value;
    u16 limit;

    for (fieldIndex = 0; fieldIndex < 6; fieldIndex++) {
        switch (fieldIndex) {
        case 0:
            value = (packed >> 25) & 0x1F;
            limit = 1;
            break;
        case 1:
            value = (packed >> 20) & 0x1F;
            limit = 0x7FFF;
            break;
        case 2:
            value = (packed >> 15) & 0x1F;
            limit = 0x1F;
            break;
        case 3:
            value = (packed >> 10) & 0x1F;
            limit = 0x1F;
            break;
        case 4:
            value = (packed >> 5) & 0x1F;
            limit = 0x1F;
            break;
        case 5:
            value = packed & 0x1F;
            limit = 0x1F;
            break;
        default:
            value = 0;
            limit = 0;
            break;
        }
        if (value == 0x1F) {
            value = limit;
        }
        switch (fieldIndex) {
        case 0:
            bounds[fieldIndex] = value - 1;
            break;
        case 1:
            bounds[fieldIndex] = value - 1;
            break;
        case 2:
            bounds[fieldIndex] = value - 1;
            break;
        case 3:
            bounds[fieldIndex] = value;
            break;
        case 4:
            bounds[fieldIndex] = value;
            break;
        case 5:
            bounds[fieldIndex] = value;
            break;
        default:
            bounds[fieldIndex] = value;
            break;
        }
    }
}

void Ovl11DecodeConditionFlagIds(u32 packed, s16 *ids, s32 selector) {
    s32 fieldIndex;
    s32 value;

    for (fieldIndex = 0; fieldIndex < 3; fieldIndex++) {
        if (selector == 1) {
            switch (fieldIndex) {
            case 0:
                value = (u16)(packed & 0x3FF);
                break;
            case 1:
                value = (u16)((packed >> 10) & 0x3FF);
                break;
            case 2:
                value = (u16)((packed >> 20) & 0x3FF);
                break;
            default:
                value = 0;
                break;
            }
        } else {
            switch (fieldIndex) {
            case 0:
                value = (u16)(packed & 0x3FF);
                break;
            case 1:
                value = (u16)((packed >> 10) & 0x3FF);
                break;
            case 2:
                value = (u16)((packed >> 20) & 0x3FF);
                break;
            default:
                value = 0;
                break;
            }
        }
        *ids++ = value;
    }
}

s32 Ovl11GetConditionRecordSlot(s32 selector) {
    u32 selector16;

    selector16 = selector & 0xFFFF;
    switch (selector16) {
    case 0:
        return 3;
    case 1:
        return 7;
    case 2:
        return 9;
    case 3:
        return 0xE;
    case 4:
        return 0x14;
    default:
        return 3;
    }
}
