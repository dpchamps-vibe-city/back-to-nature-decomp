/*
 * counter_choice — reconstructed translation unit.
 *
 * Counter-driven message choices and their record-wide effects. The
 * entry/update protocol owns the choice state and candidate tables; effect
 * adapters and saturating halfword writers form its complete local helper
 * closure.
 *
 * A choice offers the two largest of six signed counters and one random
 * text row. Choosing a counter consumes one count and applies the associated
 * record-wide effect. Fades, messages and CD audio complete asynchronously.
 * Storage remains in the overlay data blob; this service is not reentrant.
 * See ovl11_counter_choice.h and notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_counter_choice.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "screen_fade.h"
#include "screen_primitives.h"
#include "cd_audio.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "message_window.h"
#include "ovl11_area_mode_sequence.h"
#include "sprite_rotation.h"
#include "ovl11_action_guards.h"
#include "game_flags.h"
#include "random.h"

u32 func_80017A64(void);
void func_80017A48(u32 context);
void func_80017A08(s32 slot, s32 textAddress);
void ovl_11_func_800BCF28(void);
void ovl_11_func_800CD670(s16 mode);
s32 ovl_11_func_80107C80(s16 id, u16 flags, s32 *position, s32 mode);

void Ovl11StepCounterChoice(void);
void Ovl11FinishCounterChoice(void);
void Ovl11BuildCounterChoiceText(s32 *textAddresses);
s32 Ovl11ApplyCounterChoice(s16 choice);
void Ovl11AddToAllRecordValues255(s16 increment);
void Ovl11AddToAllRecordValues65535(s32 increment);
void Ovl11AddToBothRecordPoolValues255(s16 increment);
void Ovl11AddToRecordValue255(s16 index, s16 increment);
void Ovl11AddToRecordValue65535(s16 index, s16 increment);
void Ovl11AddToObjectValue255(void *object, s16 increment);

void Ovl11ResetCounterChoice(void) {
    gOvl11CounterChoicePhase = 0;
    gOvl11CounterChoiceText[0] = 0;
    gOvl11CounterChoiceText[1] = 0;
    gOvl11CounterChoiceText[2] = 0;
    gOvl11CounterChoiceIndices.primaryCategory = -1;
    gOvl11CounterChoiceIndices.secondaryCategory = -1;
    gOvl11CounterChoiceIndices.randomTextIndex = -1;
    gOvl11CounterChoiceMessage = -1;
    gOvl11CounterChoiceSavedSelection = 0;
}

char *Ovl11BeginCounterChoice(void) {
    char *upperWork;
    char *workStart;
    u32 savedSelection;
    s16 *phase;

    Ovl11ResetCounterChoice();
    phase = &gOvl11CounterChoicePhase;

    /* Preserve the work origin separately: folding the upper-half base into
     * each access changes the original address formation. */
    workStart = (char *)&D_8006C838;
    upperWork = workStart + 0x8000;

    savedSelection = *(u16 *)(upperWork + 0x64D2);
    *phase = 1;
    *(u16 *)(upperWork + 0x64D2) = 0;
    gOvl11CounterChoiceSavedSelection = savedSelection;
    return upperWork;
}

void Ovl11UpdateCounterChoice(void) {
    u32 previousTextContext;

    if (gOvl11CounterChoicePhase != 0) {
        previousTextContext = func_80017A64();
        func_80017A48(3U);
        Ovl11StepCounterChoice();
        func_80017A48(previousTextContext);
    }
}

void Ovl11StepCounterChoice(void) {
    switch (gOvl11CounterChoicePhase) {
    case 1:
        StopSoundSequence(0);
        BeginScreenFadeIncrease(30, 1, 2);
        BeginMasterVolumeFadeOut(30);
        gOvl11CounterChoicePhase = 2;
        break;
    case 2:
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            gOvl11CounterChoicePhase = 3;
            Ovl11BuildCounterChoiceText(gOvl11CounterChoiceText);
            BeginMasterVolumeFadeIn(1);
        }
        break;
    case 3:
        func_80017A08(0, gOvl11CounterChoiceText[0]);
        func_80017A08(1, gOvl11CounterChoiceText[1]);
        func_80017A08(2, gOvl11CounterChoiceText[2]);
        RequestMessageWindow(2, 0xFD);
        if (GetMessageWindowStatus() == 2) {
            gOvl11CounterChoiceMessage = Ovl11ApplyCounterChoice((s16)GetMessageWindowChoice());
            gOvl11CounterChoicePhase = 4;
        }
        break;
    case 4:
        RequestMessageWindow(2, gOvl11CounterChoiceMessage);
        if (GetMessageWindowStatus() == 2) {
            if (gOvl11CounterChoiceMessage == 0x10A) {
                PlaySoundSequenceWithReverb(0, 0, ((struct Ovl11SelectionMusic *)&D_8007AFF0)->music, 0);
                Ovl11ApplyAreaSoundVolume();
                BeginScreenFadeDecrease(10, 1, 2);
                BeginMasterVolumeFadeIn(10);
                gOvl11CounterChoicePhase = 8;
            } else {
                DrawScreenTileRect((u32 *)gActiveRenderContext->field_120, 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 0);
                BeginScreenFadeIncrease(10, 0, 1);
                BeginMasterVolumeFadeIn(10);
                gOvl11CounterChoicePhase = 5;
            }
        }
        break;
    case 5:
        DrawScreenTileRect((u32 *)gActiveRenderContext->field_120, 0, 0, *(s16 *)&HWD0, *(s16 *)&VWD0, 0, 0);
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            StartCdAudioTrack(25);
            gOvl11CounterChoicePhase = 6;
        }
        break;
    case 6:
        if (GetCdAudioState() == 0) {
            gOvl11CounterChoiceDelay = 30;
            gOvl11CounterChoicePhase = 7;
        }
        break;
    case 7:
        if (--gOvl11CounterChoiceDelay <= 0) {
            BeginScreenFadeDecrease(10, 0, 1);
            PlaySoundSequenceWithReverb(0, 0, ((struct Ovl11SelectionMusic *)&D_8007AFF0)->music, 0);
            Ovl11ApplyAreaSoundVolume();
            BeginMasterVolumeFadeIn(10);
            gOvl11CounterChoicePhase = 8;
        }
        break;
    case 8:
        if (IsScreenFadeComplete() == 1 && GetMasterVolumeRampState() == 0) {
            ResetScreenFade();
            gOvl11CounterChoicePhase = 9;
        }
        break;
    case 9:
        Ovl11FinishCounterChoice();
        break;
    }
}

void Ovl11FinishCounterChoice(void) {
    Ovl11C7010Object *object;

    ((struct Ovl11SelectionResetState *) D_8006C838)->savedSelection = (u16) gOvl11CounterChoiceSavedSelection;
    Ovl11ResetCounterChoice();
    object = (void *) ((u8 *) D_8006C838 + 0x51C8);
    ovl_11_func_800BCF28();
    object->unk6C = (s32) (object->unk6C & ~0x400);
    ovl_11_func_800CD670(0x14);
    ovl_11_func_80107C80(0x20, 0U, NULL, 1);
    ((struct Ovl11SelectionResetMode *) &D_8007AFF0)->mode = 2;
    SetGameFlag(0x76U, 1U);
}

/* At least two categories must have counts >= 0; otherwise -1 remains
 * an unchecked table index. Only a tie between the two winners consumes
 * the extra Rand(2) call. Text addresses retain the existing 32-bit ABI. */
void Ovl11BuildCounterChoiceText(s32 *textAddresses) {
    s16 *counts;
    s32 i;
    s16 primary;
    s16 secondary;
    s16 randomRow;
    s32 highest;
    s32 nextHighest;
    s16 swap;

    counts = gCounterChoiceCategoryCounts;
    highest = -1;
    primary = -1;
    nextHighest = -1;
    secondary = -1;
    for (i = 0; i < 6; i++) {
        if (highest < counts[i]) {
            primary = i;
            highest = counts[i];
        }
    }
    for (i = 0; i < 6; i++) {
        if (primary != i && nextHighest < counts[i]) {
            secondary = i;
            nextHighest = counts[i];
        }
    }
    if (highest == nextHighest) {
        if (Rand(2) != 0) {
            swap = primary;
            primary = secondary;
            secondary = swap;
        }
    }
    randomRow = Rand(0x13);
    textAddresses[0] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + gOvl11CounterChoiceCategoryText[primary].textOffset));
    textAddresses[1] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + gOvl11CounterChoiceCategoryText[secondary].textOffset));
    textAddresses[2] = (s32)((u8 *)D_8005175C + (gTextBankOffsets[0] + gOvl11CounterChoiceRandomText[randomRow].textOffset));
    gOvl11CounterChoiceIndices.primaryCategory = primary;
    gOvl11CounterChoiceIndices.secondaryCategory = secondary;
    gOvl11CounterChoiceIndices.randomTextIndex = randomRow;
}

/* Choices 1/2 use the selected counter categories, 3 chooses one of the
 * random row's two messages, and 4 exits without consuming a counter.
 * A category with zero count returns 0x1E9 without applying effects. */
s32 Ovl11ApplyCounterChoice(s16 choice) {
    s16 message;
    s16 category;
    s16 *counts;

    if (choice == 4) {
        return 0x10A;
    }
    if (choice == 3) {
        s16 *messages;
        s32 column;

        messages = gOvl11CounterChoiceRandomWords;
        column = Rand(2) + 1;
        return messages[gOvl11CounterChoiceRandomIndex * 3 + column];
    }
    counts = gCounterChoiceCategoryCounts;
    category = gOvl11CounterChoiceIndexWords[choice - 1];
    if (counts[category] == 0) {
        message = 0x1E9;
    } else if (counts[category] == 1) {
        message = gOvl11CounterChoiceCategoryText[category].singleMessage;
    } else {
        message = gOvl11CounterChoiceCategoryText[category].multipleMessage;
    }
    if (message != 0x1E9) {
        counts[gOvl11CounterChoiceIndexWords[choice - 1]] = counts[gOvl11CounterChoiceIndexWords[choice - 1]] - 1;
        switch (gOvl11CounterChoiceIndexWords[choice - 1]) {
        case 0:
            Ovl11AddToAllRecordValues255(1);
            Ovl11AddToAllRecordValues65535(0xC8);
            return message;
        case 1:
            Ovl11AddToAllRecordValues255(1);
            Ovl11AddToAllRecordValues65535(0x64);
            return message;
        case 2:
            Ovl11AddToBothRecordPoolValues255(1);
            return message;
        case 3:
            Ovl11AddToAllRecordValues255(0xA);
            Ovl11AddToAllRecordValues65535(0x1F4);
            return message;
        case 4:
            Ovl11AddToBothRecordPoolValues255(1);
            break;
        }
    }
    return message;
}

void Ovl11AddToAllRecordValues255(s16 increment) {
    s16 narrowedIncrement;
    s32 i;

    narrowedIncrement = increment;
    for (i = 0; i < 0x25; i++) {
        Ovl11AddToRecordValue255((s16)i, narrowedIncrement);
    }
}

void Ovl11AddToAllRecordValues65535(s32 increment) {
    s16 narrowedIncrement;
    s32 i;

    narrowedIncrement = increment;
    for (i = 0; i < 0x25; i++) {
        Ovl11AddToRecordValue65535((s16)i, narrowedIncrement);
    }
}

void Ovl11AddToBothRecordPoolValues255(s16 increment) {
    s32 i;

    for (i = 0; i < (((Ovl11Status99C8View *)&D_8006C838)->field_44D2 == 0 ? 5 : 10); i++) {
        Ovl11AddToObjectValue255((u8 *)&D_800742EC + i * 0xB4, increment);
    }
    for (i = 0; i < (((Ovl11Status99C8View *)&D_8006C838)->field_44D0 == 0 ? 10 : 20); i++) {
        Ovl11AddToObjectValue255((u8 *)&gTaggedRecordBase + i * 0xB8, increment);
    }
}

/* These are upper-only clamps, not general saturating arithmetic. The
 * signed comparison and unsigned value read deliberately use the same
 * halfword; negative deltas/initial values are not normalized. */
void Ovl11AddToRecordValue255(s16 index, s16 increment) {
    char *work;
    char *record;
    char *upperRecord;
    s32 limit;
    s32 current;
    u32 value;
    s32 total;
    s32 delta;
    s32 cap;

    delta = increment;
    work = (char *)&D_8006C838;
    record = work + index * 0x1D4;
    upperRecord = record + 0x8000;
    cap = 0xFF;
    limit = cap - delta;
    current = *(s16 *)(upperRecord + 0x19EA);
    value = *(u16 *)(upperRecord + 0x19EA);
    if (current < limit) {
        total = delta + value;
        *(s16 *)(upperRecord + 0x19EA) = total;
    } else {
        *(s16 *)(upperRecord + 0x19EA) = cap;
    }
}

void Ovl11AddToRecordValue65535(s16 index, s16 increment) {
    char *work;
    char *record;
    u16 value;

    work = (char *)&D_8006C838;
    record = work + index * 0x1D4;
    value = *(u16 *)(record + 0x8000 + 0x19EC);
    if (value < 0xFFFF - increment) {
        *(u16 *)(record + 0x8000 + 0x19EC) = value + increment;
    } else {
        *(u16 *)(record + 0x8000 + 0x19EC) = 0xFFFF;
    }
}

void Ovl11AddToObjectValue255(void *object, s16 increment) {
    s32 limit;
    s32 current;
    u32 value;
    s32 total;
    s32 delta;
    s32 cap;

    delta = increment;
    cap = 0xFF;
    limit = cap - delta;
    current = *(s16 *)((char *)object + 0x16);
    value = *(u16 *)((char *)object + 0x16);
    if (current < limit) {
        total = delta + value;
        *(s16 *)((char *)object + 0x16) = total;
    } else {
        *(s16 *)((char *)object + 0x16) = cap;
    }
}
