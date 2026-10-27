#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"
#include "psyq/abs.h"
#include "ovl11_record_pair_order.h"
#include "vblank_timer.h"
#include "ovl11_direction_vector.h"
#include "ovl11_movement_cell_rank.h"
#include "ovl11_movement_step.h"
#include "psyq/libgpu.h"
#include "psyq/libcd.h"
#include "psyq/libetc.h"
#include "psyq/memory.h"
#include "psyq/libgs.h"
#include "psyq/libsnd.h"
#include "psyq/libapi.h"
#include "ovl11_record_movement.h"


void Ovl11UpdateRecordProbeFlag (Recon_ovl_11_func_800D2594_A0View *arg0);
s32 Ovl11ProbeMovementPosition (Ovl11Func800BDCE8Arg1 *arg0);
s32 Ovl11ChooseRecordDirection (Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2);
void Ovl11TryRecordRegionExit (Recon_ovl_11_func_800D2594_A0View *arg0);

s32 ovl_11_func_8010ADD4(Recon_ovl_11_func_800D2594_A0View *arg0) {
    arg0->unk34 &= 0xFF7FFFFF;
    Ovl11StepRecordMovement(arg0, 0xC, 0x31);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}
