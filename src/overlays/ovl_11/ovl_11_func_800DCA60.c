#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "projected_primitives.h"
#include "ovl11_scene_record_visibility.h"
#include "ovl11_scene_rotation_ramps.h"
#include "ovl11_scene_animation_pair.h"
#include "ovl11_scene_animation_cycle.h"

Ovl11FuncC9D4Entry *ovl_11_func_800DC9D4 (void);
Ovl11FuncDCF10Entry *ovl_11_func_800DCF10 (void *arg0, void *arg1, void *arg2, u8 *arg3);
void ovl_11_func_800DCE98 (s32 arg0);
void func_8001AF70 (u16 arg0, u16 arg1);
s32 func_8001AF44 (u32 arg0);

typedef struct __attribute__((packed)) {
    u8 bytes[8];
} Ovl11EntryTransform;

typedef struct {
    s32 flags;
    s32 offset4;
    s32 offset8;
    s32 offsetC;
    u16 duration;
    u16 pad12;
    Ovl11EntryTransform transform[3];
} Ovl11MapEntry;

void ovl_11_func_800DCA60(void) {
    Ovl11FuncC9D4Entry *temp_v0;
    Ovl11FuncDCF10Entry *temp_v0_3;
    u8 *temp_s3;
    s32 var_s4;
    u16 temp_v0_2;
    Ovl11MapEntry *var_s1;
    u8 *base;

    var_s4 = 0xE;
    base = (u8 *)&D_8007AFF0;
    base += 0x20000;
    var_s1 = (Ovl11MapEntry *)(*(u8 **)(base + 0x5388) + 0x948);
    temp_s3 = (u8 *)D_8007AFF0 + 0x2000;
    do {
        if (!(var_s1->flags & 0x8000)) {
            temp_v0 = ovl_11_func_800DC9D4();
            if (temp_v0 != NULL) {
                *(Ovl11EntryTransform *)&temp_v0->pad[8] = var_s1->transform[0];
                *(Ovl11EntryTransform *)&temp_v0->pad[0x10] = var_s1->transform[1];
                *(Ovl11EntryTransform *)&temp_v0->pad[0x18] = var_s1->transform[2];
                InitProjectedPrimitiveSource(temp_v0, temp_s3 + var_s1->offset4);
                temp_v0_2 = (u16) var_s1->flags;
                temp_v0->unk28 = temp_v0_2;
                if (temp_v0_2 & 0x4000) {
                    temp_v0_3 = ovl_11_func_800DCF10(temp_s3 + var_s1->offset4, temp_s3 + var_s1->offset8, temp_s3 + var_s1->offsetC, &temp_v0->pad[8]);
                    (*(Ovl11FuncDCF10Entry **) ((u8 *) temp_v0 + 0x2C)) = temp_v0_3;
                    (*(s16 *) ((u8 *) temp_v0_3 + 0x2E)) = 0;
                    (*(u16 *) ((u8 *) (*(Ovl11FuncDCF10Entry **) ((u8 *) temp_v0 + 0x2C)) + 0x30)) = var_s1->duration;
                }
            }
        }
        var_s4 -= 1;
        var_s1++;
    } while (var_s4 >= 0);
    Ovl11InitSceneAnimationCycle();
    Ovl11InitSceneAnimationPair();
    Ovl11InitSceneZRotationRamp();
    Ovl11InitSceneYRotationRamp();
    Ovl11InitSceneRecordVisibilityPair();
}
