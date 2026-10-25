#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"
#include "psyq/abs.h"
#include "ovl11_record_pair_order.h"


void ovl_11_func_800D12B8 (Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1, s32 arg2);
s32 func_80012CB4 (s32 arg0, s32 *arg1, s32 arg2);
void ovl_11_func_800D1E1C (Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800D0408 (s16 arg0, Recon800D0408A1View *arg1, s32 arg2);
s32 ovl_11_func_800D2DB8 (Ovl11Func800BDCE8Arg1 *arg0);
s32 ovl_11_func_800D24FC (Recon_ovl_11_func_800D2594_A0View *arg0, VECTOR position, s16 arg5, s32 arg6);
s32 ovl_11_func_800D196C (Recon_ovl_11_func_800D2594_A0View *arg0, u16 arg1, s32 arg2);
s32 ovl_11_func_800D1CFC (Recon_ovl_11_func_800D2594_A0View *arg0);
void ovl_11_func_800D2B40 (Recon_ovl_11_func_800D2594_A0View *arg0);
s32 ovl_11_func_800D2594 (Recon_ovl_11_func_800D2594_A0View *arg0, s32 arg1);

s32 ovl_11_func_8010ADD4(Recon_ovl_11_func_800D2594_A0View *arg0) {
    arg0->unk34 &= 0xFF7FFFFF;
    ovl_11_func_800D12B8(arg0, 0xC, 0x31);
    if (((*(s32 *) ((u8 *) arg0 + 0x48)) == 0) && (arg0->unk4C == 0) && ((*(s32 *) ((u8 *) arg0 + 0x50)) == 0)) {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 0;
    } else {
        (*(s16 *) ((u8 *) arg0 + 0x24)) = 1;
    }
    arg0->unk34 &= ~0x800;
    return 1;
}
