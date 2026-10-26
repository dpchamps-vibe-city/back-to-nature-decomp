#include "common.h"
#include "game_types.h"
#include "ovl11_clock_calendar.h"
#include "u16_string.h"
#include "game_flags.h"
#include "ovl11_record_flag_query.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_map_grid_cells.h"

s32 ovl_11_func_800D5C3C(s16 arg0);
s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800EC354(s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800EF0E8(s16 arg0);
s32 ovl_11_func_800EEC1C(s16 id, s16 which, s16 last, s32 amount);
s32 ovl_11_func_800E9D28(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

s32 ovl_11_func_80104EC8(s16 arg0) {
    Ovl11D124Entry record18;
    s16 sp20[4];
    Ovl11D124Entry record28;
    s16 sp30[4];
    Ovl11D124Entry record38;
    s16 sp40[2];
    s16 temp_a1_2;
    s16 temp_s0_2;
    s16 temp_s0_7;
    s16 temp_s0_8;
    s16 temp_v1;
    s16 temp_v1_3;
    s16 temp_v1_5;
    s16 var_a0;
    s16 var_a0_11;
    s16 var_a0_13;
    s16 var_a0_6;
    s16 var_s0;
    s16 var_s0_2;
    s16 var_s1;
    s32 temp_s0;
    s32 temp_s0_6;
    s32 temp_v0;
    s32 var_a0_12;
    s32 var_a0_7;
    s32 var_a0_8;
    s32 var_a0_9;
    s32 var_a1_2;
    s32 var_a1_3;
    s32 var_a2;
    s32 var_a2_3;
    s32 var_a2_5;
    s32 var_a2_6;
    s32 var_a3_2;
    s32 var_a3_3;
    s32 var_s4;
    s32 var_v0;
    s32 var_v0_3;
    u16 temp_v0_2;
    u16 amount_A;
    u16 amount_B;
    u16 amount_C;
    u16 amount_D;
    u16 amount_E;
    u16 amount_F;
    u16 amount_10;
    u16 amount_11;
    u16 amount_13;
    u16 amount_14;
    u16 amount_15;
    u16 amount_17;
    u16 amount_18;
    u16 amount_19;
    u16 amount_1B;
    u16 amount_1C;
    u16 amount_1D;
    u16 amount_1E;
    u16 amount_6B;
    u16 amount_6D;
    u16 amount_6F;
    u32 var_a0_10;
    u32 var_a0_4;
    u32 var_a0_5;
    u32 threshold_A;
    u32 threshold_B;
    u32 threshold_C;
    u32 threshold_D;
    u32 threshold_E;
    u32 threshold_F;
    u32 threshold_10;
    u32 threshold_11;
    u32 threshold_13;
    u32 threshold_14;
    u32 threshold_17;
    u32 threshold_18;
    u32 threshold_19;
    u32 threshold_1B;
    u32 threshold_1C;
    u32 threshold_1D;
    u32 threshold_1E;
    u32 threshold_6B;
    u32 threshold_6D;
    u32 threshold_6F;
    void *temp_v0_3;
    void *temp_v0_4;
    u8 *animal_cursor;

    switch (arg0) {
    case 0x0:
        if (((Ovl11ConditionState *) D_8006C838)->field_929C == 0) {
            temp_v1 = ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind;
            if ((temp_v1 == 0x30) || (temp_v1 == 0x2A)) {
                return 1;
            }
        }
        return 0;
    case 0x1:
    case 0x2:
        return (s32) ((u8) ((Ovl11ConditionState *) D_8006C838)->field_92D1 >> 7);
    case 0x3:
        var_a0 = 0xFA;
        var_a2 = 1;
block_192:
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x4:
        var_a1_2 = 0;
        for (var_a0_7 = 0; var_a0_7 < 10; var_a0_7++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a0_7].id != 0 ||
                (((Ovl11ConditionState *) D_8006C838)->items[var_a0_7].flags & 0x40000)) {
                var_a1_2++;
            }
        }
        if (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0) {
            if (var_a1_2 < 5) {
enough_slots:
                return 1;
            } else {
                return 0;
            }
        } else {
            if (var_a1_2 < 10) {
                goto enough_slots;
            } else {
                return 0;
            }
        }
    case 0x5:
        for (var_a0_8 = 0; var_a0_8 < 10; var_a0_8++) {
            if ((((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].flags & 0x40000000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].id == 0x10A) {
                break;
            }
        }
        if (var_a0_8 != 10) {
            if (((Ovl11ConditionState *) D_8006C838)->date[2] !=
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].tile) {
                return CompareU16StringAgainstPair(((Ovl11ConditionState *) D_8006C838)->items[var_a0_8].name,
                    (u16 *) ((u8 *) D_80051F26 + *D_80054BC0),
                    (u16 *) ((u8 *) D_80051F26 + 0xE + *D_80054BC0)) != 0;
            }
        }
        return 0;
    case 0x26: case 0x2B: case 0x2C: case 0x2D: case 0x2E:
    case 0x4D: case 0x5D: case 0x5E: case 0x60:
        return 1;
    case 0x6:
        for (var_a0_9 = 0; var_a0_9 < 10; var_a0_9++) {
            if ((((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].flags & 0x40000000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].id == 0x10A) {
                break;
            }
        }
        if (var_a0_9 != 10) {
            if (((Ovl11ConditionState *) D_8006C838)->date[2] !=
                ((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].tile) {
                return CompareU16StringAgainstPair(((Ovl11ConditionState *) D_8006C838)->items[var_a0_9].name,
                    (u16 *) ((u8 *) D_80051F26 + *D_80054BC0),
                    (u16 *) ((u8 *) D_80051F26 + 0xE + *D_80054BC0)) == 0;
            }
        }
        return 0;
    case 0x7:
        record18.unk0 = 0x31;
        record18.unk4 = ovl_11_func_800D5C3C(0x31);
        if (Ovl11FindRandomGridCell(&sp20[0], &sp20[1], 1, &record18, 2) == 0) {
            return Ovl11FindRandomGridCell(&sp20[0], &sp20[1], 0, &record18, 2) != 0;
        }
        return 1;
    case 0x8:
        var_a0 = 0x104;
        var_a2 = 1;
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x9:
        temp_v0 = ovl_11_func_800E6EE0(0x103, 1, 1, 0);
        if (temp_v0 == 1) {
            if (((Ovl11ConditionState *) D_8006C838)->field_E6CB ==
                ((Ovl11ConditionState *) D_8006C838)->date[0] + 1) {
                if (ovl_11_func_800EC354(7, 0, -1, -1) == temp_v0) {
                    return 1;
                }
            }
        }
        return 0;
    case 0xA:
        if (((Ovl11ConditionState *) D_8006C838)->field_5492 != -1) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5492 != 0x14) {
                return 1;
            }
            goto block_45;
        }
block_45:
        amount_A = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
        switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: threshold_A = 0x4E20; break;
        case 1: threshold_A = 0x9C40; break;
        default: threshold_A = 0xC350; break;
        }
block_64:
        return amount_A < threshold_A;
    case 0xB:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14))) {
            amount_B = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: threshold_B = 0x4E20; break;
        case 1: threshold_B = 0x9C40; break;
        default: threshold_B = 0xC350; break;
        }
            return amount_B < threshold_B;
        }

        return var_v0;
    case 0xC:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14))) {
            amount_C = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
        case 0: threshold_C = 0x4E20; break;
        case 1: threshold_C = 0x9C40; break;
        default: threshold_C = 0xC350; break;
        }
            return amount_C < threshold_C;
        }

        return var_v0;
    case 0xD:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0x14)) {
            amount_D = ((Ovl11ConditionState *) D_8006C838)->actors[20].amount;
            threshold_D = 0xC350;
            return amount_D < threshold_D;
        }
        return 1;
    case 0xE:
        if (GetGameFlag(0x192U) == 0) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_52C6 == ((Ovl11ConditionState *) D_8006C838)->actors[11].field46) {
                goto block_68;
            }

            return var_v0;
        }
block_68:
        if (((Ovl11ConditionState *) D_8006C838)->date[0] == 0) {
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if ((u32) (((Ovl11ConditionState *) D_8006C838)->field_44BE - 0xE) < 5U) {
                    return 0;
                }
            }
        }
block_72:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
            amount_E = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_E = 0x7530; break;
            case 1: threshold_E = 0x9C40; break;
            default: threshold_E = 0xEA60; break;
            }
            return amount_E < threshold_E;
        } else {

            return var_v0;
        }
        break;
    case 0xF:
        if (GetGameFlag(0xE6U) != 0) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
                amount_F = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
                switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_F = 0x7530; break;
            case 1: threshold_F = 0x9C40; break;
            default: threshold_F = 0xEA60; break;
            }
            return amount_F < threshold_F;
            } else {

                return var_v0;
            }
        } else {
            return 0;
        }
        break;
    case 0x10:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7))) {
            amount_10 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_10 = 0x7530; break;
            case 1: threshold_10 = 0x9C40; break;
            default: threshold_10 = 0xEA60; break;
            }
            return amount_10 < threshold_10;
        } else {

            return var_v0;
        }
        break;
    case 0x11:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7)) {
            amount_11 = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
            threshold_11 = 0xEA60;
            return amount_11 < threshold_11;
        }
        return 1;
    case 0x12:
        var_a0_4 = 0xE6;
block_406:
        return GetGameFlag(var_a0_4) != 1;
    case 0x13:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3))) {
            amount_13 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_13 = 0x4E20; break;
            case 1: threshold_13 = 0x9C40; break;
            default: threshold_13 = 0xEA60; break;
            }
            return amount_13 < threshold_13;
        } else {

            return var_v0;
        }
        break;
    case 0x14:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3))) {
            amount_14 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_14 = 0x4E20; break;
            case 1: threshold_14 = 0x9C40; break;
            default: threshold_14 = 0xEA60; break;
            }
            return amount_14 < threshold_14;
        } else {

            return var_v0;
        }
        break;
    case 0x15:
        if (((Ovl11ConditionState *) D_8006C838)->field_5492 == -1 ||
            ((Ovl11ConditionState *) D_8006C838)->field_5492 == 3) {
            amount_15 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
            if (((Ovl11ConditionState *) D_8006C838)->date[0] == 1) {
                if (amount_15 < 40000) {
                    return 1;
                }
            } else {
                var_v0 = 0;
                if (amount_15 < 60000) {
                    if (((Ovl11ConditionState *) D_8006C838)->actors[20].flags & 8) {
                        return 1;
                    }
                }
                return var_v0;
            }
            return 0;
        }
        return 1;
    case 0x16:
        if (GetGameFlag(0x20U) != 1) {
            if (GetGameFlag(0x25U) != 0) {
                if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3)) {
                    return (u16) ((Ovl11ConditionState *) D_8006C838)->actors[3].amount <= 0xEA5FU;
                }
                return 1;
            }
            return 0;
        }

        return 1;
    case 0x17:
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(0) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 3)) {
                amount_17 = ((Ovl11ConditionState *) D_8006C838)->actors[3].amount;
                threshold_17 = 0xEA60;
                return amount_17 < threshold_17;
            }
            return 1;
        }

        return var_v0;
    case 0x18:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9))) {
            amount_18 = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_18 = 0x4E20; break;
            case 1: threshold_18 = 0x9C40; break;
            default: threshold_18 = 0xEA60; break;
            }
            return amount_18 < threshold_18;
        } else {

            return var_v0;
        }
        break;
    case 0x19:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9))) {
            amount_19 = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_19 = 0x4E20; break;
            case 1: threshold_19 = 0x9C40; break;
            default: threshold_19 = 0xEA60; break;
            }
            return amount_19 < threshold_19;
        } else {

            return var_v0;
        }
        break;
    case 0x1B:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE))) {
            amount_1B = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_1B = 0x4E20; break;
            case 1: threshold_1B = 0x9C40; break;
            default: threshold_1B = 0xEA60; break;
            }
            return amount_1B < threshold_1B;
        } else {

            return var_v0;
        }
        break;
    case 0x1C:
        if ((u32) (((Ovl11ConditionState *) D_8006C838)->field_44C0 - 0xA) < 8U) {
            if ((u32) (((Ovl11ConditionState *) D_8006C838)->field_44C0 - 0xD) >= 3U) {
                if (((Ovl11ConditionState *) D_8006C838)->field_5492 == -1 || ((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE) {
                amount_1C = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
                switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_1C = 0x4E20; break;
            case 1: threshold_1C = 0x9C40; break;
            default: threshold_1C = 0xEA60; break;
            }
            return amount_1C < threshold_1C;
                }
                return 1;
            }
            return 0;
        }
        return 0;
    case 0x1D:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (var_v0 = 1, (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE))) {
            amount_1D = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            switch (((Ovl11ConditionState *) D_8006C838)->date[0]) {
            case 0: threshold_1D = 0x4E20; break;
            case 1: threshold_1D = 0x9C40; break;
            default: threshold_1D = 0xEA60; break;
            }
            return amount_1D < threshold_1D;
        } else {

            return var_v0;
        }
        break;
    case 0x1E:
        if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE)) {
            amount_1E = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
            threshold_1E = 0xEA60;
            return amount_1E < threshold_1E;
        }
        return 1;
    case 0x1F:
        if (((Ovl11ConditionState *) D_8006C838)->field_929C == 0) {
            record18.unk0 = 0x36;
            record18.unk4 = 0;
            temp_s0 = Ovl11FindRandomGridCell(&sp20[2], &sp20[3], 1, &record18, 0xE);
            temp_s0 = temp_s0 + Ovl11FindRandomGridCell(&sp20[2], &sp20[3], 0, &record18, 0xE);
            temp_s0 = temp_s0 - Ovl11FindRandomGridCell(&sp20[2], &sp20[3], 1, &record18, 0xA);
            return (temp_s0 - Ovl11FindRandomGridCell(&sp20[2], &sp20[3], 0, &record18, 0xA)) >= 0xC8;
        }
        return 0;
    case 0x20:
        var_a0_5 = 0x73;
block_172:
        return GetGameFlag(var_a0_5);
    case 0x21:
        if (ovl_11_func_800E6EE0(0x47, 1, 1, 0) != 1) {
            record28.unk0 = 0x28;
            record28.unk4 = ovl_11_func_800D5C3C(0x28);
            var_v0 = 1;
            if (Ovl11FindRandomGridCell(&sp30[0], &sp30[1], 1, &record28, 2) == 0) {
                if (Ovl11FindRandomGridCell(&sp30[0], &sp30[1], 0, &record28, 2) != 0) {
                    return 1;
                }
                return (s32) ((u8) ((Ovl11ConditionState *) D_8006C838)->field_91E1 >> 7);
            }

            return var_v0;
        }
        return 1;
    case 0x23:
        animal_cursor = (u8 *) D_8006C838;
        for (var_a2_3 = 0; var_a2_3 < 7; var_a2_3++, animal_cursor += sizeof(Ovl11ConditionAnimal)) {
            if (((Ovl11ConditionState *) animal_cursor)->animals[0].kind != 0x28 ||
                (((Ovl11ConditionState *) animal_cursor)->animals[0].flags & 0x20)) {
                return 0;
            }
        }
        for (var_a2_3 = 0; var_a2_3 < 7; var_a2_3++) {
            if (((Ovl11ConditionState *) D_8006C838)->animals[var_a2_3].amount >= 0x50) {
                return 1;
            }
        }
        return 0;
    case 0x24:
        if (D_80071CCA == 7) {
            if (GetGameFlag(0x1B) == 0) {
                return 0;
            }
        }
        return 1;
    case 0x25:
        var_s0 = 0;
        for (var_s4 = 0; var_s4 < 5; var_s4++) {
            temp_v0_2 = D_801274D0[var_s4];
            record28.unk0 = temp_v0_2;
            record28.unk4 = ovl_11_func_800D5C3C((s16) temp_v0_2);
            var_s0 = var_s0 + Ovl11FindRandomGridCell(&sp30[2], &sp30[3], 1, &record28, 0xA);
            var_s0 = var_s0 + Ovl11FindRandomGridCell(&sp30[2], &sp30[3], 0, &record28, 0xA);
        }
        return var_s0 >= 0x5A;
    case 0x27:
        var_a0 = 0xCC;
        var_a2 = 0;
        return ovl_11_func_800E6EE0(var_a0, 1, var_a2, 0);
    case 0x28:
        var_a0_10 = 0xE8;
block_338:
        var_v0_3 = GetGameFlag(var_a0_10) ^ 1;
block_339:
        return var_v0_3 == 0;
    case 0x29:
        var_a3_2 = 0;
        for (var_a1_3 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a1_3 < 5 : var_a1_3 < 10); var_a1_3++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a1_3].id == 0 &&
                !(((Ovl11ConditionState *) D_8006C838)->items[var_a1_3].flags & 0x02000000)) {
                var_a3_2++;
            }
        }
        return var_a3_2 >= 5;
    case 0x2A:
        for (var_a2_5 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a2_5 < 5 : var_a2_5 < 10); var_a2_5++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].id != 0 &&
                (((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].flags & 0x40000) &&
                ((Ovl11ConditionState *) D_8006C838)->items[var_a2_5].value < 0x32) {
                return 0;
            }
        }
        return 1;
    case 0x2F:
        return (u32) ((u16) ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind - 0x24) >= 2U;
    case 0x30:
        var_a3_3 = 0;
        for (var_a2_6 = 0; (((Ovl11ConditionState *) D_8006C838)->field_44D2 == 0 ? var_a2_6 < 5 : var_a2_6 < 10); var_a2_6++) {
            if (((Ovl11ConditionState *) D_8006C838)->items[var_a2_6].id == 0x109) {
                var_a3_3++;
            }
        }
        return var_a3_3 >= 3;
    case 0x31:
        if (((Ovl11ConditionState *) D_8006C838)->field_E641 != 0) {
            temp_s0_6 = ovl_11_func_800EEC1C(0x122, 1, 1, 0);
            temp_s0_7 = temp_s0_6 + ovl_11_func_800EEC1C(0x123, 1, 1, 0);
            temp_s0_8 = temp_s0_7 + ovl_11_func_800EEC1C(0x124, 1, 1, 0);
            temp_s0_2 = temp_s0_8 + ovl_11_func_800EEC1C(0xE8, 1, 1, 0);
            return ((temp_s0_2 + ovl_11_func_800EEC1C(0x125, 1, 1, 0)) << 0x10) > 0x20000;
        }
        return 0;
    case 0x32:
        if (D_80071CCA == 0x14) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        return 0;
    case 0x33:
        if (D_80071CCA == 7) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        return 0;
    case 0x34:
        if (D_80071CCA == 3) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        return 0;
    case 0x35:
        if (D_80071CCA == 9) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        return 0;
    case 0x36:
        if (D_80071CCA == 0xE) {
            var_v0_3 = ovl_11_func_800EF0E8(-1) ^ 1;
            return var_v0_3 == 0;
        }
        return 0;
    case 0x37:
        return ((Ovl11ConditionState *) D_8006C838)->field_E642 != 0x59;
    case 0x38:
        return ((Ovl11ConditionState *) D_8006C838)->field_E670 != 0;
    case 0x39:
        var_v0_3 = Ovl11AnyRecordFlagMatch(7, 3) ^ 1;
        return var_v0_3 == 0;
    case 0x3D:
        return ((Ovl11ConditionState *) D_8006C838)->actors[0].field14 >= 0x32;
    case 0x3E:
        var_v0 = 0;
        if ((gProgressStateFlags & 0xFC) == 0xFC) {
            temp_v1_3 = ((Ovl11ConditionWork *) &D_8007AFF0)->otherHeader->kind;
            if ((temp_v1_3 == 0xC) || (temp_v1_3 == 0x22)) {
                return 1;
            }
            return 0;
        }

        return var_v0;
    case 0x3F:
        if (D_80071CCA == 3) {
            if (GetGameFlag(0x4E) == 1) {
                return 0;
            }
            if (GetGameFlag(0x4D) == 1) {
                return 0;
            }
        }
        return 1;
    case 0x40:
        var_a0_4 = 0x4D;
        return GetGameFlag(var_a0_4) != 1;
    case 0x41:
        if (GetGameFlag(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E798 == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->field_E79A;
block_258:
                if (var_a0_11 == ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                    return 1;
                }
            }

            return var_v0;
        }
        return 0;
    case 0x42:
        if (GetGameFlag(0x4DU) != 1) {
            temp_v0_3 = (void *) ((u8 *) D_8006C838 + (((Ovl11ConditionState *) D_8006C838)->field_5492 * 0x1D4));
            var_v0 = 0;
            if (((Ovl11ConditionState *) temp_v0_3)->actors[0].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) temp_v0_3)->actors[0].tile;
                if (var_a0_11 == ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                    return 1;
                }
            }

            return var_v0;
        }
        return 0;
    case 0x43:
        if (GetGameFlag(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->actors[36].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->actors[36].tile;
                if (var_a0_11 == ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                    return 1;
                }
            }

            return var_v0;
        }
        return 0;
    case 0x44:
        if (GetGameFlag(0x4DU) != 1) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_5246 == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                var_a0_11 = ((Ovl11ConditionState *) D_8006C838)->field_5248;
                if (var_a0_11 == ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                    return 1;
                }
            }

            return var_v0;
        }
        return 0;
    case 0x45:
        return (u32) (((Ovl11ConditionState *) D_8006C838)->field_E770 - 1) < 7U;
    case 0x46:
        var_v0 = 0;
        if ((u16) ((Ovl11ConditionState *) D_8006C838)->field_E770 >= 0x3CU) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5246 != ((Ovl11ConditionState *) D_8006C838)->date[1]) || (((Ovl11ConditionState *) D_8006C838)->field_5248 != ((Ovl11ConditionState *) D_8006C838)->date[2])) {
                temp_v0_4 = (void *) ((u8 *) D_8006C838 + (((Ovl11ConditionState *) D_8006C838)->field_5492 * 0x1D4));
                var_v0 = 1;
                if (((Ovl11ConditionState *) temp_v0_4)->actors[0].location == ((Ovl11ConditionState *) D_8006C838)->date[1]) {
                    if (((Ovl11ConditionState *) temp_v0_4)->actors[0].tile == ((Ovl11ConditionState *) D_8006C838)->date[2]) {
                        return 0;
                    }
                }

                return var_v0;
            }
            return 0;
        }

        return var_v0;
    case 0x47:
        return (u32) (((Ovl11ConditionState *) D_8006C838)->field_E772 - 1) < 2U;
    case 0x48:
        return (u16) ((Ovl11ConditionState *) D_8006C838)->field_E772 >= 3U;
    case 0x49:
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_E77A >= 0x1E) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E770 == 0) {
                var_v0 = 0;
                if (((Ovl11ConditionState *) D_8006C838)->field_E76E == 0) {
                    if (((Ovl11ConditionState *) D_8006C838)->field_E772 == 0) {
                        return 1;
                    }
                }
            }
        }

        return var_v0;
    case 0x4A:
        return (u16) ((Ovl11ConditionState *) D_8006C838)->field_E76E >= 0xAU;
    case 0x4B:
        if ((((Ovl11ConditionState *) D_8006C838)->actors[0].kind == 0) || (((Ovl11ConditionState *) D_8006C838)->actors[0].kind == 9)) {
            return ((Ovl11ConditionState *) D_8006C838)->field_E657 != 0;
        }
        return 0;
    case 0x4C:
        record38.unk0 = 0x31;
        record38.unk4 = ovl_11_func_800D5C3C(0x31);
        var_v0 = 1;
        if (Ovl11FindRandomGridCell(&sp40[0], &sp40[1], 1, &record38, 2) == 0) {
            record38.unk0 = 0x32;
            record38.unk4 = ovl_11_func_800D5C3C(0x32);
            var_v0 = 1;
            if (Ovl11FindRandomGridCell(&sp40[0], &sp40[1], 1, &record38, 2) == 0) {
                record38.unk0 = 0x178;
                record38.unk4 = ovl_11_func_800D5C3C(0x178);
                var_v0 = 1;
                if (Ovl11FindRandomGridCell(&sp40[0], &sp40[1], 1, &record38, 2) == 0) {
                    record38.unk0 = 0x179;
                    record38.unk4 = ovl_11_func_800D5C3C(0x179);
                    if (Ovl11FindRandomGridCell(&sp40[0], &sp40[1], 1, &record38, 2) == 0) {
                        record38.unk0 = 0x34;
                        record38.unk4 = ovl_11_func_800D5C3C(0x34);
                        return Ovl11FindRandomGridCell(&sp40[0], &sp40[1], 1, &record38, 2) != 0;
                    }
                    return 1;
                }
            }
        }

        return var_v0;
    case 0x4F:
        return gSpendableBalance >= 0x1F4;
    case 0x50:
        if (!(((Ovl11ConditionState *) D_8006C838)->field_44F8 & 0x200000)) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x7530) {
                for (var_a0_12 = 0; var_a0_12 < 37; var_a0_12++) {
                    if (((Ovl11ConditionState *) D_8006C838)->actors[var_a0_12].amount >= 50000) {
                        return 1;
                    }
                }
            }
        }
        return 0;
    case 0x51:
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x64) {
            var_v0 = 0;
            if (!(((Ovl11ConditionState *) D_8006C838)->field_44F8 & 0x02000000)) {
                if (((Ovl11ConditionState *) D_8006C838)->field_91AC == 0x106) {
                    return 1;
                }
            }
        }

        return var_v0;
    case 0x52:
        var_v0 = 0;
        if (((Ovl11ConditionState *) D_8006C838)->field_44CE == 2) {
            if (((Ovl11ConditionState *) D_8006C838)->field_5224 >= 0x1388) {
                return (u8) ((Ovl11ConditionState *) D_8006C838)->field_E6CA < 4U;
            }
            return 0;
        }

        return var_v0;
    case 0x53:
        var_s1 = 0;
        temp_a1_2 = (s16) gCalendarYear % 9;
        var_s0_2 = 0;
        switch (temp_a1_2) {
        case 0:
            var_s1 = 0x41;
            break;
        case 1:
            var_s1 = 0x47;
            break;
        case 2:
            var_s1 = 0x43;
            break;
        case 3:
            var_s1 = 0x4D;
            break;
        case 4:
            var_s0_2 = ovl_11_func_800EEC1C(0x122, 1, 1, 0);
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(0x123, 1, 1, 0);
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(0x124, 1, 1, 0);
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(0xE8, 1, 1, 0);
            var_a0_13 = 0x125;
block_305:
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(var_a0_13, 1, 1, 0);
            break;
        case 5:
            var_s0_2 = ovl_11_func_800EEC1C(0x8B, 1, 1, 0);
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(0x8C, 1, 1, 0);
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(0x8D, 1, 1, 0);
            var_a0_13 = 0xE3;
            var_s0_2 = var_s0_2 + ovl_11_func_800EEC1C(var_a0_13, 1, 1, 0);
            break;
        case 6:
            var_s1 = 0xA2;
            break;
        case 7:
            var_s1 = 0x44;
            break;
        default:
            var_s1 = 0x97;
            break;
        }
        if (var_s0_2 < 3) {
            var_v0 = 0;
            if (var_s1 != 0) {
                if (ovl_11_func_800EEC1C(var_s1, 1, 1, 0) >= 3) {
                    return 1;
                }
            }
            return var_v0;
        }
        return 1;
    case 0x54:
        var_a0_10 = 0x67;
        return GetGameFlag(var_a0_10) == 1;
    case 0x55:
        var_a0_10 = 0x68;
        return GetGameFlag(var_a0_10) == 1;
    case 0x56:
        var_a0_10 = 0x69;
        return GetGameFlag(var_a0_10) == 1;
    case 0x57:
        var_a0_10 = 0x6A;
        return GetGameFlag(var_a0_10) == 1;
    case 0x58:
        var_a0_10 = 0x6B;
        return GetGameFlag(var_a0_10) == 1;
    case 0x59:
        var_a0_10 = 0x6C;
        return GetGameFlag(var_a0_10) == 1;
    case 0x5A:
        var_a0_10 = 0x6D;
        return GetGameFlag(var_a0_10) == 1;
    case 0x5B:
        var_a0_10 = 0x6E;
        return GetGameFlag(var_a0_10) == 1;
    case 0x5C:
        var_a0_10 = 0x6F;
        return GetGameFlag(var_a0_10) == 1;
    case 0x22:
    case 0x5F:
        return (s32) (((Ovl11ConditionState *) D_8006C838)->field_91E1 >> 7);
    case 0x61:
        if ((ovl_11_func_800E9D28(0xD, 1, 0, 0) != 1) && (ovl_11_func_800E9D28(0xD, 1, 1, 0) != 1) && (ovl_11_func_800E9D28(0xD, 1, 2, 0) != 1)) {
            var_v0_3 = ovl_11_func_800E9D28(0xD, 1, 3, 0) ^ 1;
            return var_v0_3 == 0;
        }
        return 1;
    case 0x62:
        var_a0_10 = 0x2A;
        return GetGameFlag(var_a0_10) == 1;
    case 0x63:
        var_a0_10 = 0x2A;
        return GetGameFlag(var_a0_10) == 1;
    case 0x64:
        var_a0_10 = 0x27;
        return GetGameFlag(var_a0_10) == 1;
    case 0x65:
        var_a0_10 = 0x26;
        return GetGameFlag(var_a0_10) == 1;
    case 0x66:
        var_a0_10 = 0x28;
        return GetGameFlag(var_a0_10) == 1;
    case 0x67:
        var_a0_10 = 0x29;
        return GetGameFlag(var_a0_10) == 1;
    case 0x68:
        var_a0_4 = 0x38;
        if (D_80070D06 == 0) {
            return GetGameFlag(var_a0_4) != 1;
        }
        return 0;
    case 0x69:
        if (GetGameFlag(0x15FU) != 0) {
            var_a0_10 = 0x164;
            if (GetGameFlag(0x163U) != 1) {
                return GetGameFlag(var_a0_10) == 1;
            }
            return 1;
        }

        return 1;
    case 0x6A:
        var_v0 = 0;
        if (GetGameFlag(0x26U) != 1) {
            var_v0 = 0;
            if (GetGameFlag(0x27U) != 1) {
                var_v0 = 0;
                if (GetGameFlag(0x28U) != 1) {
                    var_a0_4 = 0x2A;
                    if (GetGameFlag(0x29U) != 1) {
                        return GetGameFlag(var_a0_4) != 1;
                    }
                    return 0;
                }
            }
        }

        return var_v0;
    case 0x6B:
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(1) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 7)) {
                amount_6B = ((Ovl11ConditionState *) D_8006C838)->actors[7].amount;
                threshold_6B = 0xEA60;
                return amount_6B < threshold_6B;
            }
            return 1;
        }

        return var_v0;
    case 0x6C:
        var_a0_6 = 4;
block_355:
        return ovl_11_func_800EF0E8(var_a0_6) == 2;
    case 0x6D:
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(3) == 2) {
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 0xE)) {
                amount_6D = ((Ovl11ConditionState *) D_8006C838)->actors[14].amount;
                threshold_6D = 0xEA60;
                return amount_6D < threshold_6D;
            }
            return 1;
        }

        return var_v0;
    case 0x6E:
        var_a0_6 = 0;
        return ovl_11_func_800EF0E8(var_a0_6) == 2;
    case 0x6F:
        var_v0 = 0;
        if (ovl_11_func_800EF0E8(2) == 2) {
        case 0x1A:
            if ((((Ovl11ConditionState *) D_8006C838)->field_5492 == -1) || (((Ovl11ConditionState *) D_8006C838)->field_5492 == 9)) {
                amount_6F = ((Ovl11ConditionState *) D_8006C838)->actors[9].amount;
                threshold_6F = 0xEA60;
                return amount_6F < threshold_6F;
            }
            return 1;
        }

        return var_v0;
    case 0x70:
        return ((Ovl11ConditionState *) D_8006C838)->field_929C == 0x15F;
    case 0x71:
        switch (((Ovl11ConditionWork *) &D_8007AFF0)->header->kind) {
        case 0x2A:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 0) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 7 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0x11 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0x15) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 1) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 6 ||
                    ((Ovl11ConditionState *) D_8006C838)->date[2] == 0xB) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 8) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] != 3) {
                goto no_date;
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[2] != 9) {
                goto no_date;
            }
            return 1;
        case 0x2C:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] != 1) {
                goto no_date;
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[2] != 0) {
                goto no_date;
            }
            return 1;
        case 8:
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 1) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 0x13) {
                    return 1;
                }
            }
            if (((Ovl11ConditionState *) D_8006C838)->date[1] == 2) {
                if (((Ovl11ConditionState *) D_8006C838)->date[2] == 0x14) {
                    return 1;
                }
            }
            return 0;
        default:
no_date:
            return 0;
        }
    case 0x72:
        if (Ovl11FindCalendarDate(((Ovl11ConditionState *) D_8006C838)->date[1], ((Ovl11ConditionState *) D_8006C838)->date[2]) != 0) {
            var_v0 = 0;
            if (((Ovl11ConditionState *) D_8006C838)->field_E772 != 0) {
                if (((Ovl11ConditionState *) D_8006C838)->date[1] != 0) {
date_ready:
                    return 1;
                }
                if (((Ovl11ConditionState *) D_8006C838)->date[2] != 7) {
                    goto date_ready;
                }
            }

            return var_v0;
        }
        return 0;
    case 0x73:
        var_v0 = 0;
        if (gSpendableBalance >= 0x1F4) {
            var_v0 = 0;
            if (GetGameFlag(0xE7U) == 1) {
                if (ovl_11_func_800E6EE0(0x38, 0, 2, 0) == 0) {
                    return 1;
                }
            }
        }

        return var_v0;
    case 0x74:
        temp_v1_5 = D_80071A22 - 3;
        switch (temp_v1_5) {
        case 0: var_a0_5 = 0x21; break;
        case 4: var_a0_5 = 0x22; break;
        case 6: var_a0_5 = 0x23; break;
        case 11: var_a0_5 = 0x24; break;
        case 17: var_a0_5 = 0x25; break;
        default: return 0;
        }
        goto block_172;
    case 0x75:
        var_a0_4 = 0x1B;
        return GetGameFlag(var_a0_4) != 1;
    default:
        break;
    }
    return 0;
}
