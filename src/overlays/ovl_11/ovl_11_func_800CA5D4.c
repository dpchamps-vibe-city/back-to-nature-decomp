#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, u8 arg1);
u32 func_8001589C (SpriteSourceData *src);

void ovl_11_func_800CA5D4(Ovl11C7010Object *arg0) {
    ObjectState *temp_s1;
    u8 *temp_s0;

    temp_s0 = Ovl11SavedInteractionObject;
    *(u16 *) (temp_s0 + 0x22) = arg0->unk38;
    if (arg0->unk3C == 1) {
        func_80015840((ObjectState *) ((u8 *) temp_s0 + 0x78), 0);
    }
    temp_s1 = (ObjectState *) ((u8 *) temp_s0 + 0x78);
    if (arg0->unk3C == 2) {
        func_80015840(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
    }
    if (arg0->unk3C == 4) {
        func_80015840(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
    }
    if (arg0->sprite.field_4 == (arg0->unk38 + 0x3F)) {
        func_80015840(temp_s1, (temp_s0[0x22] + 1) & 0xFF);
        return;
    }
    if (func_8001589C((SpriteSourceData *) temp_s1) != 0) {
        temp_s0[0x7D] = temp_s0[0x22];
    }
}
