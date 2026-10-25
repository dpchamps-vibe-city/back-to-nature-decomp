#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "sprite_rotation.h"
#include "ovl11_action_guards.h"


void ovl_11_func_8011F52C (void);
void ovl_11_func_800BCF28 (void);
void ovl_11_func_800CD670 (s16 arg0);
s32 ovl_11_func_80107C80 (s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void func_8001AF70 (u16 arg0, u16 arg1);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 func_8001AF44 (u32 arg0);

void ovl_11_func_8011F958(void) {
    Ovl11C7010Object *temp_s0;

    ((struct Ovl11SelectionResetState *) D_8006C838)->savedSelection = (u16) D_8012855A;
    ovl_11_func_8011F52C();
    temp_s0 = (void *) ((u8 *) D_8006C838 + 0x51C8);
    ovl_11_func_800BCF28();
    temp_s0->unk6C = (s32) (temp_s0->unk6C & ~0x400);
    ovl_11_func_800CD670(0x14);
    ovl_11_func_80107C80(0x20, 0U, NULL, 1);
    ((struct Ovl11SelectionResetMode *) &D_8007AFF0)->mode = 2;
    func_8001AF70(0x76U, 1U);
}
