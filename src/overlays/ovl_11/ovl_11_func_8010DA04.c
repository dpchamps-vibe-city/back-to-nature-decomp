#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "ovl11_grid_candidate_search.h"
#include "ovl11_record_position_tables.h"
#include "psyq/memory.h"
#include "ovl11_eligible_record_selection.h"

s32 ovl_11_func_8010DE68(Ovl11DE68Object *arg0);

s32 ovl_11_func_8010DA04(Ovl11DE68Object *arg0) {
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
    if ((((Ovl11DA04State *)D_8006C838)->field_5252 != 0) && (ovl_11_func_8010DE68(arg0) == 0)) {
        ((Ovl11DA04State *)D_8006C838)->field_5234 = (s32) (((Ovl11DA04State *)D_8006C838)->field_5234 & ~0x400);
        return -1;
    }
    if ((*(s16 *) ((u8 *) arg0 + 0x30)) == 0x28) {
        if ((u16) (*(u16 *) ((u8 *) arg0 + 0xB4)) >= 2U) {
            var_s5 = 0;
            sp1C = 2;
            sp20 = D_80127CF4[arg0->unkAC] + 1;
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
                    sp20 = D_80127CF4[arg0->unkAC];
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
