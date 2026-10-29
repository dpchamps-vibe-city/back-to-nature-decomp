/*
 * pending_event_queue — reconstructed translation unit.
 *
 * Immediate and dated event queues, message progression and cancellation,
 * one-shot request evaluation and its exclusive completion helpers.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_pending_event_queue.h"
#include "game_types.h"
#include "message_window.h"
#include "psyq/memory.h"
#include "game_flags.h"
#include "ovl11_status_pages.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_progress_gate.h"
#include "ovl11_item_value_counters.h"

s32 Ovl11PopEventMessageEntry(void);
void Ovl11CompleteLatchedEventMessage(void);

void Ovl11UpdateEventQueueGate(void) {
    if (gOvl11EventMessageActive == 0) {
        gOvl11EventMessageLatched = 0;
    } else {
        if (GetMessageWindowStatus() == 2) {
            if (gOvl11EventMessageLatched != 0) {
                gOvl11EventMessageActive = 0;
                gOvl11EventMessageLatched = 0;
                Ovl11CompleteLatchedEventMessage();
            } else {
                if (Ovl11PopEventMessageEntry() == 0) {
                    gOvl11EventMessageActive = 0;
                }
            }
        } else if (((GfxObj *)gPad0Input)->field_8 & 0x20) {
            gOvl11EventMessageActive = 0;
            CloseMessageWindow();
        }
    }
}

s32 Ovl11TryShowQueuedEventMessage(void) {
    s32 headPresent;
    if (gOvl11EventMessageActive == 0) {
        headPresent = Ovl11PopEventMessageEntry();
        if (headPresent == 1) {
            gOvl11EventMessageActive = headPresent;
            return 1;
        } else {
            RequestMessageWindow(2, 0x221);
            return 0;
        }
    } else {
        return 0;
    }
}

void Ovl11ResetEventQueues(void) {
    s32 i;
    signed char *base;

    base = (signed char *)&D_8006C838;
    for (i = 0; i < 20; i++) {
        base[0x49E6 + 4 * i] = -1;
        base[0x4A36 + 4 * i] = -1;
    }
    ((Ovl11EventQueueWorkView *)&D_8006C838)->requestOnceFlags = 0;
}

void Ovl11QueueImmediateEvent(s16 kind, s16 value);

void Ovl11QueueDatedEvent(s16 kind, s16 value, s16 days) {
    s32 i;
    Ovl11QueuedEntry *p;
    u8 *tail;

    if (days == 0) {
        Ovl11QueueImmediateEvent(kind, value);
        return;
    }
    if (value != 0) {
        p = (Ovl11QueuedEntry *)gOvl11DatedEventQueue;
        for (i = 0; i < 20; i++, p++) {
            if (p->kind == -1) {
                p->kind = kind;
                p->value = value;
                p->day = days;
                break;
            }
        }
        if (i == 20) {
            tail = gOvl11DatedEventQueue;
            memmove(tail, tail + 4, 0x4C);
            tail -= 0x4A34;
            *(s8 *)(tail + 0x4A82) = kind;
            *(s16 *)(tail + 0x4A80) = value;
            *(u8 *)(tail + 0x4A83) = days;
        }
    }
}

void Ovl11RemoveQueuedEvent(s16 entryIndex, s32 immediateQueue);
void Ovl11EvaluateEventRequests(void);

/* Removal compacts the array. The dated scan backs up its cursor/index to
 * revisit the shifted entry; the immediate-day purge intentionally does not. */
void Ovl11ServiceEventQueues(void) {
    s16 day;
    s32 i;
    s8 kind;
    Ovl11QueuedEntry *p;
    u8 *base;

    base = (u8 *)D_8006C838;
    p = (Ovl11QueuedEntry *)(base + 0x49E4);
    i = 0;
    day = *(s16 *)(base + 0x44BC);
    do {
        if (p->kind != -1 && (s8)p->day == day) {
            Ovl11RemoveQueuedEvent(i, 1);
        }
        i++;
        p++;
    } while (i < 20);

    base = gOvl11ImmediateEventQueue;
    p = (Ovl11QueuedEntry *)(base + 0x50);
    for (i = 0; i < 20; i++, p++) {
        kind = p->kind;
        if (kind != -1) {
            if ((s8)--p->day <= 0) {
                Ovl11QueueImmediateEvent(kind, p->value);
                p--;
                Ovl11RemoveQueuedEvent(i, 0);
                i--;
            }
        }
    }
    gOvl11EventMessageActive = 0;
    Ovl11EvaluateEventRequests();
}

s32 Ovl11PopEventMessageEntry(void) {
    char *base;

    base = (char *)D_8006C838;
    if (*(s8 *)(base + 0x49E6) != -1) {
        CloseMessageWindow();
        if (RequestMessageWindow((s32)(*(s8 *)(base + 0x49E6)), (s32)(*(s16 *)(base + 0x49E4))) == 0) {
            if ((*(s16 *)(base + 0x49E4)) == 0x19B) {
                gOvl11EventMessageLatched = 1;
            }
            memmove((u8 *)(base + 0x49E4), (u8 *)(base + 0x49E8), 0x4C);
            (*(s8 *)(base + 0x4A32)) = -1;
        }
        return 1;
    }
    return 0;
}

void Ovl11QueueImmediateEvent(s16 kind, s16 value) {
    u8 *base;
    u8 *tail;
    Ovl11QueuedEntry *p;
    s16 day;
    s32 i;

    base = (u8 *)D_8006C838;
    p = (Ovl11QueuedEntry *)(base + 0x49E4);
    day = *(s16 *)(base + 0x44BC);
    for (i = 0; i < 20; i++, p++) {
        if (p->kind == -1) {
            p->kind = kind;
            p->value = value;
            p->day = day;
            break;
        }
    }
    if (i == 20) {
        tail = gOvl11ImmediateEventQueue;
        memmove(tail, tail + 4, 0x4C);
        tail -= 0x49E4;
        *(s8 *)(tail + 0x4A32) = kind;
        *(s16 *)(tail + 0x4A30) = value;
        *(u8 *)(tail + 0x4A33) = day;
    }
}

void Ovl11RemoveQueuedEvent(s16 entryIndex, s32 immediateQueue) {
    s32 index;
    s32 off;
    Ovl11QueuedEntry *p;
    Ovl11QueuedEntry *src;
    Ovl11QueuedEntry *q;
    s32 len;

    index = entryIndex;
    if (immediateQueue == 1) {
        u8 *base = gOvl11ImmediateEventQueue;
        u8 *mid;
        off = index * 4;
        p = (Ovl11QueuedEntry *)(off + (s32)base);
        mid = base + 4;
        src = (Ovl11QueuedEntry *)(off + (s32)mid);
        len = 0x50 - (index + 1) * 4;
        q = (Ovl11QueuedEntry *)(base + 0x4C);
    } else {
        u8 *base = gOvl11DatedEventQueue;
        u8 *mid;
        off = index * 4;
        p = (Ovl11QueuedEntry *)(off + (s32)base);
        mid = base + 4;
        src = (Ovl11QueuedEntry *)(off + (s32)mid);
        len = 0x50 - (index + 1) * 4;
        q = (Ovl11QueuedEntry *)(base + 0x4C);
    }
    if (index == 19) {
        p->kind = -1;
        p->value = 0;
        p->day = 0;
    } else {
        memmove((char *)p, (char *)src, len);
    }
    q->kind = -1;
    q->value = 0;
    q->day = 0;
}

s32 Ovl11GetEventEligibilityFlag(void);
void Ovl11QueueDatedEvent(s16, s16, s16);

void Ovl11EvaluateEventRequests(void) {
    u8 *base;
    Ovl11EventFlagView *events;
    s16 *var_a0;
    s16 temp_s3;
    s16 temp_s6;
    s16 var_s1;
    s32 temp_v0;
    s32 var_v1_2;
    Ovl11EventStateView *var_v1;
    void *temp_s0;
    void *temp_s0_2;
    void *temp_s0_3;
    Ovl11EventStateView *temp_v1;

    events = (Ovl11EventFlagView *)((u8 *)D_8006C838 + 0x44FC);
    temp_v1 = (Ovl11EventStateView *) ((u8 *) events - 0x44FC);
    temp_s3 = temp_v1->field_44B8.halves.high;
    temp_s6 = temp_v1->field_44BC;
    if ((~temp_s3 == -1) && (temp_s6 == 0)) {
        var_s1 = temp_s3;
        if (GetGameFlag(0xE6U) == 1) {
            var_s1 = events->field_690A >= 0x64;
            goto block_6;
        }
        if (events->field_690A >= 0xC8) {
            var_s1 = 2;
        block_6:
            if (((((Ovl11EventStateView *)D_8006C838)->field_AE06) >= 0xC8) && (GetGameFlag(0x22U) == 1)) {
                var_s1 = 3;
            }
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 1);
        }
        var_s1 = ((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0x64) * 4;
        if (((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0xC8) && (GetGameFlag(0x23U) == 1)) {
            var_s1 = 5;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 0);
        }
        var_s1 = 0;
        if ((((Ovl11EventStateView *)D_8006C838)->field_BCA6) >= 0x64) {
            var_s1 = 6;
            if (GetGameFlag(0x25U) == 1) {
                var_s1 = 7;
            }
        }
        if (GetGameFlag(0x20U) == 1) {
            var_s1 = 8;
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_BCA6) >= 0xC8) && (GetGameFlag(0x21U) == 1)) {
            var_s1 = 9;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 2);
        }
        var_s1 = 0;
        if ((((Ovl11EventStateView *)D_8006C838)->field_A30E) >= 0x64) {
            var_s1 = 0xA;
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_A30E) >= 0xC8) && (GetGameFlag(0x24U) == 1)) {
            var_s1 = 0xB;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 3);
        }
        do {
            var_s1 = 0;
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0x64) {
                var_s1 = 0xC;
            }
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0xC8) {
                temp_v0 = GetGameFlag(0x25U);
                if (temp_v0 == 1) {
                    if (Ovl11TestProgressGate(0) == temp_v0) {
                        var_s1 = 0xE;
                    } else if (GetGameFlag(0x11EU) == temp_v0) {
                        var_s1 = 0xD;
                    }
                }
            }
            if (var_s1 != 0) {
                Ovl11QueueDatedEvent(2, var_s1, 4);
            }
        } while (0);
        Ovl11QueueDatedEvent(2, 0xF, 1);
        base = (u8 *)D_8006C838;
        if ((((Ovl11EventStateView *)D_8006C838)->field_A13A) >= 0x96) {
            Ovl11QueueDatedEvent(2, 0x10, 0);
        }
        var_s1 = 0x11;
        if (GetGameFlag(0x1DU) == 1) {
            var_s1 = 0x12;
        }
        Ovl11QueueDatedEvent(2, var_s1, 2);
        Ovl11QueueDatedEvent(2, 0x13, 4);
        if ((((Ovl11EventStateView *)D_8006C838)->field_AFDA) >= 0x96) {
            Ovl11QueueDatedEvent(2, 0x14, 1);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_B72A) >= 0x96) {
            Ovl11QueueDatedEvent(2, 0x15, 0);
        }
        var_s1 = 0x16;
        if ((((Ovl11EventStateView *)D_8006C838)->field_B8FE) >= 0x96) {
            var_s1 = 0x17;
        }
        Ovl11QueueDatedEvent(2, var_s1, 0);
        if ((((Ovl11EventStateView *)D_8006C838)->field_C04E) >= 0x96) {
            Ovl11QueueDatedEvent(2, 0x18, 1);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_C3F6) >= 0x64) {
            Ovl11QueueDatedEvent(2, 0x19, 3);
        }
        var_s1 = 0x1A;
        var_a0 = (s16 *) (base + 0xDE16);
        for (var_v1_2 = 0; var_v1_2 < 7; var_v1_2++) {
            if (*var_a0 < 0x64) {
                var_s1 = 0;
            }
            var_a0 += 0x7C;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 0);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_BE7C) > 0xEA5FU) && (GetGameFlag(0x25U) == 0) && (Ovl11GetEventEligibilityFlag() == 1)) {
            var_s1 = 0x1B;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 4);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0xEA5FU) && (GetGameFlag(0x22U) == 0) && (Ovl11GetEventEligibilityFlag() == 1)) {
            var_s1 = 0x1C;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 3);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0xEA5FU) && (GetGameFlag(0x21U) == 0) && (Ovl11GetEventEligibilityFlag() == 1)) {
            var_s1 = 0x1D;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 2);
        }
        var_s1 = 0;
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0xEA5FU) && (GetGameFlag(0x23U) == 0) && (Ovl11GetEventEligibilityFlag() == 1)) {
            var_s1 = 0x1E;
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 0);
        }
        var_s1 = 0;
        if ((GetGameFlag(0x24U) == 0) && (Ovl11GetEventEligibilityFlag() == 1)) {
            var_s1 = 0x1F;
            if ((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0xEA5FU) {
                var_s1 = 0x20;
            }
        }
        if (var_s1 != 0) {
            Ovl11QueueDatedEvent(2, var_s1, 1);
        }
    }
    var_v1 = (Ovl11EventStateView *)D_8006C838;
    if ((temp_s3 == (var_v1->field_5246)) && (temp_s6 == (var_v1->field_5248))) {
        if (((u16) (var_v1->field_BE7C) > 0x9C3FU) && (GetGameFlag(0x20U) == 0)) {
            Ovl11QueueDatedEvent(2, 0x29, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0x9C3FU) && (GetGameFlag(0x1DU) == 0)) {
            Ovl11QueueDatedEvent(2, 0x2A, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0x9C3FU) && (GetGameFlag(0x1CU) == 0)) {
            Ovl11QueueDatedEvent(2, 0x2B, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0x9C3FU) && (GetGameFlag(0x1EU) == 0)) {
            Ovl11QueueDatedEvent(2, 0x2C, 0);
        }
        if (((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0x9C3FU) && (GetGameFlag(0x1FU) == 0)) {
            Ovl11QueueDatedEvent(2, 0x2D, 0);
        }
        if (temp_s3 != 1) {
            if ((((Ovl11EventStateView *)D_8006C838)->field_C79E) >= 0xC8) {
                Ovl11QueueDatedEvent(2, 0x2E, 0);
            }
        }
        if (((((Ovl11EventStateView *)D_8006C838)->field_AE06) >= 0xC8) && (GetGameFlag(0x129U) == 1)) {
            Ovl11QueueDatedEvent(2, 0x2F, 0);
        }
        if ((((Ovl11EventStateView *)D_8006C838)->field_A88A) >= 0xC8) {
            Ovl11QueueDatedEvent(2, 0x30, 0);
        }
    }
    if (!(events->flags & 1) && ((temp_s3 == 0) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0)->field_498C[Ovl11ItemCounterIndex(0x41U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[Ovl11ItemCounterIndex(0x42U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[Ovl11ItemCounterIndex(0x43U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0)->field_498C[Ovl11ItemCounterIndex(0x45U)] >= 0x64U)) {
            Ovl11QueueDatedEvent(2, 0x38, 0);
            events->flags = (s32) (events->flags | 1);
        }
    }
    if (!(events->flags & 2) && ((temp_s3 == 1) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0_2 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0_2)->field_498C[Ovl11ItemCounterIndex(0x46U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[Ovl11ItemCounterIndex(0x47U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[Ovl11ItemCounterIndex(0x48U)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_2)->field_498C[Ovl11ItemCounterIndex(0x4AU)] >= 0x64U)) {
            Ovl11QueueDatedEvent(2, 0x39, 0);
            events->flags = (s32) (events->flags | 2);
        }
    }
    if (!(events->flags & 4) && ((temp_s3 == 2) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1))) {
        temp_s0_3 = (void *)D_8006C838;
        if ((((Ovl11EventStateView *)temp_s0_3)->field_498C[Ovl11ItemCounterIndex(0x4BU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[Ovl11ItemCounterIndex(0x4CU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[Ovl11ItemCounterIndex(0x4DU)] >= 0x64U) && (((Ovl11EventStateView *)temp_s0_3)->field_498C[Ovl11ItemCounterIndex(0x4FU)] >= 0x64U)) {
            Ovl11QueueDatedEvent(2, 0x3A, 0);
            events->flags = (s32) (events->flags | 4);
        }
    }
    if (!(events->flags & 8) && ((temp_s3 == 3) || ((((Ovl11EventStateView *)D_8006C838)->field_44D4) == 1)) && (GetGameFlag(0x10EU) == 1)) {
        Ovl11QueueDatedEvent(2, 0x3B, 0);
        events->flags = (s32) (events->flags | 8);
    }
    if (!(events->flags & 0x10) && (~((((Ovl11EventStateView *)D_8006C838)->field_C) & 0x200000) != 0) && (((u16) (((Ovl11EventStateView *)D_8006C838)->field_9F68) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_AA60) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_B384) > 0xC34FU) || ((u16) (((Ovl11EventStateView *)D_8006C838)->field_BE7C) > 0xC34FU))) {
        Ovl11QueueDatedEvent(2, 0x3C, 0);
        events->flags = (s32) (events->flags | 0x10);
    }
    if (!(events->flags & 0x20) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x80)) {
        Ovl11QueueDatedEvent(2, 0x3D, 0);
        events->flags = (s32) (events->flags | 0x20);
    }
    if (!(events->flags & 0x40) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 1)) {
        Ovl11QueueDatedEvent(2, 0x3E, 0);
        events->flags = (s32) (events->flags | 0x40);
    }
    if (!(events->flags & 0x80) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 2)) {
        Ovl11QueueDatedEvent(2, 0x3F, 0);
        events->flags = (s32) (events->flags | 0x80);
    }
    if (!(events->flags & 0x100) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x40000) && ((Ovl11CountStatusTaggedRecordsA() > 0) || (Ovl11CountStatusTaggedRecordsB() > 0) || ((((Ovl11EventStateView *)D_8006C838)->field_929C) != 0))) {
        Ovl11QueueDatedEvent(2, 0x40, 0);
        events->flags = (s32) (events->flags | 0x100);
    }
    if (!(events->flags & 0x200) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x80000) && (Ovl11CountStatusTaggedRecordsA() > 0)) {
        Ovl11QueueDatedEvent(2, 0x41, 0);
        events->flags = (s32) (events->flags | 0x200);
    }
    if (!(events->flags & 0x400) && ((((Ovl11EventStateView *)D_8006C838)->field_51C0) & 0x100000) && (Ovl11CountStatusTaggedRecordsB() > 0)) {
        Ovl11QueueDatedEvent(2, 0x42, 0);
        events->flags = (s32) (events->flags | 0x400);
    }
    if (!(events->flags & 0x800) && (GetGameFlag(0x24U) == 1)) {
        Ovl11QueueDatedEvent(2, 0x43, 1);
        events->flags = (s32) (events->flags | 0x800);
    }
    if (!(events->flags & 0x1000) && ((((Ovl11EventStateView *)D_8006C838)->field_44B8.word) == 0x10000) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) == 1)) {
        Ovl11QueueDatedEvent(2, 0x26, 0);
        events->flags = (s32) (events->flags | 0x800);
    }
    if (((((Ovl11EventStateView *)D_8006C838)->field_44B8.halves.high) == 2) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) == 0)) {
        Ovl11QueueDatedEvent(2, 0x27, 0);
    }
    if (!(events->flags & 0x2000) && ((((Ovl11EventStateView *)D_8006C838)->field_44B8.halves.high) == 1) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) >= 0) && ((((Ovl11EventStateView *)D_8006C838)->field_44BC) < 0x11) && ((((Ovl11EventStateView *)D_8006C838)->field_5492) == -1) && (GetGameFlag(0x22U) == 0) && ((u16) (((Ovl11EventStateView *)D_8006C838)->field_A6B8) >= 0x3A99U)) {
        Ovl11QueueDatedEvent(2, 0x19B, 0);
        events->flags = (s32) (events->flags | 0x2000);
    }
}

s32 Ovl11GetEventEligibilityFlag(void) {
    s32 enabled;
    enabled = GetGameFlag(0x1B);
    return enabled;
}

void Ovl11CompleteLatchedEventMessage(void) {
    Ovl11MatchItemSlotsCommand(0x103, 1, 3, 1);
    Ovl11InsertSecondaryItemCommand(0x103, 0, 0, 0);
    ((struct Ovl11F3840State *)D_8006C838)->field_E6CB =
        ((struct Ovl11F3840State *)D_8006C838)->field_44B8 + 1;
}
