#include "common.h"
#include "game_types.h"
#include "ovl11_record_pool_refresh.h"
#include "ovl11_record_row_values.h"

void ovl_11_func_800F40A4(void);
/* This caller forwards the signed identifier; the callee leaves it unread. */
void ovl_11_func_800F5210();
void ovl_11_func_800F4390(Ovl11SpawnEffectRecord *arg0, s32 arg1);

void ovl_11_func_800F42A8(u16 arg0, s32 arg1) {
    Ovl11SpawnEffectRecord *record;
    u8 *base;
    u8 *initialBase;

    ovl_11_func_800F40A4();
    Ovl11RefreshDispatchRecordPools();
    ovl_11_func_800F5210((s16) arg0);
    Ovl11AssignPoolRowValues((s16) arg0);
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
