#include "common.h"
#include "message_window.h"

extern s32 D_800BB9F8[];
extern s16 gOvl23RecordRunWork;

void ovl_23_func_800B8084(void) {
    s32 *base;

    UpdateMessageWindow();
    base = D_800BB9F8;
    ((void (*)(s32 *))base[gOvl23RecordRunWork])(base);
}
