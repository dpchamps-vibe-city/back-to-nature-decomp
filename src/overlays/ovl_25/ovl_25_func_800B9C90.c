#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, u8 arg1);
void ovl_25_func_800BA5BC (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B9C90(Ovl25ObjectRecord *arg0) {
    SpriteSourceData *record;
    s16 state;
    s32 var_a1;
    s32 timer;

    state = arg0->unk2;
    var_a1 = -1;
    switch (state) {                              /* irregular */
    case 9:
        var_a1 = 1;
        break;
    case 10:
        var_a1 = 0;
        break;
    case 11:
        var_a1 = 0;
        break;
    case 12:
        timer = arg0->unk4 - 1;
        arg0->unk4 = timer;
        var_a1 = 1;
        if ((timer << 0x10) <= 0) {
            arg0->unk2 = 0xB;
        }
        break;
    }
    record = &arg0->record0;
    if (arg0->record0.field_4 != var_a1) {
        func_80015840((ObjectState *) record, var_a1);
    }
    ovl_25_func_800BA5BC(record, arg0->source);
}
