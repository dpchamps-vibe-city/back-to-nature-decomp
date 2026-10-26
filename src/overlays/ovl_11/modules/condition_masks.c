/*
 * Packed calendar/daily-condition allow-lists, ovl_11:800F1B28..800F1D54.
 * The composite predicate combines three selectors and two optional record
 * queries. All state belongs to other services; this module only reads it.
 * See ovl11_condition_masks.h for bit assignments and short-circuit behavior,
 * and notes/file-groupings.md for membership and boundary evidence.
 */
#include "common.h"
#include "ovl11_condition_masks.h"

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 Ovl11TestConditionMask(s32 conditions) {
    if (Ovl11MaskAllowsSeason(conditions) == 0) {
        return 0;
    }
    if (Ovl11MaskAllowsWeekday(conditions) == 0) {
        return 0;
    }
    if (Ovl11MaskAllowsDayCondition(conditions) == 0) {
        return 0;
    }
    if (conditions & 0x20000) {
        if (ovl_11_func_800E6EE0(0, 0, 1, 0) == 0) {
            return 0;
        }
    }
    if (conditions & 0x10000) {
        if (ovl_11_func_800E6EE0(0, 1, 0, 0) == 0) {
            return 0;
        }
    }
    return 1;
}

s32 Ovl11MaskAllowsSeason(s32 conditions) {
    s32 selectedBit = 0;

    switch (gCalendarSeason) {
    case 0:
        selectedBit = 0x40000000;
        break;
    case 1:
        selectedBit = 0x20000000;
        break;
    case 2:
        selectedBit = 0x10000000;
        break;
    case 3:
        selectedBit = 0x08000000;
        break;
    }
    return (selectedBit & conditions) != 0;
}

s32 Ovl11MaskAllowsWeekday(s32 conditions) {
    s32 selectedBit = 0;

    switch (gCalendarWeekday) {
    case 0:
        selectedBit = 0x4000000;
        break;
    case 1:
        selectedBit = 0x2000000;
        break;
    case 2:
        selectedBit = 0x1000000;
        break;
    case 3:
        selectedBit = 0x800000;
        break;
    case 4:
        selectedBit = 0x400000;
        break;
    case 5:
        selectedBit = 0x200000;
        break;
    case 6:
        selectedBit = 0x100000;
        break;
    }
    return (selectedBit & conditions) != 0;
}

s32 Ovl11MaskAllowsDayCondition(s32 conditions) {
    char *work;
    s32 selectedBit;

    /* An unrestricted field must also accept unknown current codes. */
    selectedBit = conditions & 0xC3800;
    if (selectedBit == 0xC3800) {
        return 1;
    }

    /* Keep the shared-work origin explicit rather than introducing a second
     * global identity for this interior halfword. */
    work = (char *)D_8006C838;
    selectedBit = 0;
    switch (*(s16 *)(work + 0xE4C8)) {
    case 0:
        selectedBit = 0x80000;
        break;
    case 2:
        selectedBit = 0x40000;
        break;
    case 1:
        selectedBit = 0x2000;
        break;
    case 3:
        selectedBit = 0x1000;
        break;
    case 4:
        selectedBit = 0x800;
        break;
    }

    return (selectedBit & conditions) != 0;
}
