#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80110544(Ovl11E644Object *arg0, s16 arg1);

/* The caller supplies its nested-function static chain in entry $v0.
 * Preserve the proven entry spill and use the captured outer argument. */
void ovl_11_func_8010EE5C(Ovl11SetFieldsView *arg0, s16 arg1, s32 arg2, void *arg3) {
    s32 tmp[2];
    CAPTURE_PREV_RET(phantom);
    void *chain;

    tmp[0] = phantom;
    chain = (void *) phantom;
    if (arg0->field_30 == arg1) {
        (*(s16 *) ((u8 *) arg0 + 0xBA)) = -1;
        (*(s16 *) ((u8 *) arg0 + 0x28)) = 0;
        arg0->field_34 &= ~arg2;
        (*(u16 *) ((u8 *) arg0 + 0xB8)) = (u16) ((*(u16 *) ((u8 *) arg0 + 0xB8)) | 0x200);
        return;
    }
    arg0->field_34 |= arg2;
    ovl_11_func_80110544(**(Ovl11E644Object ***) ((u8 *) chain + 0x10), 0xE6);
}
