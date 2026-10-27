#include "common.h"
#include "game_types.h"
#include "message_window.h"
#include "game_flags.h"
#include "ovl11_record_position_tables.h"

s32 ovl_11_func_800E6EE0(s16 arg0, s16 arg1, s32 arg2, s32 arg3);

void ovl_11_func_8010CF58(Ovl11CF58Object *arg0) {
    if (arg0->unkB8 & 0x800) {
        if (GetMessageWindowStatus() == 2) {
            arg0->unkB8 &= ~0x800;
            ovl_11_func_800E6EE0(Ovl11Item_80071A8A.id, 1, 0, 1);
            if (GetGameFlag(2) == 0 && arg0->unk30 == 0x28 && arg0->unk26 != 6) {
                Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)arg0);
            }
        }
    } else if (arg0->unk30 == 0x28 && GetMessageWindowStatus() == 2 && GetGameFlag(2) == 0 && arg0->unk26 != 6) {
        Ovl11PlaceRecordFromTable((Ovl11TablePositionObject *)arg0);
    }
}
