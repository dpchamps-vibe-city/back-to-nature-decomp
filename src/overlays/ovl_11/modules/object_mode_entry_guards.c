/*
 * object_mode_entry_guards — reconstructed translation unit.
 *
 * Sparse object-mode entry guards and their item-response message selector;
 * preserve per-handler admission and value-update behavior.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_mode_entry_guards.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_record_position_tables.h"
#include "psyq/memory.h"
#include "ovl11_eligible_record_selection.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_cell_grid_lookup.h"

s32 Ovl11HandleObjectItemResponse(Ovl11DE68Object *arg0);

s32 Ovl11GuardObjectMode6(Ovl11DE68Object *arg0) {
    s16 sp18;
    s16 sp1A;
    s32 sp1C;
    s32 sp20;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 facing;
    s32 var_s3;
    s32 var_s5;
    s32 var_s6;
    u16 temp_v1;
    u16 counter;
    Ovl11DA04State *state;

    var_s5 = 1;
    var_s3 = 0;
    var_s6 = -1;
    (*(s32 *) ((u8 *) arg0 + 0x34)) = (s32) ((*(s32 *) ((u8 *) arg0 + 0x34)) | 2);
    sp1C = 0;
    sp20 = 0;
    counter = ((Ovl11DA04State *)D_8006C838)->field_5200;
    if (counter < 2U) {
        facing = counter;
        facing += 2;
    } else {
        facing = counter;
        facing -= 2;
    }
    (*(u16 *) ((u8 *) arg0 + 0x22)) = facing;
    if ((((Ovl11DA04State *)D_8006C838)->field_5252 != 0) && (Ovl11HandleObjectItemResponse(arg0) == 0)) {
        ((Ovl11DA04State *)D_8006C838)->field_5234 = (s32) (((Ovl11DA04State *)D_8006C838)->field_5234 & ~0x400);
        return -1;
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x30)) == 0x28) {
        if ((u16) (*(u16 *) ((u8 *) arg0 + 0xB4)) >= 2U) {
            var_s5 = 0;
            sp1C = 2;
            sp20 = gOvl11ObjectMode6Messages[arg0->unkAC] + 1;
        } else if (arg0->unkB8 & 8) {
            if (((s16) arg0->unk16 >= 0x32) && ((Ovl11FindCandidateGridCell(&sp18, &sp1A, 1, 1, 1) == 0) || (Ovl11FindCandidateGridCell(&sp18, &sp1A, 1, 0, 0) == 0) || (Ovl11FindCandidateGridCell(&sp18, &sp1A, 0, 1, 1) == 0) || (Ovl11FindCandidateGridCell(&sp18, &sp1A, 0, 0, 0) == 0) || (temp_v0 = Ovl11ChooseEligibleRecord(1, 0xF), (temp_v0 != -1)) || (temp_v0_2 = Ovl11ChooseEligibleRecord(4, 0xF), (temp_v0_2 != temp_v0)) || (Ovl11ChooseEligibleRecord(5, 0xF) != temp_v0_2))) {
                var_s3 = 1;
            }
            if (var_s3 == 1) {
                state = (Ovl11DA04State *)D_8006C838;
                if ((state->field_44BA == 3) && (state->field_44BC >= 0x1C)) {
                    if (++(*(s16 *) ((u8 *) arg0 + 0xBC)) < 6) {
                        var_s3 = 0;
                    }
                } else if (((Ovl11DA04State *)D_8006C838)->field_44BA == 0) {
                    if (++(*(s16 *) ((u8 *) arg0 + 0xBC)) < 6) {
                        var_s3 = 0;
                    }
                }
                if (var_s3 == 1) {
                    var_s5 = 0;
                    var_s6 = 0;
                    sp1C = 2;
                    sp20 = gOvl11ObjectMode6Messages[arg0->unkAC];
                }
            }
        }
    }
    if (var_s5 == 1) {
        Ovl11SelectRecordMessage(arg0, &sp1C, &sp20);
    }
    RequestMessageWindow(sp1C, sp20);
    temp_v1 = arg0->unkB8;
    if (temp_v1 & 8) {
        if ((s16) (*(u16 *) ((u8 *) arg0 + 0xBC)) < 4) {
            arg0->unkB8 = temp_v1 & 0xFFF7;
        }
    } else {
        arg0->unkB8 = temp_v1 | 8;
        if ((*(s16 *) ((u8 *) arg0 + 0xBC)) != 0) {
            ++(*(s16 *) ((u8 *) arg0 + 0xBC));
        }
    }
    if (var_s6 == -1) {
        ((Ovl11DA04State *)D_8006C838)->field_5234 = (s32) (((Ovl11DA04State *)D_8006C838)->field_5234 & ~0x400);
    }
    return var_s6;
}

s32 Ovl11GuardObjectMode5(Ovl11Mode5GuardObject *object) {
    s32 decreased;
    s32 lower;
    s32 upper;

    decreased = object->value - 3;
    object->value = decreased;
    object->flags &= ~0x800;
    if ((s16)decreased < 0) {
        lower = 0;
    } else {
        lower = decreased;
    }
    object->value = lower;
    upper = lower;
    if ((s16)upper >= 0x100) {
        upper = 0xFF;
    }
    object->value = upper;
    Ovl11StartTimedSpriteEffect((Ovl11TimedSpriteEffect *)((s32)object + 0xA8), 0x24, 0x1E);
    if (object->area == 0x28) {
        return -1;
    }
    return 0;
}

/* D_8006C838 view for this function: a u16 counter at +0x5200 and the
 * s32 flag word at +0x5234 (bit 0x400). */
typedef struct {
    /* 0x0000 */ char pad_0000[0x5200];
    /* 0x5200 */ u16 field_5200;
    /* 0x5202 */ char pad_5202[0x32];
    /* 0x5234 */ s32 field_5234;
} Ov11View8010DDC8;

s32 Ovl11GuardObjectMode13(u8 *arg0) {
    s32 ret;
    u16 v;

    ret = -1;
    *(s32 *)(arg0 + 0x34) |= 2;
    v = ((Ov11View8010DDC8 *)&D_8006C838)->field_5200;
    if (v < 2U) {
        *(s16 *)(arg0 + 0x22) = v + 2;
    } else {
        *(s16 *)(arg0 + 0x22) = v - 2;
    }
    RequestMessageWindow(0, gOvl11ObjectMode13Messages[*(u16 *)(arg0 + 0xAC)]);
    if (ret == -1) {
        ((Ov11View8010DDC8 *)&D_8006C838)->field_5234 &= ~0x400;
    }
    return -1;
}

s32 Ovl11HandleObjectItemResponse(Ovl11DE68Object *arg0) {
    s16 message;
    s16 item;
    s32 marked;
    s32 eligible;
    s32 multiplier;
    s32 alternate;
    u16 clamped;
    u16 flags;

    message = 0;
    marked = 0;
    eligible = 0;
    multiplier = 1;
    alternate = 0;
    if (arg0->unkB8 & 0x10) {
        alternate = 1;
    }
    item = ((Ovl11DE68State *)D_8006C838)->item;
    if (arg0->unkC4 == ((Ovl11DE68State *)D_8006C838)->area) {
        if (arg0->unkC6 == ((Ovl11DE68State *)D_8006C838)->room) {
            multiplier = 5;
        }
    }
    if (item == 0xA4) {
        item = ((Ovl11DE68State *)D_8006C838)->alternateItem;
        arg0->unk16 += 2;
    }
    switch (Ovl11GetItemPackedProperty(item, 0x1B)) {
    case 0:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC];
        arg0->unk16 += gOvl11ObjectItemResponseDeltas[alternate][0] * multiplier;
        if (multiplier != 1) {
            message = gOvl11ObjectLocalItemResponseMessages[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 1:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 1;
        arg0->unk16 += gOvl11ObjectItemResponseDeltas[alternate][1] * multiplier;
        if (multiplier != 1) {
            message = gOvl11ObjectLocalItemResponseMessages[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 2:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 2;
        arg0->unk16 += gOvl11ObjectItemResponseDeltas[alternate][2] * multiplier;
        if (multiplier != 1) {
            message = gOvl11ObjectLocalItemResponseMessages[arg0->unkAC];
        }
        marked = 1;
        eligible = 1;
        break;
    case 3:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 3;
        arg0->unk16 += gOvl11ObjectItemResponseDeltas[alternate][3] * multiplier;
        marked = 1;
        break;
    case 4:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 4;
        arg0->unk16 += gOvl11ObjectItemResponseDeltas[alternate][4] * multiplier;
        marked = 1;
        break;
    case 5:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 7;
        break;
    case 6:
        message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 8;
        break;
    }
    if (item >= 0xF7) {
        if (item >= 0xFA) {
            if (item == 0x100) {
                marked = 0;
                message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 6;
            }
        } else {
            marked = 1;
            message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 9;
        }
    }
    if (eligible == 1 && arg0->unkAC == 0 && message != 0) {
        flags = arg0->unkB8;
        if (!(flags & 0x400) && ((Ovl11DE68State *)D_8006C838)->event != 0 && (s16)arg0->unk16 >= 0x65 && item != 0xC1) {
            message = gOvl11ObjectItemResponseMessages[arg0->unkAC] + 0xA;
            arg0->unkB8 = flags | 0x400;
        }
    }
    clamped = arg0->unk16;
    if ((s16)arg0->unk16 >= 0x100) {
        clamped = 0xFF;
    }
    arg0->unk16 = clamped;
    if ((s16)clamped < 0) {
        clamped = 0;
    }
    arg0->unk16 = clamped;
    if (message != 0) {
        RequestMessageWindow(0, message);
        if (marked == 1) {
            arg0->unkB8 |= 0x810;
        }
        return 0;
    }
    return -1;
}
