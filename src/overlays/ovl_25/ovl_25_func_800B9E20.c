#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"


void func_80015840 (ObjectState *obj, s8 arg1);
void ovl_25_func_800BA5BC (SpriteSourceData *arg0, u16 *arg1);
void ovl_25_func_800BA758 (SpriteSourceData *arg0, u16 *arg1);

void ovl_25_func_800B9E20(void) {
    ObjectState *temp_s0;
    ObjectState *temp_s1;

    func_80015840((ObjectState *) &D_800C027C, 0);
    temp_s1 = (ObjectState *) &D_800C027C + 6;
    func_80015840(temp_s1, 1);
    temp_s0 = (ObjectState *) &D_800C027C - 0xD;
    ovl_25_func_800BA5BC((SpriteSourceData *) &D_800C027C, (u16 *) temp_s0);
    ovl_25_func_800BA758((SpriteSourceData *) temp_s1, (u16 *) temp_s0);
}
