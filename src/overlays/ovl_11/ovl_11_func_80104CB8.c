#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "sprite_rotation.h"
#include "master_volume.h"
#include "sound_driver.h"
#include "projected_primitives.h"
#include "ovl11_vertex_blend.h"

void ovl_11_func_800BFEA4(void);
void ovl_11_func_800BFF00(void);
void ovl_11_func_800C044C(s16 arg0);
void ovl_11_func_800BFE3C(void);
void ovl_11_func_800CBEF8(void);
void ovl_11_func_800DB00C(Ovl11SceneCoordinates *arg0, Ovl11SceneCoordinates *arg1);
void func_8001E340(Ovl11SceneGrid **arg0);
void ovl_11_func_800DB0E4(void);
void ovl_11_func_800DC990(void);
void ovl_11_func_800DCA60(void);
void ovl_11_func_800DB2AC(void *arg0);
void ovl_11_func_800BC868(void);
void ovl_11_func_800BC9AC(void);
void ovl_11_func_800F0FB0(s32 arg0);
s32 ovl_11_func_800F0F50(void);
void ovl_11_func_800DBAB0(void);
s32 func_8001AF44(u32 arg0);
u32 Rand(s32 arg0);
void ovl_11_func_800CBB9C();
void func_8001AF70(u16 arg0, u16 arg1);

void ovl_11_func_80104CB8(void) {
    ((Ovl11AreaInitView *)D_8006C838)->flags &= 0xFF7FFFFF;
    ovl_11_func_800BFEA4();
    ovl_11_func_800BFF00();
    ovl_11_func_800C044C(((Ovl11AreaInitView *)D_8006C838)->field_E4C8);
    ovl_11_func_800BFE3C();
    ovl_11_func_800CBEF8();
    ((Ovl11AreaInitView *)D_8006C838)->field_4488 = 2;
    ((Ovl11FarInitView *)&D_8007AFF0)->field_2548C = 0;
    InitProjectedPrimitiveSource(gOvl11SceneGeometry, (u8 *)D_8007AFF0 + 0x2000);
    ovl_11_func_800DB00C(&D_800A00C4, &D_800A00CC);
    func_8001E340((Ovl11SceneGrid **)gOvl11SceneGeometry);
    ovl_11_func_800DB0E4();
    Ovl11ResetVertexBlendPool();
    ovl_11_func_800DC990();
    ovl_11_func_800DCA60();
    ovl_11_func_800DB2AC(D_8009F78C);
    Ovl11RegisterPoolSpriteRotations((s32)D_8009F78C);
    ovl_11_func_800BC868();
    while (IsSoundTransferComplete() == 0) {
        UpdateMasterVolumeAndSoundTransfer();
    }
    OpenSoundSequencesAndStart();
    ovl_11_func_800BC9AC();
    if (((struct Ovl11UpdateWork *)&D_8007AFF0)->field_25476 == 2) {
        ovl_11_func_800F0FB0(8);
        ovl_11_func_800F0F50();
        ovl_11_func_800DBAB0();
        ovl_11_func_800DBAB0();
    }
    if (!(((Ovl11AreaInitView *)D_8006C838)->field_4450 & 4) && !func_8001AF44(2)) {
        ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11AreaInitView *)D_8006C838)->field_51C8, Rand(4) + 0x11B, 0);
        if (((Ovl11AreaInitView *)D_8006C838)->field_44C0 >= 7) {
            func_8001AF70(0xAD, 1);
        }
    }
    BeginMasterVolumeFadeIn(10);
    BeginScreenFadeDecrease(10, 0, 2);
    ((Ovl11AreaInitView *)D_8006C838)->flags &= 0xFF7FFFFF;
}
