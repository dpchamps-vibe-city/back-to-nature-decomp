#include "common.h"
#include "game_types.h"
#include "scratchpad.h"
#include "psyq/libgte.h"
#include "ovl11_collision_query_budget.h"
#include "gte_scene_state.h"

void ovl_11_func_800C06B0 (void);
void ovl_11_func_800C0688 (void);
void ovl_11_func_800C3D70 (void);
void ovl_11_func_800BFD1C (void);
void ovl_11_func_800F65B8 (void);
void ovl_11_func_80113818 (void);
void ovl_11_func_800DB674 (void);
void ovl_11_func_800C401C (void);
void ovl_11_func_80107F58 (void);
void ovl_11_func_8011F114 (void);
s32 ovl_11_func_800CD9A0 (Ovl11C7010Object *arg0);
void ovl_11_func_800CAADC (void);
void ovl_11_func_800DB5CC (void);
void ovl_11_func_8010726C (void);
void ovl_11_func_800C056C ();
void ovl_11_func_801209D4 (void);
void ovl_11_func_800DB78C (void);
void ovl_11_func_800DCC5C (void);
void ovl_11_func_800DB904 (void);
void ovl_11_func_800CA24C ();
void ovl_11_func_800CCB90 (void);
void ovl_11_func_800DC148 (void);
s32 ovl_11_func_800F0F50 (void);
void ovl_11_func_800DBAB0 (void);
void ovl_11_func_800C30AC (void);
void ovl_11_func_8010C7F0 (void);
void ovl_11_func_800CE834 (void);
s32 ovl_11_func_801189C8 (void);
void ovl_11_func_800F3FB4 (void);
void ovl_11_func_801212C4 (void);
void ovl_11_func_800F2400 (void);
s32 func_800225C4 (void);
void ovl_11_func_800DE46C (void);

void ovl_11_func_800C162C(void) {
    D_80128B60 = 2;
    ovl_11_func_800C06B0();
    ovl_11_func_800C0688();
    ovl_11_func_800C3D70();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    Ovl11ResetCollisionQueryCount();
    ovl_11_func_800BFD1C();
    ovl_11_func_800F65B8();
    ovl_11_func_80113818();
    ovl_11_func_800DB674();
    ovl_11_func_800C401C();
    ovl_11_func_80107F58();
    ovl_11_func_8011F114();
    if (((struct Ovl11FrameWork *) &D_8007AFF0)->mode == 0) {
        D_80128B60 = ovl_11_func_800CD9A0((Ovl11C7010Object *) &D_80071A00);
    }
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    if (D_80128B60 == 2) {
        ovl_11_func_800CAADC();
    } else {
        ((struct Ovl11FrameState *) D_8006C838)->field_52D8 = 0;
        ((struct Ovl11FrameState *) D_8006C838)->field_52E0 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253A8 = 0;
        ((struct Ovl11FrameWork *) &D_8007AFF0)->field_253AA = 0;
    }
    ovl_11_func_800DB5CC();
    ovl_11_func_8010726C();
    ovl_11_func_800C056C(1);
    ovl_11_func_801209D4();
    SCRATCH_STACK_END();
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    ovl_11_func_800DB78C();
    ApplyGteSceneState();
    SCRATCH_STACK_END();
    if (!(((struct Ovl11FrameState *) D_8006C838)->flags & 0x800000)) {
        ovl_11_func_800DCC5C();
        ovl_11_func_800DB904();
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        ovl_11_func_800CA24C((Ovl11SpritePositionView *) ((struct Ovl11FrameState *) D_8006C838)->position, 1);
        ovl_11_func_800CCB90();
        ovl_11_func_800DC148();
        SCRATCH_STACK_END();
    }
    SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
    ovl_11_func_800F0F50();
    ovl_11_func_800DBAB0();
    ovl_11_func_800C30AC();
    ovl_11_func_8010C7F0();
    ovl_11_func_800CE834();
    ovl_11_func_801189C8();
    ovl_11_func_800F3FB4();
    ovl_11_func_801212C4();
    ovl_11_func_800F2400();
    func_800225C4();
    ovl_11_func_800DE46C();
    SCRATCH_STACK_END();
}
