/* Interaction-record preparation and its separate selection-request path.
 * The parameter is an existing six-byte prefix, not a complete record layout;
 * preserve the raw field accesses and six-byte pointer strides below. */
#include "common.h"
#include "game_types.h"
#include "ovl11_item_table_queries.h"
#include "ovl11_direction_vector.h"
#include "ovl11_timed_sprite_effect.h"
#include "ovl11_interaction_record_prepare.h"

s32 ovl_11_func_800E6EAC(s16 arg0, s16 arg1, s16 arg2, s16 arg3);
s32 ovl_11_func_80109188(Ovl11D04D4Obj *object, s32 state);

s32 Ovl11PrepareInteractionRecord(Ovl11InteractionRecord *record) {
    s32 flags;

    if (Ovl11ItemHasFlag8100(*(s16 *)((u8 *)record + 0)) == 0) {
        return 1;
    }
    flags = *(s32 *)((u8 *)record + 0x34);
    if (flags & 0x100000) {
        if (ovl_11_func_800E6EAC(1, 0, 0, 0) == 1) {
            ovl_11_func_80109188((Ovl11D04D4Obj *)&gStageMode1Record, 0x12);
            (*(s32 *)((u8 *)record + 0x34)) = (s32)((*(s32 *)((u8 *)record + 0x34)) & 0xFFEFFFFF);
        }
        return 2;
    }
    (*(s32 *)((u8 *)record + 0x34)) = (s32)((flags | 0x20000) & ~0x2000);
    (*(s16 *)((u8 *)record + 0x26)) = 0;
    (*(s16 *)((u8 *)record + 0x28)) = 0;
    (*(s16 *)((u8 *)record + 0x2A)) = 0;
    (*(s16 *)((u8 *)record + 0x2C)) = 0;
    Ovl11SetDirectionVector(4, (Recon800D0408A1View *)(record + 0xC), 0);
    Ovl11ResetTimedSpriteEffect((s16 *)(record + 0x1C));
    return 0;
}
