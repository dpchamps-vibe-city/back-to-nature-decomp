/*
 * Area-record action dispatch and its three exclusive selection helpers
 * (ovl_11:8010D314–8010DA04). The object tail supplies an action mask,
 * count sources and pending item IDs. Selection is not allocation: the
 * external selector chooses eligible indices; these helpers update flags,
 * invoke record actions and handle returned items. No pool is owned here.
 */

#include "common.h"
#include "game_types.h"
#include "random.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"
#include "ovl11_area_slot_flags.h"
#include "ovl11_area_record_claims.h"
#include "ovl11_save_counters.h"
#include "ovl11_seasonal_record_ids.h"
#include "ovl11_tagged_record_handlers.h"
#include "ovl11_grid_candidate_search.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/abs.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_code160_record_handlers.h"
#include "ovl11_record_position_tables.h"

s32 ovl_11_func_800F3BCC(u16 arg0);
s32 Ovl11ProcessAreaSlotActions(u8 *object, s16 area);
s32 Ovl11ProcessAreaAction6Records(u8 *object, s16 area);
s32 Ovl11ProcessAreaItemActions(u8 *object, s16 area);

s32 Ovl11DispatchAreaRecordAction(u8 *object) {
    s32 random;
    s32 count;
    s32 i;
    s16 item;
    s16 x;
    s16 y;
    u8 *cell;

    if (*(u16 *)(object + 0xB8) & 1) {
        random = Rand(3);
        count = (u16)(*(u16 *)(object + 0xAE) / 50U) + random;
        for (i = 0; i < count; i++) {
            if (*(u16 *)(object + 0xB6) != 0) {
                item = *(u16 *)(object + 0xB6);
                Ovl11ClearRecordItemSprite(object);
            } else {
                if (Ovl11FindCandidateGridCell(&x, &y, 1, 1, 1)) {
                    if (Ovl11FindCandidateGridCell(&x, &y, 0, 1, 1)) {
                        return 0;
                    }
                    cell = (u8 *)&gAlternateAreaGrid[y][x];
                } else {
                    cell = (u8 *)&((Ovl11D124Entry (*)[45])((u8 *)gAlternateAreaGrid - 0x2328))[y][x];
                }
                item = Ovl11ResolveCellItemState((void *)cell);
            }
            ovl_11_func_800F3BCC(item);
        }
    } else if (*(u16 *)(object + 0xB8) & 2) {
        random = Rand(5);
        count = (u16)(*(u16 *)(object + 0xB0) / 10U) + random;
        for (i = 0; i < count; i++) {
            if (Ovl11FindCandidateGridCell(&x, &y, 1, 0, 1)) {
                if (Ovl11FindCandidateGridCell(&x, &y, 0, 0, 1)) {
                    break;
                }
                Ovl11TryPromoteCellPendingKind(1, x, y, 0);
            } else {
                Ovl11TryPromoteCellPendingKind(0, x, y, 0);
            }
        }
    } else if (*(u16 *)(object + 0xB8) & 4) {
        switch (*(u16 *)(object + 0xB6)) {
        case 0:
            break;
        case 0x68:
            Ovl11AddSaveCounter18(0, 0);
            break;
        case 0x69:
            Ovl11AddSaveCounter20(0, 0);
            break;
        default:
            ovl_11_func_800F3BCC(*(u16 *)(object + 0xB6));
            break;
        }
        Ovl11ClearRecordItemSprite(object);
        /* A nonzero helper result opens the next fallback. Preserve the
         * distinct success/failure tests and area order, not a flat retry loop. */
        if (Ovl11ProcessAreaSlotActions(object, 5)) {
            if (!Ovl11ProcessAreaAction6Records(object, 5)) {
                if (!Ovl11ProcessAreaItemActions(object, 5)) {
                    return 0;
                }
            }
            if (Ovl11ProcessAreaSlotActions(object, 4)) {
                if (Ovl11ProcessAreaItemActions(object, 4)) {
                    if (!Ovl11ProcessAreaAction6Records(object, 1)) {
                        if (!Ovl11ProcessAreaItemActions(object, 1)) {
                            return 0;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

s32 ovl_11_func_80110944(s16 area, u16 mode);

/* Mode-1 selections update the area-specific availability bit. An unavailable
 * result latches failure but does not cancel earlier effects or later trials. */
s32 Ovl11ProcessAreaSlotActions(u8 *object, s16 area) {
    s32 random;
    s32 failed;
    s32 i;
    s32 count;
    s16 index;

    random = Rand(3);
    count = (u16)(*(u16 *)(object + 0xB2) / 50U) + random;
    failed = 0;
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 1);
        if (index == -1) {
            failed = 1;
            break;
        }
        if (area == 4) {
            if (Ovl11TakeSaveCounter20(1)) {
                Ovl11SetAreaSlotFlag(index, 0);
            } else {
                failed = 1;
            }
        } else if (area == 5) {
            if (!Ovl11TakeSaveCounter18(1)) {
                failed = 1;
            } else {
                Ovl11SetAreaSlotFlag(index, 1);
            }
        }
    }
    return failed;
}

/* Mode-2 selections receive action 6 through the flag-selected dispatcher. */
s32 Ovl11ProcessAreaAction6Records(u8 *object, s16 area) {
    s32 random;
    s32 failed;
    s32 i;
    s32 count;
    s16 index;
    s32 offset;
    u8 *base;
    Ovl30AnimalRecordB8 *records;
    Ovl11ClearFlagRecord *flags;

    random = Rand(3);
    count = (u16)(*(u16 *)(object + 0xB2) / 100U) + random;
    failed = 0;
    base = D_80074838;
    flags = (Ovl11ClearFlagRecord *)(base + 0x1F0);
    records = (Ovl30AnimalRecordB8 *)(base + 0x1BC);
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 2);
        if (index == -1) {
            failed = 1;
            break;
        }
        offset = index * 0xB8;
        if (!(*(s32 *)((u8 *)flags + offset) & 0x10000)) {
            Ovl11DispatchCode160Handler((Ovl11ActionDispatchView *)(offset + (u32)records), 6);
        } else {
            Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)(offset + (u32)records), 6);
        }
    }
    return failed;
}

/* Try at most one mode-4 record, copying the input ID to its pending-item
 * field. If that produces no item, mode-8 records supply the fallback. The
 * count includes Rand(3) even on the single-attempt path; zero skips it. */
s32 Ovl11ProcessAreaItemActions(u8 *object, s16 area) {
    s32 random;
    s32 failed;
    s32 fallbackFailed;
    s32 i;
    s32 count;
    s32 status;
    s32 result;
    s16 index;
    u8 *record;
    u8 *base;
    Ovl11RecordPresentation out;

    random = Rand(3);
    count = (u16)(*(u16 *)(object + 0xB2) / 100U) + random;
    failed = 0;
    for (i = 0; i < count; i++) {
        index = ovl_11_func_80110944(area, 4);
        if (index == -1) {
            failed = 1;
            break;
        }
        record = (u8 *)&gTaggedRecordBase + index * 0xB8;
        *(u16 *)(record + 0xB6) = *(u16 *)(object + 0xAC);
        if (!(*(s32 *)(record + 0x34) & 0x10000)) {
            Ovl11DispatchCode160Handler((Ovl11ActionDispatchView *)record, 7);
        } else {
            Ovl11DispatchTaggedRecordHandler((Ovl11ActionDispatchView *)record, 8);
        }
        if (*(u16 *)(object + 0xB6) == 0) {
            failed = 1;
        }
        break;
    }
    i = 0;
    if (failed) {
        fallbackFailed = 0;
        if (count > 0) {
            base = D_80074838;
            do {
                result = (s16)ovl_11_func_80110944(area, 8);
                status = -1;
                if (result != status) {
                    status = Ovl11ConsumeSeasonalRecord((Ovl11SeasonalRecordView *)(*(u8 **)(base + 0x5DCC) + result * 0x18), &out);
                    result = 1;
                    if (status == result) {
                        *(u16 *)(object + 0xB6) = out.id;
                        continue;
                    }
                }
                fallbackFailed = 1;
                goto fallback_done;
            } while (++i < count);
        }
fallback_done:
        failed = fallbackFailed;
    }
    if (*(u16 *)(object + 0xB6) != 0) {
        ovl_11_func_800F3BCC(*(u16 *)(object + 0xB6));
        Ovl11ClearRecordItemSprite(object);
    }
    return failed;
}
