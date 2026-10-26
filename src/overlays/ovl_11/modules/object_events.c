/*
 * object_events — reconstructed translation unit.
 *
 * The shared object-event lifecycle, scheduled-command interpreter,
 * animation/reaction services and time-of-day reconstruction.
 *
 * ovl11_object_events.h is the facade; pool/script headers retain the detailed
 * lifecycle and command contracts. All views describe the same 0x1D4-byte
 * records; no private linkage or storage ownership is inferred by this merge.
 * Membership and evidence-limited boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "psyq/memory.h"
#include "random.h"
#include "sprite_source.h"
#include "ovl11_collision_query_budget.h"
#include "screen_fade.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_object_event_pool.h"
#include "sprite_rotation.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_object_event_script.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_object_events.h"
#include "ovl11_projected_record_sprites.h"
#include "psyq/libgs.h"
#include "psyq/libcd.h"
#include "ovl11_area_resource_load.h"
#include "ovl11_surface_height.h"

s32 Ovl11BeginObjectCommandRow(Ovl11C3B78Arg0 *arg0, s32 arg1, s32 arg2);

void Ovl11RebuildObjectEventsForTime(void) {
    s16 sceneId;
    u16 clearedFlags;
    s32 sentinel;
    s32 slotIndex;
    s32 workOrElapsed;
    s32 flagsOrIndex;
    s32 time;
    struct Ovl11EventInitRecord *object;
    struct Ovl11EventInitWork *work;

    object = (struct Ovl11EventInitRecord *)&gObjectEventSlotBase;
    ((struct Ovl11EventInitState *)D_8006C838)->fieldCB70 = 0x13;
    ((struct Ovl11EventInitState *)D_8006C838)->fieldCB72 = 0x28;
    gOvl11ObjectScriptLiveMode = 0;
    for (slotIndex = 0; slotIndex < 37; slotIndex++, object++) {
        sentinel = 0x36;
        object->field10 = 0;
        object->field30 = 0;
        object->field34 = 0;
        object->field38 = 0;
        clearedFlags = object->flags & 0xBEBC;
        object->flags = clearedFlags;
        work = (struct Ovl11EventInitWork *)D_8009AFF0;
        object->flags20 &= 0xCFFF;
        sceneId = work->header[1];
        gOvl11ObjectScriptTick = 0;
        if ((sceneId != sentinel) && (object->field2E == sceneId) &&
            (Ovl11SelectScriptSpriteSource(object->id, (SpriteSourceData *)((u8 *)object + 0x80)) != 0)) {
            if (object->id == 0x1B) {
                object->field2C = sentinel;
                object->field2E = sentinel;
                continue;
            }
            if (Ovl11BeginObjectCommandRow((Ovl11C3B78Arg0 *)object, gOvl11ObjectScriptLiveMode, 0) != 0) {
                object->flags |= 0x100;
                Ovl11DispatchCurrentScriptCommand((Ovl11C3B78Arg0 *)object);
                Ovl11LoadScheduledEvent((Ovl11EventObject *)object);
            } else {
                flagsOrIndex = object->flags;
                flagsOrIndex |= 0x100;
                object->flags = flagsOrIndex;
                workOrElapsed = (s32)D_8006C838;
                time = ((struct Ovl11EventInitState *)workOrElapsed)->time;
                workOrElapsed = 0;
                if (time >= 0x168) {
                    flagsOrIndex = object->index;
                    workOrElapsed = time - (object->entries[flagsOrIndex].unk0 + object->offset);
                }
                if (workOrElapsed >= 0) {
                    Ovl11RunObjectScriptTicks((Ovl11C3B78Arg0 *)object, gOvl11ClockMinutePeriodTwice * workOrElapsed / 2);
                } else {
                    object->flags &= 0xFEFF;
                }
                Ovl11LoadScheduledEvent((Ovl11EventObject *)object);
            }
        }
    }
    gOvl11ObjectScriptLiveMode = 1;
}

/*
 * Lifecycle and counter maintenance for 37 shared object-event records.
 * Each slot is 0x1D4 bytes; signed ID -1 marks it free. Allocation raises
 * flag 0x8000, consumed by the event-script update pass below.
 * Original order: ovl_11:800C1BA0..800C1F94 exclusive.
 * Public contracts: ovl11_object_event_pool.h. Grouping: notes/file-groupings.md.
 */
void ovl_11_func_800F0C70(void);

/* Internal callers only, but original external linkage is retained. The
 * initializer's s32 argument is an ADDRESS, not a numeric slot index. */
void Ovl11InitEventSlot(s32 slotAddress);
void Ovl11AllocateEventSlot(s32 id);

void Ovl11AllocateAllEventSlots(void) {
    s32 id;

    id = 0;
    do {
        Ovl11AllocateEventSlot(id);
        id += 1;
    } while (id < 0x25);
    ovl_11_func_800F0C70();
}

void Ovl11ClearEventSlotTimeOrigins(void) {
    u32 index;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    for (index = 0; index < 37; index++) {
        slot->unkA = 0;
        slot++;
    }
}

void Ovl11ResetAllEventSlots(void) {
    s32 remaining;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    remaining = 0x24;
    do {
        Ovl11InitEventSlot((s32) slot);
        Ovl11ResetTimedSpriteEffect((s16 *) &slot->unk30[0xB0]);
        remaining -= 1;
        slot += 1;
    } while (remaining >= 0);
    ovl_11_func_800F0C70();
}

void Ovl11InitEventSlot(s32 slotAddress) {
    memset(slotAddress, 0, 0x1D4);
    *(s16 *)slotAddress = -1;
}

void Ovl11ClearEventSlotScheduleChanges(void) {
    s32 remaining;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    remaining = 0x24;
    do {
        slot->unk1E &= ~0x10;
        remaining--;
        slot++;
    } while (remaining >= 0);
}

/* First-free scan. A full pool silently leaves state unchanged. Stored IDs
 * narrow to s16; initial counters depend on SLOT index, not the requested ID. */
void Ovl11AllocateEventSlot(s32 id) {
    s32 slotIndex;
    struct_80076220 *slot;

    slot = &gObjectEventSlotBase;
    slotIndex = 0;
search_slot:
    if ((*(s16 *) ((u8 *) slot + 0)) == -1) {
        Ovl11InitEventSlot((s32) slot);
        (*(s16 *) ((u8 *) slot + 0)) = id;
        slot->unk1E |= 0x8000;
        Ovl11ResetTimedSpriteEffect((s16 *) &slot->unk30[0xB0]);
        if (Ovl11IsSpecialCounterSlot((s16) slotIndex) == 1) {
            slot->unk4 = 0;
            slot->unk2 = 0;
            return;
        }
        slot->unk4 = 0;
        slot->unk2 = 0x32;
        return;
    }
    slotIndex += 1;
    slot += 1;
    if (slotIndex >= 0x25) {
        return;
    }
    goto search_slot;
}

void Ovl11UpdateEventSlotCounters(void) {
    u32 slotIndex;
    struct_80076220 *slot;
    s32 value;
    s32 count;
    s32 kind;
    s32 one;
    u8 *work;

    slot = &gObjectEventSlotBase;
    slotIndex = 0;
    one = 1;
    /* Reuse the shared-state anchor of the first record for the selected
     * slot word. The 0x19E8 offset is part of the established split-base view. */
    work = (u8 *)slot - 0x19E8;
    for (; slotIndex < 0x25; slotIndex++, slot++) {
        kind = Ovl11IsSpecialCounterSlot((s16)slotIndex);
        value = slot->unk4;
        count = (s16)slot->unk2;
        if (slot->unk1E & 0x400) {
            if (kind == one) {
                value += 0xC8;
            } else {
                count++;
            }
        }
        switch (slotIndex) {
        case 0x19:
        case 0x1D:
        case 0x1E:
        case 0x1F:
        case 0x20:
        case 0x23:
            break;
        default:
            if (!(slot->unk1E & 0x600) && Rand(10) == 0) {
                if (kind == one) {
                    value -= 0xC8;
                } else {
                    count--;
                }
            }
            break;
        }
        if (value > 0xFFFF) value = 0xFFFF;
        if (value < 0) value = 0;
        if (count > 0xFF) count = 0xFF;
        if (count < 0) count = 0;
        slot->unk4 = value;
        slot->unk2 = count;
        if (slotIndex == *(s16 *)(work + 0x6778)) {
            if (slot->unk1E & 0x200) {
                SetGameFlag(0x4B, 1);
            } else {
                SetGameFlag(0x4B, 0);
            }
        }
        slot->unk1E &= ~0x600;
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_9F68 >= 0x5DD) {
        SetGameFlag(0xE0, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_A6B8 >= 0x5DD) {
        SetGameFlag(0xE1, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_AA60 >= 0x5DD) {
        SetGameFlag(0xE2, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_B384 >= 0x5DD) {
        SetGameFlag(0xE3, 1);
    }
    if (((struct Ovl11RecordThresholds *)D_8006C838)->field_BE7C >= 0x5DD) {
        SetGameFlag(0xE4, 1);
    }
}

/*
 * Per-object event scripts: scheduled-entry selection, command dispatch,
 * 16.16 X/Z motion and tick/countdown processing, plus the 37-record driver.
 * ovl_11:800C1F94..800C3138, in original function order.
 *
 * The live-mode flag distinguishes ordinary execution from time-of-day
 * reconstruction. Reconstruction skips selected motion commands and advances
 * the shared script tick inside replay loops; it is not a wall/frame clock.
 * Public contracts and record/command units: ovl11_object_event_script.h.
 * Membership and boundary evidence: notes/file-groupings.md.
 */
/* Forward animation/command services. Distinct pointer views preserve each
 * established record field's width/signedness; they are not separate objects. */
void Ovl11SelectObjectEventAnimation(Ovl11AnimationObject *, s32, s32, s32);
s32 Ovl11TryStartCommandReaction(Ovl11AnimationObject *, u16 *);
void Ovl11ShowObjectCommandMessage(Ovl11C3F6CObjectView *, u16 *);
s32 Ovl11BeginObjectCommandRow(Ovl11C3B78Arg0 *, s32, s32);
void Ovl11SetObjectCommandMotion(Ovl11C3B78Arg0 *, Ovl11C3B78Arg1 *);
s32 Ovl11GetObjectCommandTableAddress(void);
s32 ovl_11_func_800DD1D0(s16 *);
void Ovl11TryDrawObjectEvent(Ovl11AnimationObject *);

/* Forward entry used by the tick loop; external linkage is retained. */
s32 Ovl11UpdateObjectCommand(Ovl11C3B78Arg0 *object);

void Ovl11ApplyEventAnimationCommand(Ovl11AnimationObject *object, u16 *commandWords) {
    u16 command;

    command = *commandWords;
    switch (command) {
    case 1:
    case 16:
        if (*(s16 *) ((u8 *) object + 0x10) != 0) {
            Ovl11SelectObjectEventAnimation(object, 2, 0, -2);
            return;
        }
        Ovl11SelectObjectEventAnimation(object, 2, object->field_6 + 1, -2);
        return;
    case 0:
        *(s32 *) ((u8 *) object + 0x50) = 0;
        *(s32 *) ((u8 *) object + 0x54) = 0;
        *(s32 *) ((u8 *) object + 0x58) = 0;
        Ovl11SelectObjectEventAnimation(object, 1, 0, -2);
        return;
    }
}

/* Preserve each access's width/signedness across the established partial
 * record views. These abbreviations are local to the command dispatcher. */
#define SCRIPT_H(off) (*(s16 *) ((u8 *) object + (off)))
#define SCRIPT_U(off) (*(u16 *) ((u8 *) object + (off)))
#define CURRENT_COMMAND() ((Ovl11C3B78Arg1 *) (scriptBase + object->unkE * 0xCC + object->unkC * 0xC))

void Ovl11AdvanceScriptCommand(Ovl11C3B78Arg0 *object, s32 scriptBase, Ovl11C3B78Arg1 *event) {
    u16 command;
    s32 index;
    s32 next;
    s32 z;
    u16 count;

    object->unk30 = event->unk6;
    count = (u16)object->unkC;
    z = event->unkA;
    next = count + 1;
    object->unkC = next;
    index = object->unkC;
    object->unk50 = 0;
    object->unk54 = 0;
    object->unk58 = 0;
    object->unk38 = z;
    if (index == 17) {
        object->unkC = 16;
        SCRIPT_H(0x1A) = 0;
        object->unk6 = event->unk4;
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *) object, 1, 0, -2);
        return;
    }
    event = (Ovl11C3B78Arg1 *) (scriptBase + object->unkE * 0xCC + index * 0xC);
    if (gOvl11ObjectScriptLiveMode == 0) {
        command = event->unk0;
        if (command == 3 || command == 9 || command == 17 || command == 18) {
            object->unkC++;
            event = CURRENT_COMMAND();
        }
    }
    switch (event->unk0) {
    case 0x8000:
        object->unkC = event->unk2;
        event = CURRENT_COMMAND();
        SCRIPT_U(0x1E) &= 0xBFFF;
        if (event->unk0 != 5) {
            if (event->unk0 != 2) {
                goto process;
            }
            goto set_position;
        } else {
            SCRIPT_U(0x1E) |= 0x4000;
set_position:
            object->unk30 = event->unk6;
            object->unk34 = event->unk8;
            object->unk38 = event->unkA;
            object->unk40 = event->unk6 << 16;
            object->unk44 = event->unk8 << 16;
            object->unk48 = event->unkA << 16;
            object->unkC++;
            event = CURRENT_COMMAND();
            if (gOvl11ObjectScriptLiveMode != 0) {
                SCRIPT_H(0x10) = event->unk2;
            }
        }
        break;
    case 16:
        object->unkC++;
        event = CURRENT_COMMAND();
        object->unk30 = event->unk6;
        object->unk34 = event->unk8;
        object->unk38 = event->unkA;
        object->unk40 = event->unk6 << 16;
        object->unk44 = event->unk8 << 16;
        object->unk48 = event->unkA << 16;
        break;
    case 18:
        SCRIPT_U(0x20) |= 0x1000;
        /* Fall through: these commands share the motion/event path. */
    case 17:
    case 3:
        if (!(SCRIPT_U(0x1E) & 0x1000)) {
            Ovl11TryStartCommandReaction((Ovl11AnimationObject *) object, (u16 *) event);
            return;
        }
        object->unkC++;
        event = CURRENT_COMMAND();
        object->unk30 = event->unk6;
        object->unk34 = event->unk8;
        object->unk38 = event->unkA;
        object->unk40 = event->unk6 << 16;
        object->unk44 = event->unk8 << 16;
        object->unk48 = event->unkA << 16;
        if (gOvl11ObjectScriptLiveMode != 0) {
            SCRIPT_H(0x10) = event->unk2;
        }
        break;
    case 9:
        Ovl11ShowObjectCommandMessage((Ovl11C3F6CObjectView *) object, (u16 *) event);
        return;
    case 4:
        ((Ovl11EventObject *)object)->entryIndex++;
        Ovl11LoadScheduledEvent((Ovl11EventObject *) object);
        SCRIPT_U(0x2C) = ((Ovl11EventObject *)object)->entries[((Ovl11EventObject *)object)->entryIndex].data[1];
        SCRIPT_U(0x2E) = ((Ovl11EventObject *)object)->entries[((Ovl11EventObject *)object)->entryIndex].data[2];
        ((Ovl11EventObject *)object)->entries[((Ovl11EventObject *)object)->entryIndex].time = ((struct Ovl11CommandTimeState *) D_8006C838)->time;
        if (SCRIPT_H(0x2E) != ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1]) {
            SCRIPT_U(0x20) &= 0xDFFF;
            SCRIPT_U(0x1E) &= 0xFEFF;
            ((Ovl11EventObject *)object)->entries[((Ovl11EventObject *)object)->entryIndex].time -= SCRIPT_U(0xA);
            if (D_80125F24[SCRIPT_H(0x2E)] != 0) {
                SCRIPT_H(0xA) = 0;
            }
            return;
        }
        ((Ovl11EventObject *)object)->entries[((Ovl11EventObject *)object)->entryIndex].time = ((struct Ovl11CommandTimeState *) D_8006C838)->time;
        Ovl11BeginObjectCommandRow(object, gOvl11ObjectScriptLiveMode, 0);
        if (gOvl11ObjectScriptLiveMode != 0) {
            SCRIPT_H(0x10) = event->unk2;
        }
        if (SCRIPT_H(0x10) != 0) {
            Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *) object, 2, SCRIPT_H(6), -2);
        }
        break;
    case 1:
    case 6:
        if (gOvl11ObjectScriptLiveMode != 0) {
            SCRIPT_H(0x10) = event->unk2;
        }
        break;
    case 0:
    case 0xFFFF:
        object->unk6 = ((u16 *)event)[-4];
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *) object, 1, 0, -2);
        return;
    }
process:
    if (SCRIPT_H(0x10) == 0) {
        Ovl11SetObjectCommandMotion(object, event);
        Ovl11DispatchCurrentScriptCommand(object);
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *) object, 2, SCRIPT_H(6) + 1, -2);
    }
    Ovl11ApplyEventAnimationCommand((Ovl11AnimationObject *) object, (u16 *) event);
}

#undef SCRIPT_H
#undef SCRIPT_U
#undef CURRENT_COMMAND

void Ovl11MoveTowardScriptCommand(Ovl11C3B78Arg0 *object) {
    s32 fixedX;
    s32 fixedZ;
    s32 scriptBase;
    s32 distance;
    s32 deltaX;
    s32 deltaZ;
    u16 command;
    Ovl11C3B78Arg1 *event;

    scriptBase = Ovl11GetObjectCommandTableAddress();
    fixedX = *(s32 *)((u8 *)object + 0x40) + object->unk50;
    *(s32 *)((u8 *)object + 0x40) = fixedX;
    fixedZ = *(s32 *)((u8 *)object + 0x48) + object->unk58;
    *(s32 *)((u8 *)object + 0x48) = fixedZ;
    event = (Ovl11C3B78Arg1 *)(scriptBase +
        (*(s16 *)((u8 *)object + 0xE) * 0xCC) + (object->unkC * 0xC));
    /* Division, not a signed shift: negative positions truncate toward zero. */
    object->unk30 = fixedX / 0x10000;
    object->unk38 = fixedZ / 0x10000;
    if ((*(u16 *)((u8 *)object + 0x1E) & 0x100) && !(gOvl11ObjectScriptTick & 1)) {
        Ovl11ResolveSurfaceHeight(&object->unk30, 0xC000);
    }
    command = event->unk0;
    switch (command) {
    case 3:
    case 0x11:
    case 0x12:
        if ((*(u16 *)((u8 *)object + 0x1E) & 2) &&
            (*(u16 *)((u8 *)object + 0x1A) == 5) &&
            (Ovl11GetSpriteRotationState((s16)event->unk2) == 3)) {
            Ovl11AdvanceScriptCommand(object, scriptBase, event);
            *(s16 *)((u8 *)object + 0x12) = 0x14;
            *(u16 *)((u8 *)object + 0x1E) &= 0xFFFC;
        }
        break;
    default:
        distance = 0x40;
        if (D_80125F24[*(s16 *)((u8 *)object + 0x2E)] == 0x1000) {
            distance = 0x10;
        }
        if (*(u16 *)((u8 *)object + 0x1E) & 0x4000) {
            distance = 0x10;
        }
        deltaX = abs(event->unk6 - object->unk30);
        if (deltaX < distance) {
            deltaZ = abs(event->unkA - object->unk38);
            if (deltaZ < distance) {
                Ovl11AdvanceScriptCommand(object, scriptBase, event);
            }
        }
    }
}

s32 Ovl11FindCountedWordIndex(s16 key, u32 *table) {
    u32 count;
    u32 index;

    count = *table;
    table += 1;
    index = 0;
    if (count != 0) {
        do {
            if (key == *table) {
                return index;
            }
            index += 1;
            table += 1;
        } while (index < count);
    }
    return -1;
}

s32 Ovl11SelectScriptSpriteSource(s16 key, SpriteSourceData *source) {
    s32 group;
    s32 day;
    s32 first;
    s32 index;
    u32 *table;
    u32 *fallback;

    table = (u32 *) &D_8009CBF8;
    fallback = D_8008F7F8_words;
    group = ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[6];
    index = Ovl11FindCountedWordIndex(key, table);
    first = index;
    if (index == -1) {
        index = Ovl11FindCountedWordIndex(key, fallback);
        if (index != first) {
            group = (s16) ((s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->hour / 6);
            if (group >= 3) {
                group = 2;
            }
            table = fallback;
            day = (s16) ((struct Ovl11SpriteSelectors *) D_8006C838)->day;
            group += day * 3 - (day / 7) * 21;
            group += 14;
        } else {
            return 0;
        }
    }
    if (group == -1) {
        while (1) {
        }
    }
    gOvl11ScriptSpriteGroup = group;
    gOvl11ScriptSpriteItemIndex1 = index + 1;
    if (group == 0x3E6) {
        group = ((struct Ovl11SpriteSelectors *) D_8006C838)->season * 3 + 35;
        group += ((struct Ovl11SpriteSelectors *) D_8006C838)->variant;
    }
    InitSpriteSourceData(source, (SpriteDataHeader *) ((u8 *) table + D_80125DC0[group][index + 1]));
    return 1;
}

typedef struct __attribute__((packed)) {
    s32 value;
} Ovl11UnalignedEventWord;

/* This four-byte descriptor copy is byte-aligned (swl/swr); the surrounding
 * halfword fields retain natural alignment. Do not silently widen the view. */
void Ovl11LoadScheduledEvent(Ovl11EventObject *object) {
    u16 previous;
    u16 selected;
    u8 *entryBase;
    u8 *entryBase2;

    entryBase = (u8 *)object + ((u32)object->entryIndex * 8);
    *(Ovl11UnalignedEventWord *)((u8 *)object + 0x2C) = *(Ovl11UnalignedEventWord *)(entryBase + 0xE8);
    entryBase2 = (u8 *)object + ((u32)object->entryIndex * 8);
    selected = (u16)(*(u16 *)(entryBase2 + 0xE6) >> 8);
    if (object->field14 != (s16)selected) {
        object->flags &= 0xFFEF;
    }
    previous = (u16)object->field14;
    object->field14 = (s16)selected;
    object->field16 = previous;
}

void Ovl11AdvanceScheduledEvent(Ovl11EventObject *object) {
    s16 oldEvent;
    s32 time;
    s32 nextTime;
    s32 ready;
    s32 changed;
    u16 flags;

    if (((Ovl11C2B04GlobalState *) D_8006C838)->flags & 0x100000) {
        if (object->entryIndex != 0x1D) {
            time = ((Ovl11C2B04GlobalState *) D_8006C838)->time;
            nextTime = object->entries[object->entryIndex + 1].time + *(s16 *) ((u8 *) object + 0xA);
            ready = 0;
            /* Schedule times before 06:00 belong to the following day. */
            if (nextTime < 0x168) {
                time -= 0x5A0;
            }
            if (time >= nextTime) {
                ready = 1;
            }
            if (ready != 0) {
                oldEvent = *(s16 *) ((u8 *) object + 0x2E);
                changed = 0;
                if (object->flags & 0x100) {
                    object->flags20 |= 0x2000;
                } else {
                    object->flags20 &= 0xDFFF;
                }
                object->entryIndex++;
                Ovl11LoadScheduledEvent(object);
                if (oldEvent != *(s16 *) ((u8 *) object + 0x2E)) {
                    changed = 1;
                }
                if (ovl_11_func_800DD1D0((s16 *) ((u8 *) object + 0x2C)) == 0) {
                    if (!(object->flags & 0x100)) {
                        if (Ovl11SelectScriptSpriteSource(*(s16 *) object, (SpriteSourceData *) ((u8 *) object + 0x80)) != 0) {
                            *(s16 *) ((u8 *) object + 0x10) = 0;
                            goto advance;
                        }
                    } else {
advance:
                        flags = object->flags;
                        object->flags20 &= 0xEFFF;
                        if (!(flags & 0x1000) && changed != 0) {
                            object->flags = flags | 0x100;
                        }
                        object->field12 = 0x14;
                        Ovl11BeginObjectCommandRow((Ovl11C3B78Arg0 *)object, gOvl11ObjectScriptLiveMode, 0);
                    }
                }
            }
        }
    }
}

void Ovl11DispatchCurrentScriptCommand(Ovl11C3B78Arg0 *object) {
    Ovl11C3B78Arg1 *event;

    event = (Ovl11C3B78Arg1 *)(Ovl11GetObjectCommandTableAddress() +
        (*(s16 *)((u8 *)object + 0xE) * 0xCC) + (object->unkC * 0xC));
    Ovl11SetObjectCommandMotion(object, event);
    Ovl11ApplyEventAnimationCommand((Ovl11AnimationObject *)object, &event->unk0);
}

void Ovl11RunObjectScriptTicks(Ovl11C3B78Arg0 *object, s32 tickCount) {
    s32 position[3];
    void *hitObject;
    s32 hitFlags;
    s32 probeEnabled;
    s32 tick;
    u16 flags;

    if ((*(s16 *) ((u8 *) object + 0x12)) != 0) {
        (*(s16 *) ((u8 *) object + 0x12)) = (s16) ((u16) (*(s16 *) ((u8 *) object + 0x12)) - 1);
    }
    flags = *(u16 *) ((u8 *) object + 0x1E);
    if (flags & 0x100) {
        probeEnabled = 1;
        if (flags & 0x1000) {
            position[0] = *(s32 *) ((u8 *) object + 0x60);
            position[1] = *(s32 *) ((u8 *) object + 0x64);
            position[2] = *(s32 *) ((u8 *) object + 0x68);
        } else {
            position[0] = object->unk30;
            position[1] = object->unk34;
            position[2] = object->unk38;
            if (flags & 1) {
                probeEnabled = 0;
            }
            if ((*(s16 *) ((u8 *) object + 0xA)) >= 0x1E) {
                probeEnabled = 0;
            }
            if ((*(u16 *) ((u8 *) object + 0x20)) & 0x1000) {
                probeEnabled = 0;
            }
        }
        if ((*(u16 *) ((u8 *) object + 0x1E)) & 1) {
            probeEnabled = 0;
        }
        if (D_80071A3C == 4) {
            probeEnabled = 0;
        }
        if ((probeEnabled != 0) && (Ovl11QueryCollisionBudgeted(position, *(Ovl11CollisionTag *)((u8 *)object + 0x2C), 0x3E8U, 0x3E8U, &hitObject, &hitFlags, 0, 1) != 0) && ((*(s16 *) ((u8 *) object + 0xA)) < 0x1E)) {
            (*(u16 *) ((u8 *) object + 0x1E)) = (u16) ((*(u16 *) ((u8 *) object + 0x1E)) | 4);
        }
    }
    if ((((*(u16 *) ((u8 *) object + 0x1A)) == 3) || ((*(u16 *) ((u8 *) object + 0x1E)) & 4)) && (D_8006C844 & 0x100000)) {
        if (++*(s16 *)((u8 *)object + 0xA) >= 0x1E) {
            (*(u16 *) ((u8 *) object + 0x1E)) = (u16) ((*(u16 *) ((u8 *) object + 0x1E)) & 0xFFFB);
            Ovl11UpdateObjectCommand(object);
        }
    }
    for (tick = 0; tick < tickCount; tick++) {
        Ovl11TickTimedSpriteEffect((Ov11Timer *) ((u8 *) object + 0xE0));
        /* These sentinel tests are intentionally separate: -2 runs the
         * command-state path, -3 freezes the countdown. */
        if (*(s16 *)((u8 *)object + 8) != -2) {
            if (*(s16 *)((u8 *)object + 8) != -3) {
                if (*(s16 *)((u8 *)object + 8) != -2) {
                    --*(s16 *)((u8 *)object + 8);
                }
            }
            if (*(s16 *)((u8 *)object + 8) == -1) {
                Ovl11DispatchCurrentScriptCommand(object);
            }
        } else {
            Ovl11UpdateObjectCommand(object);
            if (!(*(u16 *)((u8 *)object + 0x1E) & 0x100)) {
                break;
            }
        }
        if (gOvl11ObjectScriptLiveMode == 0) {
            gOvl11ObjectScriptTick += 1;
        }
    }
}

void Ovl11StepObjectEvent(Ovl11C3B78Arg0 *object) {
    u16 flags;
    u16 activeFlags;

    flags = *(u16 *) ((u8 *) object + 0x1E);
    if (flags & 0x8000) {
        (*(u16 *) ((u8 *) object + 0x1E)) = (u16) (flags & 0x7FFF);
    }
    if (!(D_8006C844 & 0x08000000)) {
        if (GetGameFlag(3U) == 0) {
            Ovl11AdvanceScheduledEvent((Ovl11EventObject *)object);
        }
        if (IsScreenFadeComplete() != 0) {
            activeFlags = *(u16 *) ((u8 *) object + 0x1E);
            if (activeFlags & 0x100) {
                if (!(activeFlags & 0x20)) {
                    if (GetGameFlag(3U) == 0) {
                        Ovl11RunObjectScriptTicks(object, 1);
                    }
                }
            }
        }
    }
}

s32 Ovl11UpdateObjectCommand(Ovl11C3B78Arg0 *object) {
    switch (*(u16 *)((u8 *)object + 0x1A)) {
    case 2:
    case 5:
    case 6:
        if (!(*(u16 *)((u8 *)object + 0x1E) & 4)) {
            if (*(s16 *)((u8 *)object + 0x10) == 0) {
                Ovl11MoveTowardScriptCommand(object);
            } else {
                if (--*(s16 *)((u8 *)object + 0x10) == 0) {
                    Ovl11DispatchCurrentScriptCommand(object);
                }
            }
        }
        break;
    }
    return 1;
}

void Ovl11UpdateObjectEvents(void) {
    s32 index;
    struct_80076220 *object;

    object = &gObjectEventSlotBase;
    gOvl11ObjectScriptTick++;
    if (!(D_8006C848 & 0x02000000)) {
        for (index = 0; index < 37; index++) {
            Ovl11StepObjectEvent((Ovl11C3B78Arg0 *) object);
            if (!(object->unk1E & 0x1000)) {
                if (object->unk1E & 0x100) {
                    Ovl11TryDrawObjectEvent((Ovl11AnimationObject *) object);
                }
            }
            object++;
        }
    }
}

s32 Ovl11TryStartObjectReaction(Ovl11AnimationObject *obj);

s32 Ovl11TriggerNearbyObjectReactions(u8 *actor) {
    s32 biasedDeltaX;
    s32 biasedDeltaZ;
    s32 angleToActor;
    s32 actorHeading;
    s32 slotIndex;
    struct_80076220 *slot;
    u16 flags;

    slot = &gObjectEventSlotBase;
    /* Retained original store: this sentinel is never consulted by the scan.
     * All eligible slots may react; this is not a nearest-object search. */
    gOvl11ReactionSearchSentinel = 0x7FFFFFFF;
    for (slotIndex = 0; slotIndex < 35; slotIndex++) {
        flags = slot->unk1E;
        if ((flags & 0x100) && (*(s16 *)(actor + 0xFE) == (s16)slot->unk2E) &&
            ((*(s16 *)((u8 *)slot + 0x12)) == 0) && ((*(u16 *)((u8 *)slot + 0x1A)) == 2) &&
            !((*(u16 *)((u8 *)slot + 0x20)) & 0x1000) && !(flags & 0x1003) && ((s16)slot->unkA < 0x1E)) {
            biasedDeltaX = (*(s32 *)(actor + 0x100) - (*(s32 *)((u8 *)slot + 0x30))) + 1;
            biasedDeltaZ = (*(s32 *)(actor + 0x108) - (*(s32 *)((u8 *)slot + 0x38))) + 1;
            gOvl11ReactionDistanceSquared = biasedDeltaX * biasedDeltaX;
            gOvl11ReactionDistanceSquared += biasedDeltaZ * biasedDeltaZ;
            if (gOvl11ReactionDistanceSquared <= 0xF423FF) {
                angleToActor = ratan2(biasedDeltaX * 0xA, biasedDeltaZ * 0xA);
                actorHeading = gOvl11ReactionHeadingAngles[*(u16 *)(actor + 0x38)];
                if (((angleToActor - 0x258) < actorHeading) && (actorHeading < (angleToActor + 0x258))) {
                    Ovl11TryStartObjectReaction((Ovl11AnimationObject *)slot);
                }
            }
        }
        slot += 1;
    }
    return 0;
}

#define STATE ((struct Ovl11AnimationState *) D_8006C838)

void Ovl11SelectObjectEventAnimation(Ovl11AnimationObject *obj, s32 eventState, s32 animation, s32 countdown) {
    ObjectState *initial = (ObjectState *) &obj->sprite;
    SpriteSourceData *sprite;
    u16 hour;
    s16 allowed;
    u16 previous;
    s16 id;
    u16 *flags = &obj->flags;

    previous = obj->field_1A;
    obj->field_1A = eventState;
    obj->field_8 = countdown;
    *flags |= 0x80;
    obj->field_1C = previous;
    SelectSpriteAnimation(initial, animation);
    if (STATE->selected == obj->id) {
        if (GetGameFlag(0x4F) == 1 && STATE->mode == 0) {
            hour = (u16) STATE->hour;
            allowed = (hour < 5 || hour >= 12);
            if (hour >= 14 && hour < 16) {
                allowed = 0;
            }
            if (allowed && animation < 5) {
                SelectSpriteAnimation(initial, animation + 5);
            }
        }
        if (STATE->hour >= 16) {
            if (STATE->field_44CE == 2) {
                SetGameFlag(0x72, 1);
            }
            SelectSpriteAnimation((ObjectState *) &obj->sprite, 10);
            return;
        }
    }
    id = obj->id;
    sprite = &obj->sprite;
    if (id == 0x24) {
        if (GetGameFlag(0x4F) == 1) {
            if (STATE->mode == 0) {
                s32 value;

                obj->field_6 = 0;
                value = Rand(2);
                if (value != 0) {
                    SelectSpriteAnimation((ObjectState *) sprite, 2);
                } else {
                    SelectSpriteAnimation((ObjectState *) sprite, 4);
                }
                if (STATE->hour >= 14) {
                    SelectSpriteAnimation((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            } else {
                SelectSpriteAnimation((ObjectState *) sprite, (u8) obj->field_6 + 10);
                if (STATE->hour >= 12) {
                    SelectSpriteAnimation((ObjectState *) sprite, 5);
                    obj->field_6 = 0;
                }
            }
        }
    }
    if (obj->id == 0x18 && ((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 1 && obj->field_E == 4) {
        SelectSpriteAnimation((ObjectState *) sprite, (u8) obj->field_6 + 5);
    }
    if (GetSpriteFrameControlHighBit(sprite) != 0) {
        obj->sprite.field_5 = *(u8 *) &obj->field_6;
    }
    if (obj->id == 0x17) {
        SelectSpriteAnimation((ObjectState *) sprite, 1);
    }
}

#undef STATE

s32 Ovl11IsSpecialCounterSlot(s32 slotIndex) {
    s32 relativeIndex;
    s32 special;

    relativeIndex = (s32)((slotIndex << 0x10) + 0xFFFD0000) >> 0x10;
    special = 0;
    switch (relativeIndex) {
    case 0:
    case 4:
    case 6:
    case 11:
    case 17:
        special = 1;
        break;
    }
    return special;
}

void Ovl11DrawObjectEvent(Ovl11AnimationObject *arg0);

void Ovl11TryDrawObjectEvent(Ovl11AnimationObject *object) {
    if (*(s16 *)((u8 *)object + 0x2E) ==
            ((struct Ovl11D2764StateView *)&D_8007AFF0)->field_25476 &&
        !(D_8006C844 & 0x800000)) {
        Ovl11DrawObjectEvent(object);
    }
}

void Ovl11SelectObjectEventAnimation(Ovl11AnimationObject *obj, s32 arg1, s32 arg2, s32 arg3);

s32 Ovl11TryStartObjectReaction(Ovl11AnimationObject *obj) {
    s32 threshold;

    if (obj->field_1A == 3) {
        return 0;
    }
    if (obj->flags & 1) {
        return 0;
    }
    if (obj->flags & 0x1000) {
        return 0;
    }
    if (!(obj->flags & 0x100)) {
        return 0;
    }
    if (*(s16 *) ((u8 *) obj + 0xA) < 30) {
        threshold = *(s16 *) ((u8 *) obj + 2) - *(s16 *) ((u8 *) obj + 0xA) + 20;
        if ((s32) Rand(101) < threshold) {
            Ovl11SelectObjectEventAnimation(obj, 3, 0, 150);
            Ovl11StartTimedSpriteEffect((Ovl11C35ECStatePair *) ((u8 *) obj + 0xE0), 0x22, 150);
            obj->field_6 = (gPrimaryObjectFacing + 2) & 3;
            obj->sprite.field_5 = *(u8 *) &obj->field_6;
            return 1;
        }
    }
    return 0;
}

void func_80016054(s32 arg0, s32 arg1, s32 arg2, s32 arg3, u8 arg4,
                    s16 arg5, s16 arg6, s32 arg7, s32 arg8, u16 arg9);
void ovl_11_func_800C9E90(s32 arg0, s32 arg1, s32 arg2, s32 arg3);

void Ovl11DrawObjectEvent(Ovl11AnimationObject *arg0) {
    u16 pos[3];
    s32 screen[4];
    s32 ot;

    pos[0] = *(u16 *)((u8 *)arg0 + 0x30) + 0x96;
    pos[1] = *(u16 *)((u8 *)arg0 + 0x34) - 0x64;
    pos[2] = *(u16 *)((u8 *)arg0 + 0x38) - 0x96;
    if (Ovl11ProjectRecordSprite(pos, screen) != 0) {
        ot = gActiveRenderContext->field_120 + ((screen[2] >> 2) * 4);
        if (arg0->id == ((struct Ovl11FlagDateView *)D_8006C838)->field_E778) {
            if (GetGameFlag(0x72) != 0) {
                ot -= 0x14;
            }
        }
        Ovl11DrawTimedSpriteEffect((u8 *)arg0 + 0xE0, ot, (s16)screen[0],
                              (s16)(screen[1] - 0x28));
        func_80016054(ot, gActiveRenderContext->field_118, (s32)&arg0->sprite,
                       arg0->sprite.field_4, arg0->sprite.field_5,
                       (s16)screen[0], (s16)screen[1], arg0->sprite.field_8,
                       0, 0x40);
        ovl_11_func_800C9E90(ot, screen[0], screen[1], 0);
    }
    if (!(((struct struct_8006C838_view *)D_8006C838)->field_0C & 0x08000000)) {
        UpdateSpriteAnimation(&arg0->sprite);
    }
}

s32 Ovl11GetObjectCommandTableAddress(void) {
    return D_8007AFF0 + 0x1000;
}

void Ovl11SetObjectCommandMotion(Ovl11C3B78Arg0 *arg0, Ovl11C3B78Arg1 *arg1);
s32 Ovl11TryStartCommandReaction(Ovl11AnimationObject *arg0, u16 *arg1);
void Ovl11ShowObjectCommandMessage(Ovl11C3F6CObjectView *arg0, u16 *arg1);

/* restartMode==0 permits same-row reuse and starts at command 1 after the
 * position descriptor. Nonzero skips reuse while retaining the command index.
 * Reaction dispatch is independently suppressed during time reconstruction. */
s32 Ovl11BeginObjectCommandRow(Ovl11C3B78Arg0 *object, s32 allowReaction, s32 restartMode) {
    s32 scheduleOffset;
    s32 tableAddress;
    s32 initializePosition;
    s32 result;
    u16 flags;
    u16 scheduleIndex;
    u16 firstCommandId;
    u16 commandId;
    u8 rowIndex;
    Ovl11C3B78Arg1 *command;
    void *schedule;

    tableAddress = Ovl11GetObjectCommandTableAddress();
    if (restartMode == 0) {
        scheduleIndex = *(u16 *)((u8 *)object + 0x22);
        scheduleOffset = scheduleIndex * 8;
        if (scheduleIndex != 0) {
            if (((s16)((Ovl11EventObject *)object)->entries[scheduleIndex].data[2] ==
                 (s16)((Ovl11EventObject *)object)->entries[scheduleIndex - 1].data[2]) &&
                (*((u8 *)object + scheduleOffset + 0xE6) == *(s16 *)((u8 *)object + 0xE)) &&
                (*(u16 *)((u8 *)object + 0x1E) & 0x100)) {
                return 1;
            }
        }
    }
    schedule = (u8 *)object + (*(u16 *)((u8 *)object + 0x22) * 8);
    rowIndex = *((u8 *)schedule + 0xE6);
    command = (Ovl11C3B78Arg1 *)(tableAddress + (rowIndex * 0xCC));
    *(s16 *)((u8 *)object + 0xE) = (s16)rowIndex;
    object->unk6 = command->unk4;
    initializePosition = 0;
    if ((command->unk0 != 7) || !(*(u16 *)((u8 *)object + 0x20) & 0x2000)) {
        initializePosition = 1;
    }
    if (initializePosition != 0) {
        object->unk30 = command->unk6;
        object->unk34 = command->unk8;
        object->unk38 = command->unkA;
        *(s32 *)((u8 *)object + 0x40) = command->unk6 << 16;
        *(s32 *)((u8 *)object + 0x44) = command->unk8 << 16;
        *(s32 *)((u8 *)object + 0x48) = command->unkA << 16;
        if (restartMode == 0) {
            object->unkC = 1;
        }
    }
    if (command->unk0 == 7) {
        object->unkC = 0;
        Ovl11MoveTowardScriptCommand(object);
    }
    Ovl11SetObjectCommandMotion(object, (Ovl11C3B78Arg1 *)((u8 *)command + (object->unkC * 0xC)));
    Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *)object, 1, 0, -2);
    flags = *(u16 *)((u8 *)object + 0x1E) & 0xBFFF;
    *(u16 *)((u8 *)object + 0x1E) = flags;
    firstCommandId = command->unk0;
    if (firstCommandId == 5) {
        *(u16 *)((u8 *)object + 0x1E) = flags | 0x4000;
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *)object, 2, (s16)object->unk6 + 1, -2);
        if (gOvl11ObjectScriptLiveMode != 0) {
            *(u16 *)((u8 *)object + 0x10) = command->unk2;
        }
        return 1;
    }
    command++;
    if (firstCommandId == 7) {
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *)object, 2, (s16)object->unk6 + 1, -2);
        return 0;
    }
    commandId = command->unk0;
    if (commandId == 3 || commandId == 17 || commandId == 18) {
        if (command->unk0 == 0x12) {
            *(u16 *)((u8 *)object + 0x20) |= 0x1000;
        }
        object->unk6 = command->unk4;
        if ((allowReaction != 0) && !(*(u16 *)((u8 *)object + 0x1E) & 0x1000)) {
            Ovl11TryStartCommandReaction((Ovl11AnimationObject *)object, (u16 *)command);
            return 0;
        }
        command++;
        object->unkC = (u16)object->unkC + 1;
        Ovl11SetObjectCommandMotion(object, command);
        commandId = command->unk0;
    }
    if (commandId == 9) {
        Ovl11ShowObjectCommandMessage((Ovl11C3F6CObjectView *)object, (u16 *)command);
        command++;
        object->unkC = (u16)object->unkC + 1;
        Ovl11SetObjectCommandMotion(object, command);
    }
    result = 0;
    if (command->unk0 == 1) {
        Ovl11SelectObjectEventAnimation((Ovl11AnimationObject *)object, 2, (s16)object->unk6 + 1, -2);
        result = 0;
    }
    return result;
}

/* Shared emitter/command views are in game_types.h. Motion angles use the
 * SDK's 4096-unit turn; command -8 and +4 retain their original u16 views. */
typedef struct {
    u8 pad0[8];
    u16 unk8;
} Ovl11C3B78View;

void Ovl11SetObjectCommandMotion(Ovl11C3B78Arg0 *object, Ovl11C3B78Arg1 *command) {
    u16 commandId;
    s32 biasedDeltaX;
    s32 biasedDeltaZ;
    s32 motionScale;
    s32 heading;
    char *farBase;

    commandId = command->unk0;
    if ((commandId == 3) || (commandId == 9) || (commandId == 0x11) || (commandId == 0x12)) {
        object->unk50 = 0;
        object->unk54 = 0;
        object->unk58 = 0;
        return;
    }
    if (object->unkC == 0) {
        object->unk6 = command->unk4;
    } else {
        /* Direction comes from the preceding command's +4 halfword. */
        object->unk6 = *(u16 *)((u8 *)command - 8);
    }
    biasedDeltaX = ((object->unk30 - command->unk6) * 0x3E8) + 1;
    biasedDeltaZ = ((object->unk38 - command->unkA) * 0x3E8) + 1;
    if ((biasedDeltaX == 1) && (biasedDeltaZ == biasedDeltaX)) {
        object->unk50 = 0;
        object->unk54 = 0;
        object->unk58 = 0;
    } else {
        heading = ratan2(biasedDeltaX, biasedDeltaZ) + 0x800;
        motionScale = gOvl11CommandMotionScale;
        farBase = (char *)&D_8007AFF0;
        if ((*(Ovl11C3B78View **)(farBase + 0x25388))->unk8 & 0x1000) {
            motionScale = gOvl11ReducedCommandMotionScale;
        }
        object->unk50 = motionScale * rsin(heading);
        object->unk58 = motionScale * rcos(heading);
    }
}

void Ovl11CountDownObjectEventCounters(void) {
    char *farBase = (char *)&D_8007AFF0;
    char *state;
    struct_80076220 *slot;
    s32 *farObject;
    s32 slotIndex;
    s32 nextCounter;

    farObject = *(s32 **)(farBase + 0x25388);
    if (*(s16 *)farObject == 0x13) {
        state = (char *)&D_8006C838;
        if (*(s16 *)(state + 0x5238) < 0xF) {
            *(u16 *)(state + 0x5238) = *(u16 *)(state + 0x5238) + 1;
        }
        slot = &gObjectEventSlotBase;
        for (slotIndex = 0; slotIndex < 37; slotIndex++) {
            slot->unk2 = slot->unk2 - 1;
            if ((s16)slot->unk2 < 0) {
                slot->unk2 = 0;
            }
            nextCounter = slot->unk4 - 1;
            if (nextCounter < 0) {
                slot->unk4 = 0;
            } else {
                slot->unk4 = nextCounter;
            }
            slot++;
        }
    }
}

void Ovl11TriggerTimedObjectEvents(void) {
    s16 temp_a1;

    if (!((u16)((struct Ovl11SpriteWork *)&D_8007AFF0)->header[4] & 0x1000) && (temp_a1 = ((struct Ovl11SpriteWork *)&D_8007AFF0)->header[1], (D_80125F24[temp_a1] != 0x1000))) {
        if (temp_a1 == 1) {
            if ((D_80070D02 == 0x438) && (GetGameFlag(0x40U) == 0)) {
                Ovl11LoadRenderContextSpriteBank();
                SetGameFlag(0x40U, 1U);
            }
        } else {
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x2D0) {
                Ovl11ClearEventSlotTimeOrigins();
                if (GetGameFlag(0x3FU) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        Ovl11LoadCalendarSpriteSet(0xE, 1);
                        Ovl11RebuildObjectEventsForTime();
                    }
                    SetGameFlag(0x3FU, 1U);
                }
            }
            if (((struct Ovl11CCCC0State *)D_8006C838)->angle == 0x438) {
                Ovl11ClearEventSlotTimeOrigins();
                if (GetGameFlag(0x40U) == 0) {
                    if (!(((struct Ovl11CCCC0State *)D_8006C838)->flags & 0x10)) {
                        Ovl11LoadCalendarSpriteSet(0xE, 1);
                        Ovl11RebuildObjectEventsForTime();
                    }
                    SetGameFlag(0x40U, 1U);
                }
            }
        }
    }
}

s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);

s32 Ovl11TryStartCommandReaction(Ovl11AnimationObject *object, u16 *commandWords) {
    u16 flags;
    s32 result;

    flags = object->flags;
    if (!(flags & 0x100)) {
        result = 1;
    } else if (!(flags & 2)) {
        ovl_11_func_80107C80(*(s16 *)((u8 *)commandWords + 2), (u16)object->field_6, NULL, 1);
        object->flags |= 3;
        Ovl11SelectObjectEventAnimation(object, 5, object->field_6 + 1, -2);
        result = 1;
    } else {
        result = 0;
    }
    return result;
}

s32 ovl_11_func_800F3D88(void);

void Ovl11ShowObjectCommandMessage(Ovl11C3F6CObjectView *object, u16 *commandWords) {
    s32 valueBeforeUpdate;
    u8 *base;

    /* Selection uses the pre-call value; the scene comparison uses state
     * after the updater. Do not move that call outside the guarded tail. */
    if (((*(s16 *)((u8 *)commandWords + 2)) == 0) && (GetGameFlag(0xA0U) != 1) &&
        (base = (u8 *)D_8006C838, valueBeforeUpdate = *(s32 *)(base + 0x49C0),
         ovl_11_func_800F3D88(), ((*(s16 *)(base + 0x52C6)) == object->unk2E))) {
        if (valueBeforeUpdate == 0) {
            RequestMessageWindow(1, 0x72B);
            return;
        }
        if (valueBeforeUpdate < 0x2710) {
            RequestMessageWindow(1, 0x72C);
            return;
        }
        RequestMessageWindow(1, 0x72D);
    }
}
