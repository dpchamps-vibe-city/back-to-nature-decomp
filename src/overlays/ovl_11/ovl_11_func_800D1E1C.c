#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"

void ovl_11_func_800D1E1C(Recon_ovl_11_func_800D2594_A0View *arg0) {
    VECTOR position;
    void *collision;
    s32 result;

    position.vx = *(s32 *) ((u8 *) arg0 + 0x38);
    position.vy = arg0->unk3C;
    position.vz = *(s32 *) ((u8 *) arg0 + 0x40);
    if (Ovl11QueryCollisionBudgeted((s32 *) &position,
                           *(Ovl11CollisionTag *) ((u8 *) arg0 + 0x2E),
                           0, 0, &collision, &result, (s32) arg0, 0x10) != 0) {
        arg0->unk34 |= 0x200000;
        return;
    }
    arg0->unk34 &= 0xFFDFFFFF;
}
