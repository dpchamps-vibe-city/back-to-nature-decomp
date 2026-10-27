/*
 * Complete populated object-state entry-callback set (slots 5, 6 and 7).
 * A message gate, a value debit and a no-op gate share the entry protocol;
 * the dispatch table and general state selection live elsewhere.
 * See ovl11_object_state_callbacks.h and notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_state_callbacks.h"
#include "message_window.h"
#include "game_flags.h"

s32 Ovl11ApplyStateValueDebit(Ovl11StateValueView *object) {
    s32 difference;
    s32 clamped;

    difference = object->value - (u16)gOvl11StateValueDebit;
    object->value = difference;
    if ((s16)difference < 0) {
        clamped = 0;
    } else {
        clamped = difference;
    }
    object->value = clamped;
    return 0;
}

int Ovl11StateEntryNoop(void) {
    return 0;
}

s32 Ovl11RequestObjectStateMessage(Ovl11D4030Arg *object) {
    s16 selector;
    s16 modeSelector;
    s32 message;
    u16 objectId;

    objectId = object->unk0;
    message = -1;
    switch (objectId) {
    case 0x108:
        selector = object->unk30;
        if (selector == 0x1B) {
            if (GetGameFlag(0x9EU) == 0) {
                SetGameFlag(0x9EU, 1U);
                message = 0x26B;
            } else {
                message = 0x263;
            }
        } else if (selector == 0x1F) {
            if (GetGameFlag(0x9FU) == 0) {
                SetGameFlag(0x9FU, 1U);
                message = 0x26A;
            } else {
                message = 0x264;
            }
        }
        break;
    case 0x162:
        message = 0x261;
        break;
    case 0x165:
        message = 0x262;
        break;
    case 0x17A:
        modeSelector = object->unk30;
        switch (modeSelector) {
        case 0x9:
            message = 0x266;
            if (((struct struct_8006C838_800D4030 *) D_8006C838)->field_99D8 == modeSelector) {
                message = 0x277;
            }
            break;
        case 0x19:
            message = 0x269;
            break;
        case 0x18:
            message = 0x267;
            break;
        case 0x35:
            message = 0x268;
            break;
        }
        break;
    case 0x17B:
        message = 0x265;
        break;
    default:
        message = -1;
        break;
    }
    if (message != -1) {
        RequestMessageWindow(2, message);
    }
    return -1;
}
