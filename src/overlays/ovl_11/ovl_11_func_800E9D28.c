#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "card_deal.h"
#include "ovl11_state_field_access.h"
#include "game_flags.h"

void ovl_11_func_800F4AC0 (s32 arg0);
s32 ovl_11_func_800F4B14 (void);
void ovl_11_func_800EF488 (s16 arg0);
s32 ovl_11_func_800EFA1C (s16 arg0);
void ovl_11_func_800EF6E4 ();
void ovl_11_func_801129A0 (void);
s32 ovl_11_func_80118A68 (s32 arg0);
s32 ovl_11_func_80118C6C (void);
void ovl_11_func_80103830 (void);
s32 ovl_11_func_801037DC (void);
s32 ovl_11_func_801165C8 (void);
s32 ovl_11_func_80103770 (void);
void ovl_11_func_800F0358 (void);
s32 ovl_11_func_800F0474 (s16 arg0);
void ovl_11_func_800F05C0 (void);
s32 ovl_11_func_800CFB20 (s32 arg0, s32 arg1);
void ovl_11_func_800CFC20 (void);
u16 ovl_11_func_80112160 (s32 arg0);
s32 ovl_11_func_80118C7C (s16 arg0, s16 arg1, s16 arg2);
void ClearActiveOrderingTables (void);
void ovl_11_func_800BD6A8 (s16 arg0, s16 arg1);
s32 ovl_11_func_800E9104 (s16 arg0, s16 arg1, s32 arg2, s32 arg3);
s32 Ovl11TestStateFieldCondition (s32 arg0, s32 arg1, s32 arg2);

s32 ovl_11_func_800F0380(s16 arg0);
void ovl_11_func_800F2538(s16 arg0, s16 arg1, s16 arg2);

s32 ovl_11_func_800E9D28(s16 arg0, s16 arg1, s32 arg2, s32 arg3) {
    s32 sp10;
    s32 work;
    u32 upperOffset;
    s32 poolOffset;
    s32 byteOffset;
    E9Flags2 *pool;
    s16 temp_a0_2;
    s16 temp_a0_3;
    s16 var_a0;
    s32 temp_a1;
    s32 temp_v0;
    s32 temp_v1;
    u16 var_s3;
    E9State *temp_v0_2;
    E9State *temp_v0_3;

    var_s3 = 0;
    temp_a1 = arg1 << 0x10;
    temp_v1 = (s16)(arg0 - 1);
    sp10 = 0;
    switch (temp_v1) {
    case 0:
        if (arg1 == 1) {
            ovl_11_func_800F4AC0(arg2 == 1);
            return 1;
        }
        gOvl11ValueSlots[arg2] = ovl_11_func_800F4B14();
        break;
    case 1:
        switch (arg1) {
        case 0:
            if (arg2 != 0x29) {
                var_a0 = (s16) arg2;
            } else {
                var_a0 = -1;
            }
            ovl_11_func_800EF488(var_a0);
            return 1;
        case 1:
            if (arg3 != 0) {
                arg2 = *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4)));
            }
            switch (arg2) {
            case 1: var_s3 = 0x21; break;
            case 2: var_s3 = 0x22; break;
            case 3: var_s3 = 0x23; break;
            case 4: var_s3 = 0x24; break;
            case 5: var_s3 = 0x25; break;
            case -1: return ovl_11_func_800EFA1C((s16)arg3);
            }
            return GetGameFlag(var_s3);
        case 2:
            ovl_11_func_800EF6E4((s16) arg2, (s16) arg3);
            return 1;
        case 3:
            ((E9State *)D_8006C838)->fE76C = (s16) (*((u16 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) - 1);
            goto block_204;
        case 4:
            if (arg2 != -1) {
                if (D_80071CCA == -1) {
                    *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = (s32) D_80071CCA;
                } else if (GetGameFlag(0x1BU) == 1) {
                    *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = 0;
                } else {
                    *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = 1;
                }
            }
            if (arg3 != -1) {
                *((s32 *) ((u8 *) gOvl11ValueSlots + (arg3 * 4))) = (s32) D_80071CCA;
            }
            goto block_204;
        }
        goto block_204;
    case 2:
        SetGameFlag(0x4FU, 1U);
        SetGameFlag(0x50U, 1U);
        ((E9State *)D_8006C838)->fE780 = 1;
        ((E9State *)D_8006C838)->fE76E = 0;
        ovl_11_func_801129A0();
        ovl_11_func_800F2538(2, 0x37, 2);
        for (var_s3 = 0; var_s3 < 4; var_s3++) {
            if (GetGameFlag((u16)(var_s3 + 0x21)) != 0) {
                if (((E9State *)((u8 *)D_8006C838 + ((E9SmallView *)D_8006C838)->records[var_s3].id * 0x1D4))->f99EA >= 0xC8) {
                    ovl_11_func_800F2538(2, var_s3 + 0x33, 2);
                }
            }
        }
        temp_v0 = ovl_11_func_800EFA1C(0);
        if ((temp_v0 == 1) && (((E9State *)D_8006C838)->fC79E >= 0xC8)) {
            if (((E9State *)D_8006C838)->f44BA == temp_v0) {
                var_s3 = 0x32;
                if (((E9State *)D_8006C838)->f44BC < 0x1B) {
                    var_s3 = 0x31;
                }
            } else {
                var_s3 = 0x31;
                if (((E9State *)D_8006C838)->f44BA == 0) {
                    if (((E9State *)D_8006C838)->f44BC < 0x1B) {
                        var_s3 = 0x32;
                    }
                }
            }
            ovl_11_func_800F2538(2, var_s3, 7);
            return 1;
        }
        /* Duplicate return node #205. Try simplifying control flow for better match */
        return 1;
    case 3:
        var_s3 = ((E9State *)D_8006C838)->f5492;
        temp_v0_2 = (E9State *) ((u8 *) D_8006C838 + (var_s3 * 0x1D4));
        temp_v0_2->f99EC = arg1;
        temp_v0_2->f9A06 = (u16) (temp_v0_2->f9A06 | 0x400);
        SetGameFlag(0x4DU, 0U);
        SetGameFlag(0x4AU, 0U);
        ((E9State *)D_8006C838)->fE772 = 0;
        goto block_204;
    case 4:
        switch (arg1) {                             /* switch 4; irregular */
        case 0:
            D_80071A16 = 0x64;
            SetGameFlag(0x49, 1U);
            goto block_204;
        case 1:
            ((E9State *)D_8006C838)->fE77A = 0;
            temp_v0_3 = (E9State *) ((u8 *) D_8006C838 + ((s16) ((E9State *)D_8006C838)->f5492 * 0x1D4));
            temp_v0_3->f9A06 = (u16) (temp_v0_3->f9A06 | 0x400);
            SetGameFlag(0x4BU, 1U);
            SetGameFlag(0x4C, 1U);
            goto block_204;
        }
        goto block_204;
    case 5:
        if (arg2 != 0) {
            return (D_800712BE & arg1) != 0;
        }
        ((E9State *)D_8006C838)->f4A86 = (u16) (((E9State *)D_8006C838)->f4A86 | arg1);
        goto block_204;
    case 6:
        switch (arg1) {
        case 0:
            ((E9State *)D_8006C838)->f44CC |= 0x1;
            SetGameFlag(0x67, 0);
            goto block_204;
        case 1:
            ((E9State *)D_8006C838)->f44CC |= 0x2;
            SetGameFlag(0x68, 0);
            goto block_204;
        case 2:
            ((E9State *)D_8006C838)->f44CC |= 0x4;
            SetGameFlag(0x69, 0);
            goto block_204;
        case 3:
            ((E9State *)D_8006C838)->f44CC |= 0x8;
            SetGameFlag(0x6A, 0);
            goto block_204;
        case 4:
            ((E9State *)D_8006C838)->f44CC |= 0x10;
            SetGameFlag(0x6B, 0);
            goto block_204;
        case 5:
            ((E9State *)D_8006C838)->f44CC |= 0x20;
            SetGameFlag(0x6C, 0);
            goto block_204;
        case 6:
            ((E9State *)D_8006C838)->f44CC |= 0x40;
            SetGameFlag(0x6D, 0);
            goto block_204;
        case 7:
            ((E9State *)D_8006C838)->f44CC |= 0x80;
            SetGameFlag(0x6E, 0);
            goto block_204;
        case 8:
            ovl_11_func_800E9D28(6, 4, 0, 0);
            SetGameFlag(0x6F, 0);
            return 1;
        }
block_118:
        return 0;
    case 7:
        if (arg3 == 0) {
            switch (arg1) {
            case 0:
                ovl_11_func_80118A68(0x10);
                return 1;
            case 1:
                ovl_11_func_80118A68(0x11);
                return 1;
            case 2:
                ovl_11_func_80118A68(0x12);
                return 1;
            case 3:
                ovl_11_func_80118A68(0x13);
                return 1;
            case 4:
                ovl_11_func_80118A68(0x19);
                return 1;
            case 5:
                ovl_11_func_80118A68(0x14);
                return 1;
            case 6:
                ovl_11_func_80118A68(0x15);
                return 1;
            case 7:
                ovl_11_func_80118A68(0x16);
                return 1;
            case 8:
                ovl_11_func_80118A68(0x17);
                return 1;
            case 9:
                if (arg2 == 0) {
                    ovl_11_func_80118A68(0x18);
                    return 1;
                }
                return GetGameFlag(0x38U);
            case 10:
                ovl_11_func_80118A68(0xE);
                return 1;
            case 11:
                ovl_11_func_80118A68(0x1C);
                return 1;
            }
        } else {
            return ovl_11_func_80118C6C() == 0;
        }
        return 0;
    case 8:
        SetGameFlag(0x66, 1U);
        goto block_204;
    case 9:
        ovl_11_func_800F2538(arg1, (s16) arg2, (s16) arg3);
        return 1;
    case 10:
        ((E9State *)D_8006C838)->f44F8 = (s32) (((E9State *)D_8006C838)->f44F8 | 2);
        goto block_204;
    case 11:
        switch (arg1) {
        case 0:
            DealCardGameTable();
            return 1;
        case 1:
            ((E9State *)D_8006C838)->fE514 = (s16) arg2;
            goto block_204;
        case 2:
            if (arg2 != 0) {
                ovl_11_func_80103830();
                return 1;
            }
            return ovl_11_func_801037DC();
        case 3:
            if (arg2 != 0) {
                ovl_11_func_80118A68(0x1A);
                return 1;
            }
            return ovl_11_func_801165C8();
        case 4:
            return ovl_11_func_80103770();
        case 5:
            switch (arg2) {
            case 0:
                if (((E9State *)D_8006C838)->fE614 == 1) {
                    ((E9State *)D_8006C838)->f92D0 = (s32) (((E9State *)D_8006C838)->f92D0 | 0x40);
                }
                return ((E9State *)D_8006C838)->fE614;
            case 1:
                return ((E9State *)D_8006C838)->fE624;
            case 2: {
                gOvl11ValueSlots[arg3] = ((E9State *)D_8006C838)->fE62A;
                return ((E9State *)D_8006C838)->fE62C;
            }
            case 3: {
                gOvl11ValueSlots[arg3] = ((E9State *)D_8006C838)->fE634;
                return ((E9State *)D_8006C838)->fE630;
            }
            case 4: {
                gOvl11ValueSlots[arg3] = ((E9State *)D_8006C838)->fE63C;
                return ((E9State *)D_8006C838)->fE638;
            }
            }
            break;
        case 6:
            if (arg2 == 0) {
                return D_80070CC4 == 0xFF;
            }
            D_80070CC4 = 0;
        case 7:
            for (var_s3 = 0; var_s3 < 10; var_s3++) {
                if (((E9RecordsView *)D_8006C838)->records[var_s3].flags & 0x20000) {
                    ((E9State *)D_8006C838)->fE61C = ((E9RecordsView *)D_8006C838)->records[var_s3].id;
                    ((E9State *)D_8006C838)->fE620 = &((E9RecordsView *)D_8006C838)->records[var_s3];
                    return 1;
                }
            }
            return 0;
        case 8:
            for (var_s3 = 0; var_s3 < 10; var_s3++) {
                if (((E9Flags *) &D_80074320)[var_s3].flags & 0x20000) {
                    ((E9Flags *) &D_80074320)[var_s3].flags |= 0x40;
                    return 1;
                }
            }
            return 0;
        case 9:
            ((E9State *)D_8006C838)->fE628 = -1U;
            ((E9State *)D_8006C838)->fE62A = -1;
            ((E9State *)D_8006C838)->fE62C = 0;
            goto block_204;
        case 10:
            if (arg3 == 0) {
                ((E9State *)D_8006C838)->fE628 = (u16) arg2;
            } else {
                ((E9State *)D_8006C838)->fE628 = (u16) *((u16 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4)));
            }
            goto block_204;
        case 11:
            *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = (s32) (s16) ((E9State *)D_8006C838)->fE628;
            goto block_204;
        case 12:
            temp_a0_2 = ((E9State *)D_8006C838)->fE618;
            ((E9State *)D_8006C838)->fE618 = 0;
            ((E9State *)D_8006C838)->f5228 = (s32) temp_a0_2;
            goto block_204;
        }
        return 0;
    case 12:
        switch (arg1) {
        case 0:
            ((E9State *)D_8006C838)->fC |= 0x08000000;
            ovl_11_func_800F0358();
            return 1;
        case 1:
            return ovl_11_func_800F0380((s16)arg2);
        case 2:
            return ovl_11_func_800F0474((s16)arg2);
        case 3:
            ((E9State *)D_8006C838)->fC &= 0xF7FFFFFF;
            ovl_11_func_800F05C0();
            return 1;
        }
        return 0;
    case 13:
        switch (arg1) {                             /* switch 10; irregular */
        case 0:
            return ovl_11_func_800CFB20(1 << arg2, 1 << arg3);
        case 1:
            ovl_11_func_800CFC20();
            break;
        default:
            return 0;
        }
    case 14:
        switch (arg1) {
        case 0:
            ((E9State *)D_8006C838)->f44F8 |= 0x400;
            break;
        case 1:
            ((E9State *)D_8006C838)->f44F8 |= 0x400000;
            break;
        default:
            return 0;
        }
    case 15:
        poolOffset = 0x1F0;
        for (var_s3 = 0; var_s3 < 20; var_s3++) {
            byteOffset = var_s3 * 0xB8;
            work = (s32)D_8006C838;
            upperOffset = 0x8000;
            pool = (E9Flags2 *)((u8 *)D_80074838 + poolOffset);
            if (((E9Flags2 *)((u8 *)pool + byteOffset))->flags & 0x400) {
                if (((E9Health *)(work + byteOffset + upperOffset))->health >= 0xF0) {
                    ((E9Flags2 *)((u8 *)pool + byteOffset))->flags |= 0x40;
                    return 1;
                }
                return 0;
            }
        }
        return 0;
    case 16:
        if (arg2 != -1) {
            *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = (s32) ((E9State *)D_8006C838)->f30->f2;
        }
        return arg1 == ((E9State *)D_8006C838)->f30->f2;
    case 17:
        if (arg3 == 0) {
            var_s3 = arg1 & 0xFFFF;
        } else {
            var_s3 = *((u16 *) ((u8 *) gOvl11ValueSlots + (arg1 * 4)));
        }
        if (arg2 != 0) {
            ((E9State *)D_8006C838)->f99D8 = var_s3;
        } else if (((E9State *)D_8006C838)->f99D8 == var_s3) {
            return 1;
        }
        return 0;
    case 18:
        if (arg1 != 0) {
            SetGameFlag(8, 1U);
            goto block_204;
        }
        return GetGameFlag(8U) != 1;
    case 19:
        switch (arg1) {                             /* switch 12; irregular */
        case 0:
            return ovl_11_func_80112160(0);
        case 1:
            gOvl11ValueSlots[arg2] = ovl_11_func_80112160(1);
            return 1;
        case 2:
            ovl_11_func_80112160(2);
            return 1;
        }
    case 20:
        var_s3 = ovl_11_func_80118C7C(arg1, 1, -1);
        return var_s3 != 0;
    case 21:
        if (((E9State *)D_8006C838)->f523A < 0xF) {
            ((E9State *)D_8006C838)->f523A = (s16) ((u16) ((E9State *)D_8006C838)->f523A + 1);
        }
        goto block_204;
    case 22:
        ((E9State *)D_8006C838)->f44F8 |= 0x02000000;
        goto block_204;
    case 23:
        DrawSync(0);
        ClearActiveOrderingTables();
        temp_a0_3 = ((E9Other *)&D_8007AFF0)->f25388->fC;
        if (temp_a0_3 != 0x3E7) {
            ovl_11_func_800BD6A8(temp_a0_3, 0);
            return 1;
        }
        /* Duplicate return node #205. Try simplifying control flow for better match */
        return 1;
    case 24:
        ((E9State *)D_8006C838)->f44F8 |= 0x200000;
        goto block_204;
    case 25:
        *((s32 *) ((u8 *) gOvl11ValueSlots + (arg2 * 4))) = (s32) ((E9State *) ((u8 *) D_8006C838 + (arg1 * 0x1D4)))->f9A6C;
        goto block_204;
    case 26:
        ((E9State *)D_8006C838)->f51C0 = (s32) (((E9State *)D_8006C838)->f51C0 | 0x800000);
        goto block_204;
    case 27:
        if (arg1 == 0) {
            ovl_11_func_800E9104(0, 0x54, 0, 0xB);
            ((E9State *)D_8006C838)->f4450 = (s32) (((E9State *)D_8006C838)->f4450 | 0x1000);
            ((E9State *)D_8006C838)->f5234 = (s32) (((E9State *)D_8006C838)->f5234 | 0x800000);
        } else {
            ((E9State *)D_8006C838)->f4450 = (s32) (((E9State *)D_8006C838)->f4450 & ~0x1000);
        }
        goto block_204;
    case 28:
        if (arg1 != -1) {
            *((s32 *) ((u8 *) gOvl11ValueSlots + (arg1 * 4))) = (s32) ((E9State *)D_8006C838)->fE77C;
        }
        return Ovl11TestStateFieldCondition((s32) ((E9State *)D_8006C838)->fE77C, arg2, arg3);
    case 29:
        Ovl11ApplyStateFieldOperation((s32 *) ((u8 *) gOvl11ValueSlots + (arg1 * 4)), arg2 / 2, arg3);
        return 1;
    case 30:
        sp10 = (s32) ((E9State *)D_8006C838)->f91C2;
        Ovl11ApplyStateFieldOperation(&sp10, arg2, arg3);
        if (sp10 >= 0x100) {
            sp10 = 0xFF;
        }
        if (sp10 < 0) {
            sp10 = 0;
        }
        ((E9State *)D_8006C838)->f91C2 = (s16) (u16) sp10;
        goto block_204;
    default:
        return 0;
    }
block_204:
    return 1;
}
