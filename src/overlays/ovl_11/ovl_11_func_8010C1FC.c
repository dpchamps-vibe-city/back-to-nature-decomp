#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"

void ovl_11_func_8010C1FC(Ovl11C7010Object *arg0) {
    u8 *record;

    record = (u8 *) &gStageMode2Record;
    *(u16 *) (record + 0x22) = arg0->unk38;
    if (arg0->unk3C == 1) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), 0);
    }
    switch (arg0->unk3C) {
    case 2:
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 1) & 0xFF);
        break;
    }
    if (arg0->unk3C == 4) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 9) & 0xFF);
    }
    if (arg0->sprite.field_4 == (arg0->unk38 + 0x3F)) {
        SelectSpriteAnimation((ObjectState *) (record + 0x78), (record[0x22] + 0xD) & 0xFF);
        return;
    }
    if (GetSpriteFrameControlHighBit((SpriteSourceData *) (record + 0x78)) != 0) {
        record[0x7D] = record[0x22];
    }
}
