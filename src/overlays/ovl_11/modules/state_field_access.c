/* Descriptor-driven state-field access and rule arithmetic.
 * The 48 descriptors are read-only overlay data, not C-owned storage.
 * See ovl11_state_field_access.h for ID spaces and access constraints;
 * membership and pooled-table evidence are in notes/file-groupings.md. */
#include "ovl11_state_field_access.h"
#include "debug_break.h"
#include "game_flags.h"

s32 Ovl11ClampStateField(s32 fieldId, s32 elementIndex, s32 value);

void Ovl11UpdateStateField(s32 fieldId, s32 elementIndex, s32 operand, s32 operation) {
    s32 kind;
    s32 value;
    s32 mask;
    u8 *data;

    if ((u32)(fieldId - 0x31) < 0x2EFU) {
        SetGameFlag((fieldId - 0x30) & 0xFFFF, operand & 0xFFFF);
        return;
    }
    data = Ovl11ResolveStateField(fieldId, elementIndex, &kind);
    switch (kind) {
    case 0:
        value = *(u8 *)data;
        break;
    case 1:
        value = *(u8 *)data;
        break;
    case 2:
        value = *(u16 *)data;
        break;
    case 3:
        value = *(s16 *)data;
        break;
    case 4:
    case 5:
    default:
        value = *(s32 *)data;
        break;
    }
    mask = gOvl11StateFields[fieldId].mask;
    if (mask != 0) {
        if (operand != 0) {
            value = value | mask;
        } else {
            value = value & ~mask;
        }
    } else {
        Ovl11ApplyStateFieldOperation(&value, operand, operation);
        value = Ovl11ClampStateField(fieldId, elementIndex, value);
    }
    switch (kind) {
    case 0:
    case 1:
        *(u8 *)data = value;
        return;
    case 2:
    case 3:
        *(u16 *)data = value;
        return;
    case 4:
    case 5:
    default:
        *(s32 *)data = value;
        return;
    }
}

s32 Ovl11ReadStateField(s32 fieldId, s32 elementIndex) {
    s32 kind;
    s32 masked;
    s32 mask;
    s32 value;
    u8 *data;

    if ((u32)(fieldId - 0x31) < 0x2EFU) {
        value = GetGameFlag((fieldId - 0x30) & 0xFFFF);
    } else {
        data = Ovl11ResolveStateField(fieldId, elementIndex, &kind);
        switch (kind) {
        case 0:
        case 1:
            value = (s32)*data;
            break;
        case 2:
            value = *(u16 *)data;
            break;
        case 3:
            value = *(s16 *)data;
            break;
        case 4:
        case 5:
        default:
            value = *(s32 *)data;
            break;
        }
        mask = gOvl11StateFields[fieldId].mask;
        if (mask != 0) {
            masked = value & mask;
            value = masked != 0;
        }
    }
    /* This asymmetric read adjustment is intentional; writes do not undo it. */
    if ((fieldId == 0xC) || (fieldId == 0xE)) {
        value += 1;
    }
    return value;
}

/* elementIndex is retained in the original ABI but not used by the clamp. */
s32 Ovl11ClampStateField(s32 fieldId, s32 elementIndex, s32 value) {
    s32 *maximumAddress;
    s16 minimum;
    s32 maximum;
    s32 result;

    result = value;
    minimum = gOvl11StateFields[fieldId].minimum;
    maximumAddress = &gOvl11StateFields[fieldId].maximum;
    maximum = *maximumAddress;
    if (result < minimum) {
        result = minimum;
    } else if (maximum < result) {
        result = maximum;
    }
    return result;
}

/* Rule condition codes: >=, >, <=, <, ==; other codes are true. */
s32 Ovl11TestStateFieldCondition(s32 value, s32 operand, s32 condition) {
    s32 result = 0;
    switch (condition) {
    case 0:
        if (value >= operand) {
            result = 1;
        }
        break;
    case 1:
        if (operand < value) {
            result = 1;
        }
        break;
    case 2:
        if (operand >= value) {
            result = 1;
        }
        break;
    case 3:
        if (value < operand) {
            result = 1;
        }
        break;
    case 4:
        if (value == operand) {
            result = 1;
        }
        break;
    default:
        result = 1;
        break;
    }
    return result;
}

s32 Ovl11ApplyStateFieldOperation(s32 *value, s32 operand, s32 operation) {
    switch (operation) {
    case 0:
        *value = operand;
        break;
    case 1:
        *value = *value + operand;
        break;
    case 2:
        *value = *value - operand;
        break;
    case 3:
        *value = *value * operand;
        break;
    case 4:
        if (operand != 0) {
            *value = *value / operand;
        }
        break;
    case 5:
        if (operand != 0) {
            *value = *value % operand;
        }
        break;
    }
    return 1;
}

u8 *Ovl11ResolveStateField(s32 fieldId, s32 elementIndex, s32 *kindOut) {
    s32 localKind;
    u8 *data;
    s16 stride;

    if (kindOut == 0) {
        kindOut = &localKind;
    }
    stride = gOvl11StateFields[fieldId].elementStride;
    if (stride != 0) {
        data = gOvl11StateFields[fieldId].data + stride * elementIndex;
    } else {
        data = gOvl11StateFields[fieldId].data;
    }
    *kindOut = gOvl11StateFields[fieldId].kind;
    if (data == 0) {
        do {
        } while (DebugBreakAndReturnTrue() != 0);
    }
    return data;
}
