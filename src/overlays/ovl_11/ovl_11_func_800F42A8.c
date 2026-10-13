#include "common.h"
#include "game_types.h"

void ovl_11_func_800F40A4(void);
void ovl_11_func_800F5404(void);
/* This caller forwards the signed identifier; the callee leaves it unread. */
void ovl_11_func_800F5210();
void ovl_11_func_800F508C(s16 arg0);
void ovl_11_func_800F4390(Ovl11SpawnEffectRecord *arg0, s32 arg1);

void ovl_11_func_800F42A8(u16 arg0, s32 arg1) {
    Ovl11SpawnEffectRecord *record;
    u8 *base;
    u8 *initialBase;

    ovl_11_func_800F40A4();
    ovl_11_func_800F5404();
    ovl_11_func_800F5210((s16) arg0);
    ovl_11_func_800F508C((s16) arg0);
    initialBase = (u8 *) D_8006C838;
    base = initialBase + 0x8000;
    ovl_11_func_800F4390(*(Ovl11SpawnEffectRecord **) (base + 0x5DD0), *(s16 *) (base + 0x5DFA));
    if (arg1 == 0) {
        ovl_11_func_800F4390(*(Ovl11SpawnEffectRecord **) (base + 0x5DCC), *(s16 *) (base + 0x5DF8));
        record = *(Ovl11SpawnEffectRecord **) (base + 0x5DD4) + 1;
        record->unk4 &= 0xFFFE;
        record = *(Ovl11SpawnEffectRecord **) (base + 0x5DD4);
        record->unk4 &= 0xFFFE;
    }
}
