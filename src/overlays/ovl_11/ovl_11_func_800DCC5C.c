#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_transform.h"
#include "projected_primitives.h"

typedef struct {
    u8 pad0[0xC];
    s32 unkC;
    s32 unk10;
} M2C_74497bb9f79c_DCFECRef;

typedef struct {
    u16 vx;
    u16 vy;
    u16 vz;
    u16 pad;
} M2C_74497bb9f79c_DCFECVec;

typedef struct {
    M2C_74497bb9f79c_DCFECRef *unk0;
    u8 pad4[4];
    M2C_74497bb9f79c_DCFECRef *unk8;
    u8 padC[4];
    M2C_74497bb9f79c_DCFECRef *unk10;
    u8 pad14[0xC];
    M2C_74497bb9f79c_DCFECVec *unk20;
    M2C_74497bb9f79c_DCFECVec *unk24;
    M2C_74497bb9f79c_DCFECVec *unk28;
} M2C_74497bb9f79c_DCFECArg;

void ovl_11_func_800DCFEC (M2C_74497bb9f79c_DCFECArg *arg0);
s32 func_8001DFD4 (s32 *arg0, SVECTOR *arg1);

void ovl_11_func_800DCC5C(void) {
    MATRIX matrix;
    VECTOR translation;
    SVECTOR rotation;
    M2C_74497bb9f79c_DCFECArg *temp_a0;
    s32 var_s4;
    u16 temp_v1;
    u8 *var_s0;
    u8 *var_s3;

    var_s0 = gOvl11SceneRecords;
    for (var_s4 = 0; var_s4 < 15; var_s4++) {
        temp_v1 = *(u16 *) ((u8 *) var_s0 + 0x28);
        if (!(temp_v1 & 0x8000)) {
          if (!(temp_v1 & 0x1000)) {
            var_s3 = var_s0;
            if (temp_v1 & 0x4000) {
                temp_a0 = *(M2C_74497bb9f79c_DCFECArg **) ((u8 *) var_s0 + 0x2C);
                var_s3 = (u8 *) ((u8 *) temp_a0 + 0x10);
                ovl_11_func_800DCFEC(temp_a0);
            }
            PushMatrix();
            CaptureGteTransform();
            matrix = GsIDMATRIX;
            if (((*(u16 *) ((u8 *) var_s0 + 0x28)) & 0x80) || ((D_80128DF8[0] = (s16) (*(u16 *) ((u8 *) var_s0 + 8)), D_80128DF8[1] = (s16) (*(u16 *) ((u8 *) var_s0 + 0xA)), D_80128DF8[2] = (s16) (*(u16 *) ((u8 *) var_s0 + 0xC)), func_8001DFD4(D_80128E00, (SVECTOR *)D_80128DF8), (((u32) (D_80128E00[0] + 0x64) < 0x209U) != 0)) && (D_80128E00[1] < 0x155) && (D_80128E00[1] >= -0x64))) {
                translation.vx = (s16) (*(u16 *) ((u8 *) var_s0 + 8));
                translation.vy = (s16) (*(u16 *) ((u8 *) var_s0 + 0xA));
                translation.vz = (s16) (*(u16 *) ((u8 *) var_s0 + 0xC));
                TransMatrix(&matrix, &translation);
                rotation.vx = (*(u16 *) ((u8 *) var_s0 + 0x10)) + (*(u16 *) ((u8 *) var_s0 + 0x20));
                rotation.vy = (*(u16 *) ((u8 *) var_s0 + 0x12)) + (*(u16 *) ((u8 *) var_s0 + 0x22));
                rotation.vz = (*(u16 *) ((u8 *) var_s0 + 0x14)) + (*(u16 *) ((u8 *) var_s0 + 0x24));
                RotMatrixZXY(&rotation, &matrix);
                ComposeCapturedGteTransform(&matrix);
                DrawProjectedPrimitiveBatch((void **) var_s3);
            }
            PopMatrix();
            if (!(D_8006C844 & 0x08000000) && ((*(u16 *) ((u8 *) var_s0 + 0x28)) & 0x2000)) {
                (*(u16 *) ((u8 *) var_s0 + 0x10)) = (u16) ((*(u16 *) ((u8 *) var_s0 + 0x10)) + (*(u16 *) ((u8 *) var_s0 + 0x18)));
                (*(u16 *) ((u8 *) var_s0 + 0x12)) = (u16) ((*(u16 *) ((u8 *) var_s0 + 0x12)) + (*(u16 *) ((u8 *) var_s0 + 0x1A)));
                (*(u16 *) ((u8 *) var_s0 + 0x14)) = (u16) ((*(u16 *) ((u8 *) var_s0 + 0x14)) + (*(u16 *) ((u8 *) var_s0 + 0x1C)));
            }
          }
        }
        var_s0 += 0x30;
    }
}
