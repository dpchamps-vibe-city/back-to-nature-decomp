/*
 * Item counts and sell-value accounting, ovl_11:800F3BCC-800F3F7C.
 * Direct recording and staged batches share the same 25-category mapper.
 * Commit moves a pending batch into the current bank; balance application is
 * separate and guarded by game flag A0. Public contract:
 * ovl11_item_value_counters.h.
 *
 * The unused selection-bank reset retains its original intervening position.
 * No linkage or storage ownership is inferred from consolidating these entries.
 */
#include "common.h"
#include "ovl11_item_value_counters.h"
#include "ovl11_clock_calendar.h"
#include "game_flags.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_grid_lookup.h"
#include "ovl11_balance_accounting.h"

s32 Ovl11RecordItemValue(u16 itemId) {
    u16 *counts;
    u16 *work;
    u16 *categoryCount;
    s32 id;
    s32 category;
    s32 specialCount;
    s32 nextCount;

    counts = gItemValueCounterWork;
    id = itemId & 0xFFFF;
    if ((u32)(id - 0xA1) < 3U) {
        /* Rebase the counter anchor to the resident work header. */
        work = counts - 0x24C6;
        specialCount = *(u16 *)((u8 *)work + 0x44DC) + 1;
        *(u16 *)((u8 *)work + 0x44DC) = specialCount;
        if ((s16)specialCount >= 0xC8) {
            SetGameFlag(0x5FU, 1U);
        }
    }
    *(s32 *)((u8 *)counts + 0x34) = *(s32 *)((u8 *)counts + 0x34) + Ovl11GetItemSellValue(id);
    category = Ovl11ItemCounterIndex((u16)id);
    if (category == -1) {
        return 0;
    }
    categoryCount = &counts[category];
    nextCount = *categoryCount + 1;
    *categoryCount = nextCount;
    if ((u32)(nextCount & 0xFFFF) >= 0x3E8U) {
        *categoryCount = 0x3E7;
    }
    return 1;
}

s32 Ovl11ItemCounterIndex(u16 itemId) {
    if (itemId < 0x41) {
        return -1;
    }
    if (itemId < 0x50) {
        return (s16)(itemId - 0x41);
    }
    if ((u32)(itemId - 0x5D) < 0xC8) {
        switch (itemId) {
        case 232:
        case 290:
        case 291:
        case 292:
            return 0xF;
        case 139:
        case 140:
        case 141:
        case 227:
            return 0x10;
        case 145:
        case 146:
        case 147:
        case 231:
            return 0x11;
        case 136:
        case 137:
        case 138:
        case 229:
            return 0x12;
        case 142:
        case 143:
        case 144:
        case 228:
            return 0x13;
        case 148:
        case 149:
        case 150:
        case 230:
            return 0x14;
        case 161:
        case 162:
        case 163:
            return 0x15;
        case 93:
        case 94:
        case 95:
            return (s16)(itemId + 0xFFB9);
        }
    }
    return -1;
}

/* Twenty availability bytes plus two selection bytes, then a ten-byte bank.
 * Keep offset formation separate from base addition so loop reduction emits
 * an independent descending cursor for each bank. */
void Ovl11ResetSelectionFlagBanks(void) {
    s32 i;
    s32 offset;
    u8 *byte;

    for (i = 21; i >= 0; i--) {
        offset = i + 0x4AC0;
        byte = (u8 *)((s32)D_8006C838 + offset);
        *byte = 0;
    }
    for (i = 9; i >= 0; i--) {
        offset = i + 0x4AD6;
        byte = (u8 *)((s32)D_8006C838 + offset);
        *byte = 0;
    }
}

s32 Ovl11ApplyItemValueOnce(void) {
    u8 *work;
    u16 *counts;

    if (GetGameFlag(0xA0) == 1) {
        return 1;
    }
    SetGameFlag(0xA0, 1);
    work = (u8 *)&D_8006C838;
    if (Ovl11FindCalendarDate(*(s16 *)(work + 0x44BA), *(s16 *)(work + 0x44BC)) != 0) {
        return 1;
    }
    counts = (u16 *)(work + 0x498C);
    Ovl11AdjustSpendableBalance(*(s32 *)(counts + 0x1A), 0);
    *(s32 *)(counts + 0x1A) = 0;
    return 1;
}

s32 Ovl11GetItemCounter(s16 itemId) {
    u16 *counts;
    s32 category;

    counts = gItemValueCounterWork;
    category = Ovl11ItemCounterIndex(itemId);
    if (category == -1) {
        return -1;
    }
    return counts[category];
}

s32 Ovl11StageItemValue(s16 itemId) {
    Ovl11PendingItemValueCounters *pending;
    s32 id;
    s32 category;

    pending = (Ovl11PendingItemValueCounters *)&gPendingItemValueCount;
    if (pending->acceptedCount < 0x1E) {
        id = itemId & 0xFFFF;
        pending->totalValue += Ovl11GetItemSellValue(id);
        pending->acceptedCount += 1;
        category = Ovl11ItemCounterIndex((u16)id);
        if (category == -1) {
            return 0;
        }
        pending->categoryCounts[category] += 1;
        return 0;
    }
    return 1;
}

s32 Ovl11CommitPendingItemCounts(void) {
    u16 *counts;
    u16 *pending;
    u16 *source;
    u16 *destination;
    s32 i;
    u32 cap;

    counts = gItemValueCounterWork;
    pending = counts + 0x7E;
    if (*pending == 0) {
        return 0;
    }
    cap = 999;
    source = counts + 0x7F;
    destination = counts;
    for (i = 0; i < 25; i++) {
        u16 *current = destination;
        *current += *source;
        /* The two cleanup arms retain the original shared-tail allocation. */
        if (*current >= 1000) {
            *current = cap;
            *source++ = 0;
        } else {
            *source++ = 0;
        }
        destination++;
    }
    *(s32 *)&counts[0x1A] += *(s32 *)&pending[0x1A];
    pending[0] = 0;
    *(s32 *)&pending[0x1A] = 0;
    return 1;
}
