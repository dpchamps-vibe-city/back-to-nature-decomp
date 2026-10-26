/*
 * Scene-record transforms and submission, original ovl_11:DCBDC-DCE98.
 * Position and offset-angle writers feed the fifteen-record render walk.
 * Rendering may update a vertex blend and base angles as well as emit packets;
 * free-slot allocation and the independent animation controls live elsewhere.
 */
#include "common.h"
#include "ovl11_scene_record_render.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "gte_transform.h"
#include "projected_primitives.h"
#include "ovl11_vertex_blend.h"
#include "point_projection.h"

void Ovl11SetSceneRecordPosition(s16 index, void *position) {
    Ovl11SceneRecordRenderView *base;
    Ovl11SceneRecordRenderView *record;
    u16 *source;

    base = (Ovl11SceneRecordRenderView *)gOvl11SceneRecords;
    source = (u16 *)position;
    record = &base[index];
    record->positionX = source[0];
    record->positionY = source[1];
    record->positionZ = source[2];
}

void Ovl11SetSceneRecordRotationOffset(s16 index, void *angles) {
    Ovl11SceneRecordRenderView *base;
    Ovl11SceneRecordRenderView *record;
    u16 *source;

    base = (Ovl11SceneRecordRenderView *)gOvl11SceneRecords;
    source = (u16 *)angles;
    record = &base[index];
    record->offsetAngleX = source[0];
    record->offsetAngleY = source[1];
    record->offsetAngleZ = source[2];
}

void Ovl11DrawSceneRecords(void) {
    MATRIX matrix;
    VECTOR translation;
    SVECTOR rotation;
    Ovl11VertexBlend *blend;
    s32 index;
    u16 flags;
    Ovl11SceneRecordRenderView *record;
    u8 *primitive;

    record = (Ovl11SceneRecordRenderView *)gOvl11SceneRecords;
    for (index = 0; index < 15; index++) {
        flags = record->flags;
        if (!(flags & 0x8000)) {
            if (!(flags & 0x1000)) {
                primitive = (u8 *)record;
                if (flags & 0x4000) {
                    blend = record->blend;
                    primitive = (u8 *)blend + 0x10;
                    Ovl11UpdateVertexBlend(blend);
                }
                PushMatrix();
                CaptureGteTransform();
                matrix = GsIDMATRIX;
                /* Preserve the short-circuit projection: flag 0x80 bypasses
                 * both the call and the writes to shared projection scratch. */
                if ((record->flags & 0x80) ||
                    ((gOvl11SceneProjectionInput[0] = (s16)record->positionX,
                      gOvl11SceneProjectionInput[1] = (s16)record->positionY,
                      gOvl11SceneProjectionInput[2] = (s16)record->positionZ,
                      ProjectPointToScreen(gOvl11SceneProjectionOutput,
                          (SVECTOR *)gOvl11SceneProjectionInput),
                      (u32)(gOvl11SceneProjectionOutput[0] + 0x64) < 0x209U) &&
                     gOvl11SceneProjectionOutput[1] < 0x155 &&
                     gOvl11SceneProjectionOutput[1] >= -0x64)) {
                    translation.vx = (s16)record->positionX;
                    translation.vy = (s16)record->positionY;
                    translation.vz = (s16)record->positionZ;
                    TransMatrix(&matrix, &translation);
                    rotation.vx = record->baseAngleX + record->offsetAngleX;
                    rotation.vy = record->baseAngleY + record->offsetAngleY;
                    rotation.vz = record->baseAngleZ + record->offsetAngleZ;
                    RotMatrixZXY(&rotation, &matrix);
                    ComposeCapturedGteTransform(&matrix);
                    DrawProjectedPrimitiveBatch((void **)primitive);
                }
                PopMatrix();
                /* Culling does not suppress angle stepping. Disabled/free
                 * records skip it; the shared pause bit suppresses it too. */
                if (!(D_8006C844 & 0x08000000) && (record->flags & 0x2000)) {
                    record->baseAngleX = (u16)(record->baseAngleX + record->deltaAngleX);
                    record->baseAngleY = (u16)(record->baseAngleY + record->deltaAngleY);
                    record->baseAngleZ = (u16)(record->baseAngleZ + record->deltaAngleZ);
                }
            }
        }
        record++;
    }
}
