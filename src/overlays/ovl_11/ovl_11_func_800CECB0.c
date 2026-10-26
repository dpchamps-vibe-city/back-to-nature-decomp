#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/memory.h"
#include "aged_item_pool.h"
#include "ovl11_record_pair_slots.h"
#include "ovl11_item_spawn_placement.h"

s32 ovl_11_func_800E21E4 (E21E4Object *arg0);
s32 ovl_11_func_800DEB00 (Ovl11Deb00Object *arg0);
s32 ovl_11_func_800E06E0 (Ovl11Deb00Object *arg0);
s32 ovl_11_func_80108D84 (s32 *arg0);
s32 ovl_11_func_8010B264 (Ovl11B264Object *arg0);
void ovl_11_func_800CFD9C (void);
void ovl_11_func_800D0FB4(void);

void ovl_11_func_800CECB0(void) {
    E21E4Object *var_s1;
    s32 *var_s1_2;
    s32 *var_s1_3;
    s32 var_s0;
    struct Ovl11PoolResetView *var_a1;

    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 = (s32) (((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 & 0xF7FFFFFF);
    if (!(((struct Ovl11PoolResetView *) D_8006C838)->flags & 8)) {
        for (var_s0 = 0; var_s0 < 10; var_s0++) {
            var_s1 = (E21E4Object *) &((struct Ovl11PoolResetView *) D_8006C838)->records_7AB4[var_s0];
            if ((var_s1->type != 0) || (var_s1->flags & 0x02000000)) {
                ovl_11_func_800E21E4(var_s1);
            }
        }
        for (var_s0 = 0; var_s0 < 20; var_s0++) {
            var_s1_2 = (s32 *) ((u8 *) &gTaggedRecordBase + var_s0 * 0xB8);
            if ((*(u16 *) ((u8 *) var_s1_2 + 0)) != 0) {
                ovl_11_func_800DEB00((Ovl11Deb00Object *) var_s1_2);
            }
        }
        for (var_s0 = 0; var_s0 < 20; var_s0++) {
            var_s1_3 = (s32 *) ((u8 *) &gTaggedRecordBase + var_s0 * 0xB8);
            if ((*(u16 *) ((u8 *) var_s1_3 + 0)) != 0) {
                ovl_11_func_800E06E0((Ovl11Deb00Object *) var_s1_3);
            }
        }
        var_s1_3 = (s32 *) &D_800759E4;
        if (*(u16 *) var_s1_3 != 0) {
            ovl_11_func_80108D84(var_s1_3);
        }
        var_s1_3 = (s32 *) ((u8 *) var_s1_3 + 0xF0);
        if (*(u16 *) var_s1_3 != 0) {
            ovl_11_func_8010B264((Ovl11B264Object *) var_s1_3);
        }
        Ovl11UpdateAgedItemPool();
    }
    if (((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 & 0x08000000) {
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C++;
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C =
            ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C < 0x10 ?
            ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C : 0xF;
    } else {
        ((struct Ovl11PoolResetView *) D_8006C838)->counter_523C = 0U;
    }
    gOvl11ItemSpawnRuleMask = 0;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x8000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x800000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags_44F8 &= ~0x1000000;
    ((struct Ovl11PoolResetView *) D_8006C838)->flags = (s32) (((struct Ovl11PoolResetView *) D_8006C838)->flags & ~8);
    Ovl11RollSpawnWindowLatch();
    Ovl11ResyncRecordRowPlacements(1);
    Ovl11ResyncRecordRowPlacements(0);
    ovl_11_func_800D0FB4();
    if (((struct Ovl11PoolResetView *) D_8006C838)->field_E4CA == 0) {
        ovl_11_func_800CFD9C();
    } else {
        for (var_s0 = 0; var_s0 < 3; var_s0++) {
            var_a1 = (struct Ovl11PoolResetView *) ((u8 *) D_8006C838 + var_s0 * 0xB4);
            var_a1->records_97AC[0].type = 0;
            var_a1->records_97AC[0].id = 0xFFFF;
        }
    }
}
