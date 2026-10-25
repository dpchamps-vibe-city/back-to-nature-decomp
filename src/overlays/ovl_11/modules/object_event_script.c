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
#include "common.h"
#include "game_types.h"
#include "psyq/abs.h"
#include "sprite_rotation.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_collision_query_budget.h"
#include "screen_fade.h"
#include "game_flags.h"
#include "ovl11_object_event_script.h"

/* Shared animation/command services outside this unit. The distinct pointer
 * views describe established fields of the same runtime record. */
void ovl_11_func_800C32C8(Ovl11AnimationObject *, s32, s32, s32);
s32 ovl_11_func_800C3EE8(Ovl11AnimationObject *, u16 *);
void ovl_11_func_800C3F6C(Ovl11C3F6CObjectView *, u16 *);
s32 ovl_11_func_800C3838(Ovl11C3B78Arg0 *, s32, s32);
void ovl_11_func_800C3B78(Ovl11C3B78Arg0 *, Ovl11C3B78Arg1 *);
s32 ovl_11_func_800C3828(void);
s32 ovl_11_func_800BDB88(s32 *, s32);
s32 ovl_11_func_800DD1D0(s16 *);
s32 ovl_11_func_80107DEC(Ov11Timer *);
void ovl_11_func_800C3590(Ovl11AnimationObject *);

/* Forward entry used by the tick loop; external linkage is retained. */
s32 Ovl11UpdateObjectCommand(Ovl11C3B78Arg0 *object);

void Ovl11ApplyEventAnimationCommand(Ovl11AnimationObject *object, u16 *commandWords) {
    u16 command;

    command = *commandWords;
    switch (command) {
    case 1:
    case 16:
        if (*(s16 *) ((u8 *) object + 0x10) != 0) {
            ovl_11_func_800C32C8(object, 2, 0, -2);
            return;
        }
        ovl_11_func_800C32C8(object, 2, object->field_6 + 1, -2);
        return;
    case 0:
        *(s32 *) ((u8 *) object + 0x50) = 0;
        *(s32 *) ((u8 *) object + 0x54) = 0;
        *(s32 *) ((u8 *) object + 0x58) = 0;
        ovl_11_func_800C32C8(object, 1, 0, -2);
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
        ovl_11_func_800C32C8((Ovl11AnimationObject *) object, 1, 0, -2);
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
            ovl_11_func_800C3EE8((Ovl11AnimationObject *) object, (u16 *) event);
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
        ovl_11_func_800C3F6C((Ovl11C3F6CObjectView *) object, (u16 *) event);
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
        ovl_11_func_800C3838(object, gOvl11ObjectScriptLiveMode, 0);
        if (gOvl11ObjectScriptLiveMode != 0) {
            SCRIPT_H(0x10) = event->unk2;
        }
        if (SCRIPT_H(0x10) != 0) {
            ovl_11_func_800C32C8((Ovl11AnimationObject *) object, 2, SCRIPT_H(6), -2);
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
        ovl_11_func_800C32C8((Ovl11AnimationObject *) object, 1, 0, -2);
        return;
    }
process:
    if (SCRIPT_H(0x10) == 0) {
        ovl_11_func_800C3B78(object, event);
        Ovl11DispatchCurrentScriptCommand(object);
        ovl_11_func_800C32C8((Ovl11AnimationObject *) object, 2, SCRIPT_H(6) + 1, -2);
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

    scriptBase = ovl_11_func_800C3828();
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
        ovl_11_func_800BDB88(&object->unk30, 0xC000);
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
                        ovl_11_func_800C3838((Ovl11C3B78Arg0 *)object, gOvl11ObjectScriptLiveMode, 0);
                    }
                }
            }
        }
    }
}

void Ovl11DispatchCurrentScriptCommand(Ovl11C3B78Arg0 *object) {
    Ovl11C3B78Arg1 *event;

    event = (Ovl11C3B78Arg1 *)(ovl_11_func_800C3828() +
        (*(s16 *)((u8 *)object + 0xE) * 0xCC) + (object->unkC * 0xC));
    ovl_11_func_800C3B78(object, event);
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
        ovl_11_func_80107DEC((Ov11Timer *) ((u8 *) object + 0xE0));
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
                    ovl_11_func_800C3590((Ovl11AnimationObject *) object);
                }
            }
            object++;
        }
    }
}
