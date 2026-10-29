/*
 * progress_status — reconstructed translation unit.
 *
 * Progress timers, request acknowledgement and the complete 23-bit
 * derived-status refresh pipeline. See ovl11_progress_status.h for
 * operations, ordering and shared-state effects.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_progress_status.h"
#include "game_flags.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_default_position.h"
#include "ovl11_pool_record_transition.h"
#include "ovl11_record_position_recovery.h"
#include "ovl11_item_slot_commands.h"
#include "ovl11_item_record_search.h"
#include "ovl11_item_value_counters.h"

s32 *Ovl11ResetProgressControl(void) {
    Ovl11RefreshProgressStatusEntry();
    ((Ovl11ProgressState *)&D_8006C838)->activeTimer = 0;
    ((Ovl11ProgressState *)&D_8006C838)->request = 0;
    ((Ovl11ProgressState *)&D_8006C838)->pendingTimer = 0;
    return (s32 *)&D_8006C838;
}

void Ovl11RequestProgressAction(s16 request) {
    s32 timer;

    timer = ((u32)(request - 0x13F) < 3) << 2;
    if ((u32)(request - 0xF7) < 3) {
        timer = 2;
    }
    if ((u32)(request - 1) < 0x19) {
        timer = 2;
    }
    if (timer == 0) {
        ((Ovl11ProgressState *)&D_8006C838)->activeTimer = 0;
    }
    ((Ovl11ProgressState *)&D_8006C838)->request = request;
    ((Ovl11ProgressState *)&D_8006C838)->activeTimer = timer;
    SetGameFlag(0x37, 1);
}

void Ovl11ArmProgressPendingTimer(void) {
    gProgressPendingTimer = 3;
}

void Ovl11RefreshProgressStatusEntry(void) {
    Ovl11RefreshProgressStatus();
}

/* Each helper refreshes a disjoint field of the cached status word. */
void Ovl11RefreshCounterGroupAFlag(void);

void Ovl11RefreshCounterGroupBFlag(void);

void Ovl11RefreshCounterGroupCFlag(void);

void Ovl11RefreshPurchaseAvailabilityFlags(void);

void Ovl11RefreshRecordThresholdFlag(void);

void Ovl11RefreshStageProgressFlags(void);

void Ovl11RefreshStageEventFlag(void);

void Ovl11RefreshCounter5DFlag(void);

void Ovl11RefreshCounter5FFlag(void);

void Ovl11RefreshStageGateFlags(void);

void Ovl11RefreshEventFlagsHigh(void);

void Ovl11RefreshEventFlagsTop(void);

void Ovl11RefreshProgressStatus(void) {
    Ovl11RefreshCounterGroupAFlag();
    Ovl11RefreshCounterGroupBFlag();
    Ovl11RefreshCounterGroupCFlag();
    Ovl11RefreshPurchaseAvailabilityFlags();
    Ovl11RefreshRecordThresholdFlag();
    Ovl11RefreshStageProgressFlags();
    Ovl11RefreshStageEventFlag();
    Ovl11RefreshCounter5DFlag();
    Ovl11RefreshCounter5FFlag();
    Ovl11RefreshStageGateFlags();
    Ovl11RefreshEventFlagsHigh();
    Ovl11RefreshEventFlagsTop();
}

void Ovl11UpdateProgressTimers(void) {
    s32 stageFlag;
    Ovl11ProgressState *state;
    u32 selectedFlag;

    if (((Ovl11ProgressState *)D_8006C838)->activeTimer == 0 ||
        --((Ovl11ProgressState *)D_8006C838)->activeTimer < 0) {
        SetGameFlag(0x37U, 0U);
        ((Ovl11ProgressState *)D_8006C838)->activeTimer = 0;
    }
    if (GetGameFlag(0x38U) == 1) {
        if (--((Ovl11ProgressState *)D_8006C838)->pendingTimer < 0) {
            Ovl11RecoverRecordPositions();
            Ovl11PublishCurrentPoolRecord();
            Ovl11PublishNextPoolRecord();
            SetGameFlag(0x38U, 0U);
            ((Ovl11ProgressState *)D_8006C838)->pendingTimer = 0U;
            for (stageFlag = 0x3A; stageFlag < 0x3E; stageFlag++) {
                if (GetGameFlag((u16)stageFlag) == 0) {
                    break;
                }
            }
            selectedFlag = (u16)stageFlag;
            switch (stageFlag) {
            case 0x3A:
                ((Ovl11ProgressState *)D_8006C838)->stage2 = 1;
                break;
            case 0x3B:
                ((Ovl11ProgressState *)D_8006C838)->stage0 = 1;
                break;
            case 0x3C:
                ((Ovl11ProgressState *)D_8006C838)->stage1 = 1;
                break;
            case 0x3D:
                ((Ovl11ProgressState *)D_8006C838)->stage0 = 2;
                break;
            case 0x3E:
                state = (Ovl11ProgressState *)D_8006C838;
                state->complete = 1;
                state->flags = (s32) (state->flags | 0x2000);
                SetGameFlag(0x39U, 1U);
                break;
            }
            SetGameFlag((u16) selectedFlag, 1U);
            if (((Ovl11ProgressState *)D_8006C838)->mode == 2) {
                Ovl11ReadDefaultPosition(&gOvl11DefaultPositionWork);
            }
        }
    }
    if (((Ovl11ProgressState *)D_8006C838)->cooldown != 0) {
        if (--((Ovl11ProgressState *)D_8006C838)->cooldown < 0) {
            ((Ovl11ProgressState *)D_8006C838)->cooldown = 0;
        }
    }
}

/* Preserve the shared-work origin in each access; the direct request label
 * is the same halfword at +0x51C6, not another allocation. */
#define OVL11_STATUS_WORK ((Ovl11ProgressState *)D_8006C838)

u16 Ovl11HandleProgressRequest(s32 operation) {
    u16 result;
    s16 request;

    result = 0;
    switch (operation) {
    case 0:
        return gProgressRequest != 0;
    case 1:
        result = (u16)gProgressRequest;
        break;
    case 2:
        request = OVL11_STATUS_WORK->request;
        result = *(u16 *)&OVL11_STATUS_WORK->request;
        switch (request) {
        case 0x13F:
            result = 0;
            OVL11_STATUS_WORK->stage2 = operation;
            OVL11_STATUS_WORK->flags = OVL11_STATUS_WORK->flags | 0x100000;
            goto tail;
        case 0x140:
            result = 0;
            OVL11_STATUS_WORK->stage1 = operation;
            OVL11_STATUS_WORK->flags = OVL11_STATUS_WORK->flags | 0x100;
tail:
            Ovl11RefreshProgressStatus();
            goto done;
        case 0x141:
            result = 0;
            OVL11_STATUS_WORK->stage1 = 3;
            OVL11_STATUS_WORK->flags = OVL11_STATUS_WORK->flags | 0x200;
            Ovl11RefreshProgressStatus();
done:
            OVL11_STATUS_WORK->request = 0;
            break;
        default:
            OVL11_STATUS_WORK->request = 0;
            return result;
        }
        break;
    }
    return result;
}

#undef OVL11_STATUS_WORK

void Ovl11RefreshCounterGroupAFlag(void) {
    s32 *base;
    s16 *p;
    s32 i;

    base = &gProgressDerivedFlags;
    i = 0;
    p = &gOvl11StatusCounterGroupAIds;
    *base &= ~4;
    while (i < 4 && Ovl11GetItemCounter(*p) >= 0x65) {
        p++;
        i++;
    }
    if (i < 4) {
        return;
    }
    *base |= 4;
}

void Ovl11RefreshCounterGroupBFlag(void) {
    s32 *base;
    s16 *p;
    s32 i;

    base = &gProgressDerivedFlags;
    i = 0;
    p = &gOvl11StatusCounterGroupBIds;
    *base &= ~8;
    while (i < 4 && Ovl11GetItemCounter(*p) >= 0x65) {
        p++;
        i++;
    }
    if (i < 4) {
        return;
    }
    *base |= 8;
}

void Ovl11RefreshCounterGroupCFlag(void) {
    s32 *base;
    s16 *p;
    s32 i;

    base = &gProgressDerivedFlags;
    i = 0;
    p = &gOvl11StatusCounterGroupCIds;
    *base &= ~0x10;
    while (i < 4 && Ovl11GetItemCounter(*p) >= 0x65) {
        p++;
        i++;
    }
    if (i < 4) {
        return;
    }
    *base |= 0x10;
}

void Ovl11RefreshCounter5DFlag(void) {
    s32 *base;

    base = &gProgressDerivedFlags;
    *base &= ~1;

    if (Ovl11GetItemCounter(0x5D) >= 0x33) {
        *base |= 1;
    }
}

void Ovl11RefreshCounter5FFlag(void) {
    s32 *base;

    base = &gProgressDerivedFlags;
    *base &= ~2;

    if (Ovl11GetItemCounter(0x5F) >= 0x33) {
        *base |= 2;
    }
}

void Ovl11RefreshPurchaseAvailabilityFlags(void) {
    s32 *base;
    u8 *p;

    base = &gProgressDerivedFlags;
    *base &= ~0xE0;

    p = (u8 *)base - 0x51C0;
    if (!(*(s32 *)(p + 0x44F8) & 0x800)) {
        *base |= 0x20;
    }

    if (*(s16 *)(p + 0x51E6) == 0) {
        *base |= 0x40;
        return;
    }
    if (*(s16 *)(p + 0x51E6) == 2) {
        return;
    }
    if (*(s16 *)(p + 0x51C4) > 0) {
        return;
    }
    *base |= 0x80;
}

void Ovl11RefreshRecordThresholdFlag(void) {
    s32 *base;
    struct_80076220 *iter;
    s32 i;

    base = &gProgressDerivedFlags;
    *base &= ~0x100;

    if (!(*(s32 *)((u8 *)base - 0xCC8) & 0x200000)) {
        iter = &gObjectEventSlotBase;
        for (i = 0; i < 37; i++) {
            if (*(u16 *)((u8 *)iter + 4) > 0xC350U) {
                *base |= 0x100;
            }
            iter++;
        }
    }
}

void Ovl11RefreshStageProgressFlags(void) {
    s32 *base;
    u8 *p;
    s32 v;
    u16 d6;

    base = &gProgressDerivedFlags;
    v = *base & ~0x3E00;
    *base = v;

    p = (u8 *)base - 0x51C0;
    if (*(u16 *)(p + 0x44D2) == 0) {
        *base = v | 0x200;
        return;
    }
    d6 = *(u16 *)(p + 0x44CE);
    if (d6 == 0) {
        *base = v | 0x400;
        return;
    }
    if (*(u16 *)(p + 0x44D0) == 0) {
        *base = v | 0x800;
        return;
    }
    if (d6 == 1) {
        *base = v | 0x1000;
        return;
    }
    if (*(u16 *)(p + 0x44D4) == 0) {
        *base = v | 0x2000;
    }
}

void Ovl11RefreshStageEventFlag(void) {
    gProgressDerivedFlags &= ~0x4000;
    if (Ovl11MatchItemSlotsCommand(0x1E, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x4000;
    }
}

void Ovl11RefreshStageGateFlags(void) {
    s32 *base;
    u8 *p;
    u8 *c;
    s32 flags;

    base = &gProgressDerivedFlags;
    *base &= 0xFFFC7FFF;

    p = (u8 *)base - 0x51C0;
    if (*(u16 *)(p + 0x44D2) != 0) {
        if ((*(s32 *)(p + 0x44F8) & 0x100000) == 0) {
            *base |= 0x10000;
            return;
        }
    }

    c = (u8 *)&D_8006C838;
    if (*(u16 *)(c + 0x44D0) == 0) {
        return;
    }

    flags = *(s32 *)(c + 0x44F8);
    if ((flags & 0x100000) == 0) {
        return;
    }
    if ((flags & 0x100) == 0) {
        *base |= 0x8000;
    } else if ((flags & 0x200) == 0) {
        *base |= 0x20000;
    }
}

void Ovl11RefreshEventFlagsHigh(void) {
    gProgressDerivedFlags &= 0xFFE3FFFF;
    if (Ovl11MatchItemSlotsCommand(0x1C, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x40000;
    }
    if (Ovl11MatchItemSlotsCommand(0x1A, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x80000;
    }
    if (Ovl11MatchItemSlotsCommand(0x1B, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x100000;
    }
}

void Ovl11RefreshEventFlagsTop(void) {
    gProgressDerivedFlags &= 0xFF9FFFFF;
    if (Ovl11MatchItemSlotsCommand(0x1F, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x200000;
    }
    if (Ovl11MatchItemSlotsCommand(0x20, 0, 2, 0) == 0) {
        gProgressDerivedFlags |= 0x400000;
    }
}
