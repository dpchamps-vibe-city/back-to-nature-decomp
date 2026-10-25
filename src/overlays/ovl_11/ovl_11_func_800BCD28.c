#include "common.h"
#include "scratchpad.h"
#include "sprite_rotation.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "ovl11_scene_camera_lighting.h"
#include "ovl11_vertex_blend.h"

void ovl_11_func_800DB0E4 (void);
void ovl_11_func_800DC560 (s32 arg0, s32 arg1);
void ovl_11_func_800C9D38 (void);
void ovl_11_func_800DC990 (void);
void ovl_11_func_800DCA60 (void);
void ovl_11_func_80107F18 (void);
s32 ovl_11_func_800D79E8 (s32 arg0);
void ovl_11_func_800D78D0 (void);
s32 ovl_11_func_8011DF4C (s32 arg0);
void ovl_11_func_8011E45C (void);
s32 ovl_11_func_80112FA8 (s32 arg0);
void func_8001AF70 (u16 arg0, u16 arg1);
void ovl_11_func_80111F10 (void);
void ovl_11_func_800BFEF8 (void);
/* Extra debug-hook parameters forwarded by this wrapper are unread. */
s32 ovl_11_func_800CDE3C (u8 *arg0);
s32 ovl_11_func_800CDF4C (u8 *arg0);
void ovl_11_func_80112D10(void);

void ovl_11_func_800BCD28(void) {
    u8 *temp_s1;
    struct Ovl11ResetState *state;

    Ovl11SetSceneProjection();
    Ovl11InitSceneLighting();
    Ovl11ResetSceneCameraOrbit();
    ovl_11_func_800DB0E4();
    Ovl11ResetSpriteRotations();
    Ovl11ClearSceneCameraVectors();
    if (D_801287F4 == 0xB) {
        ovl_11_func_800DC560(0, 0);
    } else {
        ovl_11_func_800DC560(1, D_801217A0);
    }
    ovl_11_func_800C9D38();
    Ovl11ResetVertexBlendPool();
    ovl_11_func_800DC990();
    ovl_11_func_800DCA60();
    ovl_11_func_80107F18();
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 1) {
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        ovl_11_func_800D79E8(0);
        ovl_11_func_800D78D0();
        SCRATCH_STACK_END();
    }
    if (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 == 6) {
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        ovl_11_func_8011DF4C(0);
        ovl_11_func_8011E45C();
        SCRATCH_STACK_END();
    }
    if (((((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x37) || (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25474 == 0x43)) && (((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 != 0x42)) {
        ovl_11_func_80112FA8(0);
    }
    if ((u32) ((u16) ((Ovl11TriggerWorkView *)&D_8007AFF0)->field_25476 - 0xC) < 2U) {
        ovl_11_func_80112D10();
    }
    func_8001AF70(0x45U, 0U);
    ovl_11_func_80111F10();
    ovl_11_func_800BFEF8();
    state = (struct Ovl11ResetState *)D_8006C838;
    temp_s1 = (u8 *)state + 0x51C8;
    state->field_51F0 = 0;
    D_801217A0 = 0;
    ovl_11_func_800CDE3C(temp_s1);
    ovl_11_func_800CDF4C(temp_s1);
    func_8001AF70(0x5EU, 0U);
    D_8012DB8C = 0;
    state->field_51FE &= ~4;
}
