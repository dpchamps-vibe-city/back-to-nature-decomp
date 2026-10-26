/*
 * primary_object_grid_actions — reconstructed translation unit.
 *
 * Primary-object action dispatcher, mode-selected grid footprints, held-item
 * handoff and asynchronous cell-action request state. The single shared
 * request advances one footprint index per call, then applies its score.
 * See ovl11_primary_object_grid_actions.h for external entry contracts.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_grid_actions.h"
#include "game_types.h"
#include "ovl11_cell_grid_lookup.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_cell_actions.h"
#include "screen_fade.h"
#include "pad_actuator_sequences.h"
#include "master_volume.h"
#include "sound_commands.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_halfword_triplets.h"
#include "mode6_texture_refresh.h"
#include "map_cell_texture.h"
#include "ovl11_paired_item_ids.h"
#include "area_grid_triggers.h"
#include "ovl11_primary_object_requests.h"
#include "ovl11_record_placement.h"
#include "ovl11_moving_items.h"
#include "ovl11_primary_object_state_change.h"
#include "ovl11_surface_height.h"

s32 ovl_11_func_800C9D64(void);
s32 ovl_11_func_800D806C(s16 arg0, s16 arg1, s16 arg2, s16 arg3);

void Ovl11InstallObjectCellItems(Ovl11C4A2CObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s32 temp_s3;
    u32 var_s1;

    temp_s3 = ovl_11_func_800C9D64();
    sp18.unk0 = arg0->unk100;
    sp18.unk4 = arg0->unk104;
    sp18.unk8 = arg0->unk108;
    if (temp_s3 != 2) {
        gOvl11AreaCellConverters[temp_s3](&sp18, &sp28, &sp2A);
        var_s1 = 0;
        do {
            if (Ovl11FindCellGridTag(&arg0->unk84, var_s1, arg0->unk38, &sp2C, &sp2E, 0) != 0) {
                sp2C += sp28;
                sp2E += sp2A;
                if (ovl_11_func_800D806C(sp2C, sp2E, 1, (s16) temp_s3) == 0) {
                    Ovl11TryInstallCellItem((s16) temp_s3, sp2C, sp2E, arg0->unk84);
                }
            }
            var_s1 += 1;
        } while (var_s1 < 9U);
    }
}

void Ovl11BeginCellActionRequest(s16 arg0, s16 arg1, s16 arg2, s16 arg3, s16 arg4, s32 arg5);
void ovl_11_func_800CD5BC(Ovl11CD5BCObject *arg0, s32 arg1);

s32 Ovl11BeginCellResetAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 value;
    s16 *entry;
    s32 mode;
    s32 result;

    mode = ovl_11_func_800C9D64();
    if (mode != 2) {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &gOvl11CellResetTierCounts[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        gOvl11AreaCellConverters[mode](&pos, &col, &row);
        Ovl11BeginCellActionRequest(0, (s16)mode, col, row, tier, value);
        Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
        result = 1;
    } else {
        ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x28);
        result = 0;
    }
    return result;
}

s32 Ovl11BeginCellPromotionAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s16 temp_a0;
    s32 temp_s1;
    s32 temp_v1;
    s32 var_s0;
    s32 temp_v0;

    temp_v0 = ovl_11_func_800C9D64();
    if (temp_v0 == 2) {
        ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x28);
        return 0;
    }
    pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
    temp_s1 = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
    var_s0 = gOvl11CellPromotionTierCounts[temp_s1];
    temp_a0 = *(s16 *)((u8 *)arg0 + 0x88);
    temp_s1 -= *(s16 *)((u8 *)arg0 + 0x86);
    temp_v1 = temp_a0 - var_s0;
    if (temp_a0 < var_s0) {
        var_s0 = temp_a0 - 1;
    }
    *(s16 *)((u8 *)arg0 + 0x88) = temp_v1;
    if ((s16)temp_v1 < 0) {
        *(s16 *)((u8 *)arg0 + 0x88) = 0;
    }
    gOvl11AreaCellConverters[temp_v0](&pos, &col, &row);
    Ovl11BeginCellActionRequest(1, (s16)temp_v0, col, row, temp_s1, var_s0);
    Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
    return 1;
}

s32 ovl_11_func_800F46A4(Ovl11SpawnEffectRecord *arg0);

s32 Ovl11BeginCellEffectAction(Ovl11C7010Object *arg0) {
    Ovl11CE034Position pos;
    s16 col;
    s16 row;
    s32 tier;
    s16 *entry;
    s16 value;
    s32 mode;
    s32 result;

    mode = ovl_11_func_800C9D64();
    if (mode == 2) {
        if (D_80128C54 & 0x8000) {
            if (ovl_11_func_800F46A4(D_80128C4C_dispatch) != 0) {
                ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x96);
            }
        } else {
            ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, 0x28);
        }
        result = 0;
    } else {
        pos = *(Ovl11CE034Position *)((u8 *)arg0 + 0x100);
        tier = (s16)((s16)*(u16 *)((u8 *)arg0 + 0x18) / 20);
        entry = &gOvl11CellEffectTierCounts[tier];
        tier -= *(s16 *)((u8 *)arg0 + 0x86);
        value = *entry;
        gOvl11AreaCellConverters[mode](&pos, &col, &row);
        Ovl11BeginCellActionRequest(2, (s16)mode, col, row, tier, value);
        Ovl11ChangePrimaryObjectState(arg0, 0x68, 0, -2);
        result = 1;
    }
    return result;
}

void Ovl11BeginSelectedRecordTransfer(Recon_ovl_11_func_800C7270_A0View *arg0, s32 arg1, s32 arg2, s32 arg3) {
    Recon_ovl_11_func_800C7270_A0View *var_s0;

    if (arg1 == 0) {
        var_s0 = &arg0->unk84;
    } else {
        var_s0 = (Recon_ovl_11_func_800C7270_A0View *) ((u8 *) arg0 + ((arg2 * 6) + 0x90));
    }
    if ((*(s16 *) ((u8 *) var_s0 + 0)) != 0) {
        arg0->unk6C &= 0xE7FFFFFF;
        Ovl11ChangeObjectStateDirect(arg0, 0x15, 0x47, arg3);
        Ovl11CopyHalfwordTriplet((u16 *) &arg0->unkF0, (u16 *) var_s0);
        arg0->unk6C |= 0x8000;
        PlaySoundEffect(7);
    }
}

void Ovl11SelectHeldPairChangeAnimation(Ovl11C7010Object *arg0) {
    if ((u32)(*(u16 *)((u8 *)arg0 + 0x8A) - 0x6D) < 2U) {
        PlaySoundEffect(8);
        arg0->unk6C &= 0xEFFF7FFF;
        Ovl11ChangePrimaryObjectState(arg0, 0x34, arg0->unk38 + 0x1D, -2);
    }
}

/* Replaces the one pending request. Limit is inclusive; the step after the
 * final footprint index applies the score (40 initially, 150 after success).
 * Mode selects a grid; operation 0/1/2 resets/promotes/applies an effect. */
void Ovl11BeginCellActionRequest(s16 operation, s16 mode, s16 column,
                               s16 row, s16 filter, s32 limit) {
    gOvl11CellActionProgress = 0;
    gOvl11CellActionScore = 0x28;
    gOvl11CellActionMode = mode;
    gOvl11CellActionOperation = operation;
    gOvl11CellActionColumn = column;
    gOvl11CellActionRow = row;
    gOvl11CellActionLimit = limit;
    gOvl11CellActionFilter = filter;
}

s32 Ovl11StepCellActionRequest(Ovl11CD5BCObject *arg0) {
    s16 sp18;
    s16 sp1A;

    if (gOvl11CellActionProgress <= gOvl11CellActionLimit) {
        if (Ovl11FindCellGridTag(&arg0->value, (u32) gOvl11CellActionProgress, *(s16 *) ((u8 *) arg0 + 0x38), &sp18, &sp1A, gOvl11CellActionFilter) != 0) {
            sp18 += gOvl11CellActionColumn;
            sp1A += gOvl11CellActionRow;
            if (ovl_11_func_800D806C(sp18, sp1A, 0, gOvl11CellActionMode) != 1) {
                switch (gOvl11CellActionOperation) {
                case 0:
                    if (Ovl11TryResetCellAndSpawnEffect(gOvl11CellActionMode, sp18, sp1A) == 1) {
                        gOvl11CellActionScore = 0x96;
                    } else {
                        gOvl11CellActionProgress = gOvl11CellActionLimit + 1;
                    }
                    break;
                case 1:
                    if (Ovl11TryPromoteCellPendingKind(gOvl11CellActionMode, sp18, sp1A, 0) == 1) {
                        gOvl11CellActionScore = 0x96;
                    }
                    break;
                case 2:
                    if (Ovl11TryApplyCellActionEffect(gOvl11CellActionMode, sp18, sp1A) == 1) {
                        gOvl11CellActionScore = 0x96;
                    }
                    break;
                }
            }
        }
    } else {
        ovl_11_func_800CD5BC(arg0, gOvl11CellActionScore);
        return 1;
    }
    gOvl11CellActionProgress += 1;
    return 0;
}

s32 ovl_11_func_800F482C(Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2);

void Ovl11ApplyScoredCellFootprint(Ovl11CD5BCObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s16 temp_s3;
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s4;

    var_s4 = 0x28;
    temp_s0 = ovl_11_func_800C9D64();
    sp18.unk0 = arg0->x;
    sp18.unk4 = arg0->y;
    sp18.unk8 = arg0->z;
    temp_s3 = arg0->power / 20;
    if (temp_s0 != 2) {
        gOvl11AreaCellConverters[temp_s0](&sp18, &sp28, &sp2A);
        temp_v0 = Ovl11FindCellGridTag(&arg0->value, 0U, arg0->direction, &sp2C, &sp2E, 0);
        if (temp_v0 == 1) {
            sp2C += sp28;
            sp2E += sp2A;
            if (ovl_11_func_800D806C(sp2C, sp2E, 0, (s16) temp_s0) == 0) {
                D_80128C64 += temp_v0 << temp_s3;
                if (Ovl11TryClearCellFootprintScored((s16) temp_s0, sp2C, sp2E, D_80128C64, arg0->amount) == temp_v0) {
                    var_s4 = 0x96;
                }
            }
        }
    } else if (D_80128C54 & 0x8000) {
        D_80128C64 += 1 << temp_s3;
        if (ovl_11_func_800F482C(D_80128C4C_dispatch, D_80128C64, arg0->amount) == 1) {
            var_s4 = 0x96;
        }
    }
    ovl_11_func_800CD5BC(arg0, var_s4);
}

s32 ovl_11_func_800F474C(Ovl11SpawnEffectRecord *arg0, s16 arg1, s16 arg2);

void Ovl11ApplyCellFootprint(Ovl11CD5BCObject *arg0) {
    Ovl11CE034Position sp18;
    u16 sp28;
    u16 sp2A;
    s16 sp2C;
    s16 sp2E;
    s16 temp_s3;
    s32 temp_s0;
    s32 temp_v0;
    s32 var_s4;

    var_s4 = 0x28;
    temp_s0 = ovl_11_func_800C9D64();
    sp18.unk0 = arg0->x;
    sp18.unk4 = arg0->y;
    sp18.unk8 = arg0->z;
    temp_s3 = arg0->power / 20;
    if (temp_s0 != 2) {
        gOvl11AreaCellConverters[temp_s0](&sp18, &sp28, &sp2A);
        temp_v0 = Ovl11FindCellGridTag(&arg0->value, 0U, arg0->direction, &sp2C, &sp2E, 0);
        if (temp_v0 == 1) {
            sp2C += sp28;
            sp2E += sp2A;
            if (ovl_11_func_800D806C(sp2C, sp2E, 0, (s16) temp_s0) == 0) {
                D_80128C64 += temp_v0 << temp_s3;
                if (Ovl11TryClearCellFootprint((s16) temp_s0, sp2C, sp2E, D_80128C64, arg0->amount) == temp_v0) {
                    var_s4 = 0x96;
                }
            }
        }
    } else if (D_80128C54 & 0x8000) {
        D_80128C64 += 1 << temp_s3;
        if (ovl_11_func_800F474C(D_80128C4C_dispatch, D_80128C64, arg0->amount) == 1) {
            var_s4 = 0x96;
        }
    }
    ovl_11_func_800CD5BC(arg0, var_s4);
}

void ovl_11_func_800CE034(Ovl11CE034Object *arg0, Ovl11CE034Position *arg1, s32 arg2);

void Ovl11PlaceObjectCellTexture(Ovl11CE034Object *arg0) {
    Ovl11CE034Position sp10;
    s16 sp20;
    s16 sp22;
    s32 temp_s0;
    Ovl11D124Entry *var_s0;

    temp_s0 = ovl_11_func_800C9D64();
    ovl_11_func_800CE034(arg0, &sp10, 1);
    if (temp_s0 != 2) {
        gOvl11AreaCellConverters[temp_s0](&sp10, &sp20, &sp22);
        if (temp_s0 != 0) {
            var_s0 = &gAlternateAreaGrid[sp22][sp20];
            Ovl11SetMode6CellTexture(sp20, sp22, 0);
        } else {
            var_s0 = &gMainAreaGrid[sp22][sp20];
            Ovl11SetMapCellTexture(sp20, sp22, 0);
        }
        var_s0->unk0 = *(u16 *) ((u8 *) arg0 + 0x8A);
        if ((u16)var_s0->unk0 == 0x13B) {
            var_s0->unk0 = 0x3AU;
        }
        if ((u16)var_s0->unk0 == 0x13C) {
            var_s0->unk0 = 0x3EU;
        }
        if ((u16)var_s0->unk0 == 0x13D) {
            var_s0->unk0 = 0x3FU;
        }
        var_s0->unk4 = 0;
        var_s0->unk5 = 0;
        var_s0->unk6 = 0;
        var_s0->unk2 = 0x167;
    }
}

s32 ovl_11_func_800CB114(Ovl11CB114Object *arg0, s32 *arg1, s16 arg2);

void Ovl11MoveToFreeDirection(Ovl11C498CObject *arg0) {
    VECTOR position;
    s32 result;
    u16 direction;
    u32 i;

    direction = arg0->direction;
    for (i = 0; i < 4U; i++) {
        arg0->direction = i;
        position = *(VECTOR *)arg0->position;
        ovl_11_func_800CE034((Ovl11CE034Object *)arg0,
                           (Ovl11CE034Position *)&position, 2);
        result = Ovl11ResolveSurfaceHeight((s32 *)&position, 0xC080);
        if ((result | ovl_11_func_800CB114((Ovl11CB114Object *)arg0,
                                        (s32 *)&position, 0x18)) == 0) {
            *(VECTOR *)arg0->position = position;
            return;
        }
    }
    arg0->direction = direction;
}

void ovl_11_func_800CD08C (Recon_ovl_11_func_800CD08C_A0View *arg0);
s32 ovl_11_func_800CD0F4 (Recon_ovl_11_func_800CCFF8_A0View *arg0, s32 arg1);
s32 ovl_11_func_800D58D0 (void *arg0);
void ovl_11_func_800CCF14(Ovl11TripletTailView *arg0);
void ovl_11_func_800CF308 (s16 arg0);
s32 ovl_11_func_800CD3C4 (s32 arg0);
s32 ovl_11_func_800D622C (s8 arg0, s8 arg1, s16 arg2, s32 arg3);
void Ovl11InstallObjectCellItems (Ovl11C4A2CObject *arg0);
void Ovl11FinalizePrimaryItemAction (Ovl11C6770Object *arg0);
s32 ovl_11_func_80107C80 (s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void ovl_11_func_800CD670 (s16 arg0);
s32 ovl_11_func_800F3E44 (s16 arg0);
void ovl_11_func_800CBD78 (Ovl11C7010Object *arg0);
void ovl_11_func_800CF314 (u8 *arg0, C580C_b6c4596908ca_Ovl11CF314Arg1 *arg1, C580C_b6c4596908ca_Ovl11CF314Arg2 *arg2);
s32 ovl_11_func_800CDF4C (u8 *arg0);
void Ovl11MoveToFreeDirection (Ovl11C498CObject *arg0);
void ovl_11_func_800F43CC(s32 arg0, VECTOR position, u16 arg5, s32 arg6);
void ovl_11_func_800F0FB0 (s32 arg0);
void ovl_11_func_800D666C (s16 arg0);
void ovl_11_func_800F3AF0 (u16 arg0, u16 arg1);
s32 ovl_11_func_800F2354 (s32 arg0, s32 arg1);
s32 ovl_11_func_800C8FA8 (Ovl11C8FA8Object *arg0);
s32 ovl_11_func_8010CE80 (Recon_ovl_11_func_8010CE80_A0View *arg0, s32 arg1);
void Ovl11PlaceObjectCellTexture (Ovl11CE034Object *arg0);
void ovl_11_func_800C9D98 (void);
s32 Ovl11CommitPendingItemRecord (Ovl11C7010Object *arg0);
void ovl_11_func_800CCF58 (Ovl11C7010Object *arg0);
s32 Ovl11BeginCellEffectAction (Ovl11C7010Object *arg0);
s32 Ovl11BeginCellResetAction (Ovl11C7010Object *arg0);
s32 Ovl11BeginCellPromotionAction (Ovl11C7010Object *arg0);
s32 Ovl11StepCellActionRequest (Ovl11CD5BCObject *arg0);
void Ovl11ApplyCellFootprint (Ovl11CD5BCObject *arg0);
void Ovl11ApplyScoredCellFootprint (Ovl11CD5BCObject *arg0);
s32 ovl_11_func_800F3EF0(void);

/* Action scripts use six-byte {state, animation, attributes} entries.
 * 0x270F terminates the script. Preserve the shared finish/advance tails:
 * request reset, gauge changes and script advancement have distinct gates. */
s32 Ovl11StepPrimaryObjectAction(Ovl11C580CObject *arg0) {
    s16 column;
    VECTOR position;
    Ovl11CC7CCRequest request1;
    Ovl11CC7CCRequest request2;
    Ovl11CC7CCRequest request3;
    Ovl11TripletTailView *temp_s0;
    Ovl11TripletTailView *temp_s0_2;
    Ovl11TripletTailView *temp_s0_3;
    s16 temp_s0_4;
    s16 temp_s0_5;
    s16 temp_v1_4;
    s16 var_a1_3;
    s16 var_a2_2;
    s32 var_s0;
    s32 applyCost;
    s32 waitForAction;
    s32 var_v0;
    u16 *temp_s0_6;
    u16 *var_a0_3;
    u16 temp_v0;
    s32 temp_v0_2;
    s32 status;
    u8 temp_v1;
    void *temp_a0;
    void *temp_v0_3;
    void *temp_v1_3;

    waitForAction = 0;
    if (arg0->unk6C & 0x1000) {
        temp_v0 = arg0->state;
        switch (temp_v0) {
        case 0x3D:
        case 0x3E:
        case 0x3F:
        case 0x40:
        case 0x41:
        case 0x42:
        case 0x43:
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
reset_done:
            return 0;
        case 0x52:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x15);
            arg0->timer = 0x64;
            goto negative_action;
        case 0x55:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 8);
            temp_s0 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0);
            }
            ovl_11_func_800CCF14(temp_s0);
            arg0->unk6C &= 0xFFFF7FFB;
            Ovl11ResetPrimaryObjectRequest();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x50:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CF308(0);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            Ovl11ResetPrimaryObjectRequest();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x51:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0x11);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 4);
            temp_s0_2 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x84);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0(temp_s0_2);
            }
            ovl_11_func_800CCF14(temp_s0_2);
            arg0->unk6C &= 0xFFFF7FFB;
            Ovl11ResetPrimaryObjectRequest();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4E:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 6);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4F:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 2);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            Ovl11ResetPrimaryObjectRequest();
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;
        case 0x4D:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 5);
            if (arg0->attributes & 8) {
                ovl_11_func_800D58D0((void *) ((u8 *) arg0 + 0x84));
            }
            arg0->unk6C &= ~4;
            ovl_11_func_800D622C(0, ovl_11_func_800CD3C4(0), 0, 0);
            return 0;

        case 0x53:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            PlaySoundEffect(0xE);
            Ovl11InstallObjectCellItems((Ovl11C4A2CObject *) arg0);
            status = arg0->unk6C;
            status &= 0xEFFFFFFF;
            arg0->unk6C = status;
            ovl_11_func_800CCF14((Ovl11TripletTailView *) ((u8 *) arg0 + 0x84));
            Ovl11ResetPrimaryObjectRequest();
negative_action:
            ovl_11_func_800D622C(-1, ovl_11_func_800CD3C4(0), 0, 0);
            break;
        case 0x54:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x2D, 1, -2);
            return 0;
        case 0x4C:
            arg0->state = 0x31U;
            Ovl11FinalizePrimaryItemAction((Ovl11C6770Object *) arg0);
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            arg0->unk6C &= ~0x4000;
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x3B:
            PlaySoundEffect(0x10);
            arg0->timer114 = 0;
            arg0->direction = 0U;
            arg0->unk6C |= 1;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x56U, 0, -2);
state_done:
            return 0;
        case 0x56:
            arg0->unk6C |= 0x1800;
            arg0->attributes |= 0x100;
            arg0->unk6C &= ~0x40000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x3CU, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            arg0->unk6C |= 0x40000000;
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x58, 0, gOvl11ClockMinutePeriodTwice * 0xA);
            RequestPadActuatorSequence(0, 3);
            return 0;
        case 0x58:
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x57, 1, gOvl11ClockMinutePeriodTwice * 0xA);
            return 0;
        case 0x45:
            arg0->unk6C = (arg0->unk6C | 0x10) & 0xDFFFFFFF;
            goto state_done;
        case 0x49:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            temp_s0_3 = (Ovl11TripletTailView *) ((u8 *) arg0 + 0x8A);
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D58D0(temp_s0_3);
            ovl_11_func_800CCF14(temp_s0_3);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x46:
            if ((D_80128C50 & 0x80) && (ovl_11_func_800F3EF0() != 0)) {
                PlaySoundEffect(0x13);
                SetGameFlag(7U, 1U);
            }
            arg0->unk6C &= ~0x4000;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x4B:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            ovl_11_func_80107C80(0x2F, 0U, NULL, 1);
            arg0->direction = (u16) ((arg0->direction + 2) & 3);
            ovl_11_func_800CD670(0x14);
            return 0;
        case 0x4A:
            temp_v1 = arg0->phase;
            if (temp_v1 == 3) {
                ((struct Ovl11C580CWork *)&D_8007AFF0)->mode = 0xC;
                BeginScreenFadeIncrease(0x1E, 0, 2);
                BeginMasterVolumeFadeOut(0x1E);
                arg0->unk6C |= 0x400;
                SetGameFlag(0x47U, 1U);
                return 0;
            }
            if (temp_v1 == 1) {
                SetGameFlag(0xAU, 1U);
            }
            Ovl11ChangeObjectStateDirect((Recon_ovl_11_func_800C7270_A0View *) arg0, 0x4A, arg0->phase + 1, *((s32 *) ((u8 *) &gOvl11ActionPhaseDelays + (arg0->phase * 4))));
            return 0;

        default:
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *) arg0);
            goto reset_done;
        }
        return 0;
    } else {
        status = arg0->state;
        applyCost = 0;
        switch (status) {
        case 0x6F:
            PlaySoundEffect(9);
            ((struct Ovl11C580CState *)D_8006C838)->record->flags |= 0x100;
            gOvl11RecordSpriteYOffsetIndex = 3;
            if (ovl_11_func_800F3E44(arg0->item) != 0) {
                RequestMessageWindow(3, 0x383);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x3A:
            if (arg0->unk6C & 0x80) {
                Ovl11RefreshRecordById(*(s16 *)Ovl11SavedInteractionObject, (Ovl25SetupStatePair *) ((u8 *) arg0 + 0xFC), (VECTOR *) ((u8 *) arg0 + 0x100));
            } else {
                ovl_11_func_800CF314((u8 *)&D_80075AD4, (C580C_b6c4596908ca_Ovl11CF314Arg1 *) ((u8 *) arg0 + 0xFC), (C580C_b6c4596908ca_Ovl11CF314Arg2 *) ((u8 *) arg0 + 0x100));
            }
            arg0->unk6C &= ~0x180;
            ovl_11_func_800CDF4C((u8 *) arg0);
            Ovl11MoveToFreeDirection((Ovl11C498CObject *) arg0);
            return 1;
        case 0x59:
        case 0x5A:
        case 0x5B:
        case 0x5C:
        case 0x5D:
        case 0x5E:
        case 0x5F:
        case 0x61:
        case 0x62:
        case 0x64:
        case 0x65:
        case 0x66:
        case 0x69:
        case 0x6A:
        case 0x6B:
        case 0x6E:
            temp_s0_4 = arg0->item;
            if (arg0->state == 0x61) {
                PlaySoundEffect(0x13);
            }
            if (arg0->state == 0x62) {
                ovl_11_func_800CE034((Ovl11CE034Object *)arg0, (Ovl11CE034Position *)&position, 4);
                ovl_11_func_800F43CC(0x64, position, arg0->collisionId, 0);
                PlaySoundEffect(7);
            }
            if (arg0->state == 0x6E) {
                PlaySoundEffect(0x10);
                ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) arg0->item;
                if (arg0->item == 0xA4) {
                    ((struct Ovl11C580CState *)D_8006C838)->selected = (u16) ((struct Ovl11C580CState *)D_8006C838)->fallback;
                }
                ovl_11_func_800F0FB0(4);
            }
            arg0->unk6C &= 0xEFFF3FFF;
            ovl_11_func_800D666C(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            if (arg0->state == 0x6A) {
                SetGameFlag(0x45U, 1U);
                temp_v0_2 = ((struct Ovl11C580CState *)D_8006C838)->count + 1;
                ((struct Ovl11C580CState *)D_8006C838)->count = temp_v0_2;
                if ((s16) temp_v0_2 < 2) {
                    ovl_11_func_800D622C(0, -5, 0, 0);
                }
            }
            if (arg0->state == 0x59) {
                request1.id = 0x8E;
                if (temp_s0_4 == 0x8C) {
                    request1.id = 0x8F;
                }
                if (temp_s0_4 == 0x8D) {
                    request1.id = 0x90;
                }
                if (temp_s0_4 == 0xE3) {
                    request1.id = 0xE4;
                }
                request1.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request1.id);
                return 0;
            } else if (arg0->state == 0x5A) {
                request2.id = 0x94;
                if (temp_s0_4 == 0x92) {
                    request2.id = 0x95;
                }
                if (temp_s0_4 == 0x93) {
                    request2.id = 0x96;
                }
                if (temp_s0_4 == 0xE7) {
                    request2.id = 0xE6;
                }
                request2.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request2.id);
                return 0;
            } else if (arg0->state == 0x5B) {
                request3.id = 0x88;
                if (temp_s0_4 == 0x123) {
                    request3.id = 0x89;
                }
                if (temp_s0_4 == 0x124) {
                    request3.id = 0x8A;
                }
                if (temp_s0_4 == 0xE8) {
                    request3.id = 0xE5;
                }
                if (temp_s0_4 == 0x125) {
                    request3.id = 0x8A;
                }
                request3.count = 1;
                PlaySoundEffect(7);
                Ovl11InstallPrimaryObjectRequest(&request3.id);
                return 0;
            } else if (arg0->state == 0x5C) {
                ovl_11_func_800F3AF0(1U, 0U);
            }
            Ovl11ResetPrimaryObjectRequest();
            break;
        case 0x33:
            ovl_11_func_800F2354((s32) (*(s16 *) ((u8 *) D_8006C858 + 0x1002)), 0);
            ovl_11_func_800D666C(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x34:
            arg0->kind = 1;
            if (arg0->item == 0x6E) {
                arg0->kind = 2;
            }
            arg0->slot = (u16) *((u16 *) ((u8 *) &gOvl11ItemKindSlots + (arg0->kind * 2)));
            ovl_11_func_800D666C(arg0->item);
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0x8A));
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x45:
            arg0->unk6C |= 0x10;
        default:
finish_action:
            goto advance_action;
        case 0x32:
            if ((ovl_11_func_800C8FA8((Ovl11C8FA8Object *) arg0) == 0) && (D_80128C54 & 0x400)) {
                ovl_11_func_8010CE80((Recon_ovl_11_func_8010CE80_A0View *) D_80128C4C_dispatch, 0xD);
                Ovl11ResetPrimaryObjectRequest();
            }
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0xF0));
            arg0->unk6C &= 0xFFFF7FFF;
            goto finish_action;
        case 0xA:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x15:
            if (arg0->state == 0x15) {
                arg0->unk6C &= 0xFFFF7FFF;
            }
            Ovl11ClearHalfwordTriplet((s16 *) ((u8 *) arg0 + 0xF0));
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x2F:
            PlaySoundEffect(9);
            Ovl11SpawnMovingItemRecord((Vec3 *) ((u8 *) arg0 + 0x100), (u16 *) ((u8 *) arg0 + 0x8A), (s16) arg0->direction);
            arg0->unk6C |= 0x40;
            /* fallthrough */
        case 0x9:
            if (arg0->state == 9) {
                PlaySoundEffect(9);
            }
            arg0->unk6C &= 0xE7FF3FFF;
            if (arg0->state == 9) {
                ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            }
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0x38:
            PlaySoundEffect(9);
            Ovl11PlaceObjectCellTexture((Ovl11CE034Object *) arg0);
            arg0->unk6C &= 0xE7FF3FFF;
            ovl_11_func_800CBD78((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0xC:
            if (Ovl11IsLowerPairedItemId(arg0->item) == 0) {
                temp_s0_6 = (u16 *) ((u8 *) arg0 + 0x8A);
                Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + (((s16) arg0->slot * 6) + 0xBA)), temp_s0_6);
                ovl_11_func_800C9D98();
                var_a0_3 = temp_s0_6;
            } else {
                ovl_11_func_800D666C(arg0->item);
                var_a0_3 = (u16 *) ((u8 *) arg0 + 0x8A);
            }
            Ovl11ClearHalfwordTriplet((s16 *) var_a0_3);
            arg0->unk6C &= 0xEFFFFFFF;
            Ovl11ResetPrimaryObjectRequest();
            return 0;
        case 0xD:
            arg0->unk6C |= 0x10000000;
            /* fallthrough */
        case 0x6:
        case 0xE:
        case 0x30:
            Ovl11CommitPendingItemRecord((Ovl11C7010Object *) arg0);
            if (Ovl11IsUpperPairedItemId(arg0->item) == 1) {
                Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x2FU, arg0->direction + 0x17, -2);
            }
            if (Ovl11IsLowerPairedItemId(arg0->item) != 1) {
                goto tool_done;
            }
            arg0->unk6C |= 0x10000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 0x67U, 0x1B, 0x1E);
tool_done:
            return 0;
        case 0x67:
            arg0->unk6C |= 0x10000000;
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) &gPrimaryObjectBase, 0xCU, (*(u16 *) ((u8 *) &gPrimaryObjectBase + 0x38)) + 0x1D, -2);
            arg0->unk6C &= 0xFFFF7FFF;
            return 0;
        case 0x2C:
            Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, 0x2D, 0xB1, -2);
            return 0;
        case 0x3:
        case 0x31:
            arg0->unk6C &= ~0x4000;
            Ovl11FinalizePrimaryItemAction((Ovl11C6770Object *) arg0);
            ovl_11_func_800CCF58((Ovl11C7010Object *) arg0);
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x28:
            ovl_11_func_800CCF14((Ovl11TripletTailView *) ((u8 *) arg0 + 0x8A));
            Ovl11ResetPrimaryObjectRequest();
            goto advance_action;
        case 0x17:
            if (Ovl11BeginCellEffectAction((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            goto advance_action;
        case 0x19:
            temp_v1_4 = *((struct Ovl11C580CWork *)&D_8007AFF0)->header;
            if ((temp_v1_4 == 0x37) || (temp_v1_4 == 0x43)) {
                var_s0 = 0x28;
                waitForAction = Ovl11ClassifyAreaGridCell((Ovl11CE034Object *) arg0);
                if (waitForAction != 0) {
                    var_s0 = 0x96;
                }
                ovl_11_func_800CD5BC((Ovl11CD5BCObject *)arg0, var_s0);
                applyCost = 1;
            } else {
                if (Ovl11BeginCellResetAction((Ovl11C7010Object *) arg0) == 1) {
                    applyCost = 0;
                    waitForAction = 1;
                } else {
                    applyCost = 1;
                }
            }
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            RequestPadActuatorSequence(0, 2);
            goto advance_action;
        case 0x1F:
            if (Ovl11BeginCellPromotionAction((Ovl11C7010Object *) arg0) == 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            StopSoundEffectVoice((s32) gOvl11ActiveItemActionSound);
            gOvl11ActiveItemActionSound = -1;
            goto finish_action;
        case 0x68:
            if (Ovl11StepCellActionRequest((Ovl11CD5BCObject *) arg0) != 1) {
                applyCost = 0;
                waitForAction = 1;
            } else {
                applyCost = 1;
            }
            goto finish_action;
        case 0x1D:
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            Ovl11ApplyCellFootprint((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x1B:
            ovl_11_func_800CD0F4((Recon_ovl_11_func_800CCFF8_A0View *) arg0, 0);
            Ovl11ApplyScoredCellFootprint((Ovl11CD5BCObject *) arg0);
            RequestPadActuatorSequence(0, 2);
            applyCost = 1;
            goto finish_action;
        case 0x39:
            return 1;
        }

advance_action:
        if (applyCost == 1) {
            if (waitForAction == 0) {
                Ovl11ResetPrimaryObjectRequest();
            }
            column = (s16)arg0->power / 20;
            temp_s0_5 = gOvl11ItemActionTierValues[(s16)((s16)(arg0->value - 1) / 5)][column];
            ovl_11_func_800D622C((s8) temp_s0_5, ovl_11_func_800CD3C4(0), 0, 0);
        }
        var_v0 = 0;
        if (waitForAction == 1) {
            return var_v0;
        }
        {
            temp_v0_3 = *(void **) ((u8 *) arg0 + 0x4C);
            temp_a0 = (void *) ((u8 *) temp_v0_3 + 6);
            if (temp_v0_3 != NULL) {
                (*(void **) ((u8 *) arg0 + 0x4C)) = temp_a0;
                if ((*(s16 *)((u8 *)temp_v0_3 + 6)) == 0x270F) {
                    Ovl11ResetPrimaryObjectRequest();
                    (*(void **) ((u8 *) arg0 + 0x4C)) = NULL;
                    goto script_finished;
                }
                if ((*(s16 *)((u8 *)temp_a0 + 4)) != 0) {
                    arg0->unk6C |= 0x1800;
                }
                temp_v1_3 = *(void **) ((u8 *) arg0 + 0x4C);
                arg0->attributes = (u16) (arg0->attributes & 0xFFEF);
                var_a1_3 = (*(s16 *)temp_v1_3);
                var_a2_2 = (*(s16 *)((u8 *)temp_v1_3 + 2));
                Ovl11ChangePrimaryObjectState((Ovl11C7010Object *) arg0, var_a1_3, (s32) var_a2_2, -2);
                return 0;
            }
script_finished:
            var_v0 = 1;
            return var_v0;
        }
    }
}

s32 ovl_11_func_800D58D0(void *arg0);
s32 ovl_11_func_800D6A94(s16 arg0);

void Ovl11FinalizePrimaryItemAction(Ovl11C6770Object *arg0) {
    Ovl11ItemState *temp_s1;

    temp_s1 = &arg0->item;
    ovl_11_func_800D58D0(temp_s1);
    if (arg0->item.id == 0x126) {
        arg0->unk6C |= 0x200;
    }
    ovl_11_func_800CCF14((Ovl11TripletTailView *)temp_s1);
    if (arg0->unk3C == 0x31) {
        arg0->item = D_80128C58;
        ovl_11_func_800D6A94(arg0->item.id);
        arg0->unk6C &= ~0x4000;
        arg0->unk6C |= 0x8000;
        if (arg0->item.id == 0) {
            arg0->unk6C &= ~0x10000000;
            arg0->unk6C &= ~0x8000;
        }
    } else {
        arg0->unk6C &= ~0x10000000;
        arg0->unk6C &= ~0x8000;
    }
}

s32 ovl_11_func_800D5750(s16 arg0);
s32 Ovl11BeginFlaggedItemFollowup(Ovl11C7010Object *arg0);

s32 Ovl11CommitPendingItemRecord(Ovl11C7010Object *arg0) {
    u16 *item;
    s32 index;

    if (arg0->unk3C == 0xE) {
        arg0->unk6C |= 0x08000000;
    } else {
        arg0->unk6C |= 0x10000000;
    }
    arg0->unk6C &= 0xFFDFFFFF;
    if (arg0->unk3C == 0x30) {
        Ovl11ChangePrimaryObjectState(arg0, 0x31, 5, -2);
        PlaySoundEffect(0x4A);
        arg0->unk6C |= 0x8000;
        return 1;
    }
    index = ovl_11_func_800D5750(*(s16 *) ((u8 *) arg0 + 0xF0));
    if (index == 2) {
        index = 1;
    }
    item = (u16 *) ((u8 *) arg0 + 0xF0);
    Ovl11CopyHalfwordTriplet((u16 *) ((u8 *) arg0 + (index * 6 + 0x84)), item);
    Ovl11ClearHalfwordTriplet((s16 *) item);
    arg0->unk6C |= 0x8000;
    if (arg0->itemIndex == 0x66) {
        PlaySoundEffect(8);
        arg0->unk6C &= 0xEFFF7FFF;
        Ovl11ChangePrimaryObjectState(arg0, 0x33, arg0->unk38 + 0x1D, -2);
        return 1;
    }
    if (Ovl11BeginFlaggedItemFollowup(arg0) == 0) {
        Ovl11ResetPrimaryObjectRequest();
        return 0;
    }
    return 1;
}

s32 Ovl11BeginFlaggedItemFollowup(Ovl11C7010Object *arg0) {
    if ((D_8006C858[arg0->itemIndex].u0.flags & 0x1200) == 0x1200) {
        Ovl11ChangePrimaryObjectState(arg0, 3, 5, -2);
        PlaySoundEffect(0x4A);
        return 1;
    }
    return 0;
}
