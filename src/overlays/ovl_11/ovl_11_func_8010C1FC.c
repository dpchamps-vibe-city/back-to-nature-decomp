#include "common.h"
#include "game_types.h"

void func_80015840(ObjectState *obj, u8 arg1);
u32 func_8001589C(SpriteSourceData *src);

void ovl_11_func_8010C1FC(Ovl11C7010Object *arg0) {
    u8 *record;

    record = (u8 *) &D_80075AD4;
    *(u16 *) (record + 0x22) = arg0->unk38;
    if (arg0->unk3C == 1) {
        func_80015840((ObjectState *) (record + 0x78), 0);
    }
    switch (arg0->unk3C) {
    case 2:
        func_80015840((ObjectState *) (record + 0x78), (record[0x22] + 1) & 0xFF);
        break;
    }
    if (arg0->unk3C == 4) {
        func_80015840((ObjectState *) (record + 0x78), (record[0x22] + 9) & 0xFF);
    }
    if (arg0->sprite.field_4 == (arg0->unk38 + 0x3F)) {
        func_80015840((ObjectState *) (record + 0x78), (record[0x22] + 0xD) & 0xFF);
        return;
    }
    if (func_8001589C((SpriteSourceData *) (record + 0x78)) != 0) {
        record[0x7D] = record[0x22];
    }
}
