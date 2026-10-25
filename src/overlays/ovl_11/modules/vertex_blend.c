/*
 * vertex_blend — reconstructed translation unit.
 *
 * Three-slot vertex-blend descriptor pool and signed-delta interpolation into
 * a writable projected-primitive source.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_vertex_blend.h"
#include "psyq/memory.h"
#include "projected_primitives.h"

void Ovl11ResetVertexBlend(Ovl11VertexBlend *entry) {
    memset(entry, 0, 0x34);
    entry->flags = 0x8000;
}

void Ovl11ResetVertexBlendPool(void) {
    u8 *entry;
    s32 i;

    entry = gOvl11VertexBlendPool;
    for (i = 2; i >= 0; i--) {
        Ovl11ResetVertexBlend((Ovl11VertexBlend *)entry);
        entry += 0x34;
    }
}

Ovl11VertexBlend *Ovl11AllocVertexBlend(void *sourceA, void *sourceB, void *output, u8 *position) {
    Ovl11VertexBlend *entry;
    s32 i;

    entry = (Ovl11VertexBlend *)gOvl11VertexBlendPool;
    for (i = 0; i < 3; i++, entry++) {
        if (entry->flags & 0x8000) {
            Ovl11ResetVertexBlend(entry);
            entry->flags &= 0x7FFF;
            InitProjectedPrimitiveSource((ProjectedPrimitiveSource *)entry, sourceA);
            InitProjectedPrimitiveSource((ProjectedPrimitiveSource *)((u8 *)entry + 8), sourceB);
            InitProjectedPrimitiveSource((ProjectedPrimitiveSource *)((u8 *)entry + 0x10), output);
            entry->x = *(u16 *)(position + 0);
            entry->y = *(u16 *)(position + 2);
            entry->z = *(u16 *)(position + 4);
            return entry;
        }
    }
    return 0;
}

void Ovl11InterpolateVertexBlend(s32 count, Ovl11VertexBlend *blend);

void Ovl11UpdateVertexBlend(Ovl11VertexBlend *blend) {
    Ovl11BlendVertexBlock *block;

    block = blend->sourceA;
    blend->verticesA = (Ovl11BlendVertex *)((u8 *)block + (block->vertexOffset + 0xC));
    block = blend->sourceB;
    blend->verticesB = (Ovl11BlendVertex *)((u8 *)block + (block->vertexOffset + 0xC));
    block = blend->output;
    blend->verticesOut = (Ovl11BlendVertex *)((u8 *)block + (block->vertexOffset + 0xC));
    Ovl11InterpolateVertexBlend(block->vertexCount, blend);
}

void Ovl11InterpolateVertexBlend(s32 count, Ovl11VertexBlend *blend) {
    s32 i;
    s16 numerator;
    s16 denominator;
    /* Preserve the wrapped halfword difference before signed scaling. */
    Ovl11BlendVertex delta;

    numerator = blend->numerator;
    denominator = blend->denominator;
    for (i = 0; i < count; i++) {
        delta.x = blend->verticesB[i].x - blend->verticesA[i].x;
        delta.y = blend->verticesB[i].y - blend->verticesA[i].y;
        delta.z = blend->verticesB[i].z - blend->verticesA[i].z;
        blend->verticesOut[i].x = (s16)delta.x * numerator / denominator + blend->verticesA[i].x;
        blend->verticesOut[i].y = (s16)delta.y * numerator / denominator + blend->verticesA[i].y;
        blend->verticesOut[i].z = (s16)delta.z * numerator / denominator + blend->verticesA[i].z;
    }
}
