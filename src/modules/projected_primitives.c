/*
 * Projected model primitives, exe:8001BFA8..8001D86C.
 *
 * Source descriptors, four-plane point rejection, GTE polygon submission
 * and optional rotation-column scaling share the private rendering state.
 * projected_primitives.h documents inputs, side effects and scratchpad
 * constraints. Membership/boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "projected_primitives.h"
#include "scratchpad.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/libgs.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"
#include "include_asm.h"
#include "render_packet_pool.h"

void InitProjectedPrimitiveSource(ProjectedPrimitiveSource *source, void *batchData) {
    s32 count;

    source->batch = batchData;
    count = *(s32 *) ((char *) batchData + 8);
    if (count >= 2) {
        source->cullPointData = (s8 *) ((char *) batchData + ((count * 28) + 12));
    } else {
        source->cullPointData = 0;
    }
}

void ClearProjectedPrimitiveSource(ProjectedPrimitiveSource *source) {
    source->batch = 0;
    source->cullPointData = 0;
}

void ApplyProjectedRotationScale(void);
void EmitProjectedPrimitiveEntry(u8 *base, u8 *elem);

s32 gProjectedRotationScaleEnabled;
void *gProjectedPrimitiveBatch;

void DrawProjectedPrimitiveBatch(ProjectedPrimitiveSource *source) {
    s32 i;
    u_long *slot;

    gProjectedPrimitiveBatch = source->batch;
    PushMatrix();
    if (gProjectedRotationScaleEnabled != 0) {
        SCRATCH_STACK_BEGIN(SCRATCH_STACK_SLOT);
        ApplyProjectedRotationScale();
        SCRATCH_STACK_END();
    }
    i = 0;
    if (i < *(s32 *)((char *)gProjectedPrimitiveBatch + 8)) {
        slot = SCRATCH_STACK_SLOT;
        do {
            SCRATCH_STACK_BEGIN(slot);
            EmitProjectedPrimitiveEntry((char *)gProjectedPrimitiveBatch + 0xC,
                         (char *)gProjectedPrimitiveBatch + 0xC + i * 0x1C);
            SCRATCH_STACK_END();
        } while (++i < *(s32 *)((char *)gProjectedPrimitiveBatch + 8));
    }
    PopMatrix();
}

/* The classified SCRATCH_STACK macros preserve the original stack switch:
 * the main-RAM stack pointer is saved in the scratchpad's top word, and
 * callees run beneath it. Keep the switch/restore boundaries intact.
 * See notes/research/scratchpad-stack-switch.md. */

s16 gProjectedPrimitiveCount;

void BuildProjectedClipPlanes(VECTOR *a, VECTOR *b);
s32 IsPointInsideProjectedClipPlanes(SVECTOR *arg0);

typedef struct {
    /* 0x00 */ u8 data[0x1C];
} ProjectedPrimitiveEntry;

typedef struct {
    /* 0x00 */ u8 pad[8];
    /* 0x08 */ s32 count;
    /* 0x0C */ ProjectedPrimitiveEntry prims[1];
} ProjectedPrimitiveBatch;

s16 DrawCulledProjectedPrimitives(ProjectedPrimitiveSource *source, VECTOR *eye, VECTOR *target) {
    u_long *slot;
    ProjectedPrimitiveBatch *batch;
    s32 i;
    s32 vecBase;

    gProjectedPrimitiveCount = 0;
    slot = SCRATCH_STACK_SLOT;
    SCRATCH_STACK_BEGIN(slot);

    batch = source->batch;
    vecBase = (s32)source->cullPointData;
    if (vecBase == 0) {
        return 0;
    }
    BuildProjectedClipPlanes(eye, target);
    PushMatrix();
    if (gProjectedRotationScaleEnabled != 0) {
        ApplyProjectedRotationScale();
    }
    i = 0;
    while (i < batch->count) {
        if (IsPointInsideProjectedClipPlanes((SVECTOR *)((s8 *)vecBase + 4 + i * 8)) != 0) {
            EmitProjectedPrimitiveEntry(batch->prims, &batch->prims[i]);
        }
        i++;
    }

    PopMatrix();
    SCRATCH_STACK_END();
    return gProjectedPrimitiveCount;
}

s32 IsPointInsideProjectedClipPlanes(SVECTOR *arg0) {
    s32 diff;
    SVECTOR diff_vec;
    SVECTOR normal;
    s16 *plane;
    SVECTOR *normal_ptr;
    s32 dot;

    diff = gProjectedClipOrigin[0] - arg0->vx;
    if (diff < 0) {
        diff += 0x7F;
    }
    diff_vec.vx = (s16)(diff >> 7);

    diff = gProjectedClipOrigin[1] - arg0->vy;
    if (diff < 0) {
        diff += 0x7F;
    }
    diff_vec.vy = (s16)(diff >> 7);

    diff = gProjectedClipOrigin[2] - arg0->vz;
    if (diff < 0) {
        diff += 0x7F;
    }
    diff_vec.vz = (s16)(diff >> 7);

    normal_ptr = &normal;
    VectorNormalSS(&diff_vec, normal_ptr);

    plane = gProjectedClipNormals;

    dot = plane[0] * normal_ptr->vx;
    dot += plane[1] * normal_ptr->vy;
    dot += plane[2] * normal_ptr->vz;
    if (dot >= 0) {
        plane += 3;
        dot = plane[0] * normal_ptr->vx;
        dot += plane[1] * normal_ptr->vy;
        dot += plane[2] * normal_ptr->vz;
        if (dot >= 0) {
            plane += 3;
            dot = plane[0] * normal_ptr->vx;
            dot += plane[1] * normal_ptr->vy;
            dot += plane[2] * normal_ptr->vz;
            if (dot >= 0) {
                plane += 3;
                dot = plane[0] * normal_ptr->vx;
                dot += plane[1] * normal_ptr->vy;
                dot += plane[2] * normal_ptr->vz;
                return (u32)~dot >> 31;
            }
        }
    }

    return 0;
}

typedef struct { u32 color; } PrimitiveColor;

#define WORD(p, n) (*(s32 *)((u8 *)(p) + (n)))
#define HALF(p, n) (*(u16 *)((u8 *)(p) + (n)))

void PopMatrix ();
void PushMatrix ();
s32 HasTriangleVertexXInBounds (s32 sxy0, s32 sxy1, s32 sxy2);
void ApplyProjectedRotationScale (void);
void BuildProjectedClipPlanes (VECTOR *arg0, VECTOR *arg1);
s32 IsPointInsideProjectedClipPlanes (SVECTOR *arg0);

s32 HasQuadVertexXInBounds(s32 arg0, s32 arg1, s32 arg2, s32 arg3); /* static */
struct_8005E3C0 *gProjectedPrimitiveContext;

void EmitProjectedPrimitiveEntry(u8 *arg0, u8 *arg1) {
    s32 sp10;
    s32 offset;
    u32 flags;
    s32 *temp_a0;
    s32 *temp_a0_2;
    s32 *temp_a0_3;
    s32 *temp_a0_4;
    s32 *temp_a0_5;
    s32 *temp_a0_6;
    s32 *temp_a0_7;
    s32 *temp_a0_8;
    POLY_F4 *temp_s1;
    POLY_G4 *temp_s1_2;
    POLY_F3 *temp_s4;
    POLY_G3 *temp_s4_2;
    s32 temp_s7;
    s32 var_a0;
    s32 var_a0_2;
    s32 var_a0_3;
    s32 var_a0_4;
    s32 var_a0_5;
    s32 var_a0_6;
    s32 var_a0_7;
    s32 var_a0_8;
    s32 var_s0;
    s32 var_s6;
    s32 var_v0;
    s32 var_v0_2;
    s32 var_v0_3;
    s32 var_v0_4;
    s32 var_v0_6;
    s32 var_v0_7;
    s32 var_v0_8;
    s32 var_v0_9;
    u32 var_v0_5;
    u8 *temp_a1;
    u8 *temp_a1_2;
    u8 *temp_a1_3;
    u8 *temp_a1_4;
    u8 *temp_s1_3;
    u8 *temp_s1_4;
    u8 *temp_s4_3;
    u8 *temp_s4_4;
    u8 *temp_s5;
    u8 *var_s2;
    u8 *packet;

    var_s6 = WORD(arg1, 0x14);
    if (var_s6 > 0) {
        PacketReservationHook(0x2000);
        temp_s5 = arg0 + WORD(arg1, 0x0);
        var_s2 = arg0 + WORD(arg1, 0x10);
        var_s0 = gActiveRenderContext->field_118;
        temp_s7 = gActiveRenderContext->field_120;
        gProjectedPrimitiveContext = gActiveRenderContext;
loop_2:
        if (var_s6 > 0) {
            flags = WORD(var_s2, 0x0);
            if ((flags & 0xFDFF0000) == 0x21010000) {
                temp_s4 = (POLY_F3 *)var_s0;
                temp_a1 = var_s2;
                gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 8), (SVECTOR *)temp_s5 + HALF(var_s2, 10), (SVECTOR *)temp_s5 + HALF(var_s2, 12));
                var_s2 = (u8 *) ((u8 *) var_s2 + 0x10);
                var_s6 -= 1;
                gte_rtpt_b();
                *(PrimitiveColor *)&temp_s4->r0 = *(PrimitiveColor *)(temp_a1 + 4);
                gte_stflg(&sp10);
                if (sp10 >= 0) {
                    gte_nclip_b();
                    setcode(var_s0, (((flags >> 0x18) & 2) | 0x20));
                    gte_stopz(&sp10);
                    if (sp10 > 0) {
                        gte_stsxy3_f3(temp_s4);
                        if (HasTriangleVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0xC), WORD(var_s0, 0x10)) != 0) {
                            gte_avsz3_b();
                            var_s0 += 0x14;
                            gte_stotz(&sp10);
                            if (!(flags & 0x8000)) {
                                sp10 += 0x23;
                            }
                            if (flags & 0x4000) {
                                sp10 += 1;
                            }
                            temp_a0 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                            WORD(temp_s4, 0x0) = (s32) ((*temp_a0 & 0xFFFFFF) | 0x04000000);
                            *temp_a0 = (s32)temp_s4 & 0xFFFFFF;
                            gProjectedPrimitiveCount += 1;
                        }
                    }
                }
                goto loop_2;
            }
loop_14:
            if (var_s6 > 0) {
                flags = WORD(var_s2, 0x0);
                if ((flags & 0xFDFF0000) == 0x31010000) {
                temp_s4_2 = (POLY_G3 *)var_s0;
                    temp_a1_2 = var_s2;
                    gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 16), (SVECTOR *)temp_s5 + HALF(var_s2, 18), (SVECTOR *)temp_s5 + HALF(var_s2, 20));
                    var_s2 = (u8 *) ((u8 *) var_s2 + 0x18);
                    var_s6 -= 1;
                    gte_rtpt_b();
                    *(s32 *)&temp_s4_2->r0 = (s32) WORD(temp_a1_2, 0x4);
                    *(s32 *)&temp_s4_2->r1 = (s32) WORD(temp_a1_2, 0x8);
                    gte_stflg(&sp10);
                    if (sp10 >= 0) {
                        gte_nclip_b();
                        WORD(var_s0, 0x14) = (s32) WORD(temp_a1_2, 0xC);
                        gte_stopz(&sp10);
                        if (sp10 > 0) {
                            gte_stsxy3_g3(temp_s4_2);
                            if (HasTriangleVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x10), WORD(var_s0, 0x18)) != 0) {
                                gte_avsz3_b();
                                setcode(var_s0, (((flags >> 0x18) & 2) | 0x30));
                                var_s0 += 0x1C;
                                gte_stotz(&sp10);
                                if (!(flags & 0x8000)) {
                                    sp10 += 0x23;
                                }
                                if (flags & 0x4000) {
                                    sp10 += 1;
                                }
                                temp_a0_2 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                WORD(temp_s4_2, 0x0) = (s32) ((*temp_a0_2 & 0xFFFFFF) | 0x06000000);
                                *temp_a0_2 = (s32)temp_s4_2 & 0xFFFFFF;
                                gProjectedPrimitiveCount += 1;
                            }
                        }
                    }
                    goto loop_14;
                }
loop_26:
                if (var_s6 > 0) {
                    flags = WORD(var_s2, 0x0);
                    packet = (u8 *)var_s0;
                    if ((flags & 0xFDFF0000) == 0x25010000) {
                        temp_s4_3 = var_s2;
                        packet = (u8 *)var_s0;
                        gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 20), (SVECTOR *)temp_s5 + HALF(var_s2, 22), (SVECTOR *)temp_s5 + HALF(var_s2, 24));
                        var_s2 = (u8 *) ((u8 *) var_s2 + 0x1C);
                        var_s6 -= 1;
                        gte_rtpt_b();
                        WORD(packet, 0xC) = (s32) WORD(temp_s4_3, 0x4);
                        WORD(packet, 0x14) = (s32) WORD(temp_s4_3, 0x8);
                        gte_stflg(&sp10);
                        if (sp10 >= 0) {
                            gte_nclip_b();
                            WORD(var_s0, 0x1C) = (s32) WORD(temp_s4_3, 0xC);
                            gte_stopz(&sp10);
                            if (sp10 > 0) {
                                gte_stsxy3_ft3(packet);
                                if (HasTriangleVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x10), WORD(var_s0, 0x18)) != 0) {
                                    gte_avsz3_b();
                                    WORD(var_s0, 0x4) = (s32) WORD(temp_s4_3, 0x10);
                                    setcode(var_s0, (((flags >> 0x18) & 2) | 0x24));
                                    gte_stotz(&sp10);
                                    if (!(flags & 0x8000)) {
                                        sp10 += 0x23;
                                    }
                                    if (flags & 0x4000) {
                                        sp10 += 1;
                                    }
                                    temp_a0_3 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                    WORD(var_s0, 0x0) = (s32) ((*temp_a0_3 & 0xFFFFFF) | 0x07000000);
                                    *temp_a0_3 = (s32)var_s0 & 0xFFFFFF;
                                    gProjectedPrimitiveCount += 1;
                                    var_s0 += 0x20;
                                }
                            }
                        }
                        goto loop_26;
                    }
loop_38:
                    if (var_s6 > 0) {
                        flags = WORD(var_s2, 0x0);
                    packet = (u8 *)var_s0;
                        if ((flags & 0xFDFF0000) == 0x35010000) {
                            temp_s4_4 = var_s2;
                            packet = (u8 *)var_s0;
                            gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 28), (SVECTOR *)temp_s5 + HALF(var_s2, 30), (SVECTOR *)temp_s5 + HALF(var_s2, 32));
                            var_s2 = (u8 *) ((u8 *) var_s2 + 0x24);
                            var_s6 -= 1;
                            gte_rtpt_b();
                            WORD(packet, 0x4) = (s32) WORD(temp_s4_4, 0x10);
                            WORD(packet, 0x10) = (s32) WORD(temp_s4_4, 0x14);
                            gte_stflg(&sp10);
                            if (sp10 >= 0) {
                                gte_nclip_b();
                                WORD(var_s0, 0x1C) = (s32) WORD(temp_s4_4, 0x18);
                                gte_stopz(&sp10);
                                if (sp10 > 0) {
                                    gte_stsxy3_gt3(packet);
                                    if (HasTriangleVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x14), WORD(var_s0, 0x20)) != 0) {
                                        gte_avsz3_b();
                                        WORD(var_s0, 0xC) = (s32) WORD(temp_s4_4, 0x4);
                                        WORD(var_s0, 0x18) = (s32) WORD(temp_s4_4, 0x8);
                                        gte_stotz(&sp10);
                                        if (!(flags & 0x8000)) {
                                            sp10 += 0x23;
                                        }
                                        if (flags & 0x4000) {
                                            sp10 += 1;
                                        }
                                        WORD(var_s0, 0x24) = (s32) WORD(temp_s4_4, 0xC);
                                        setcode(var_s0, (((flags >> 0x18) & 2) | 0x34));
                                        temp_a0_4 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                        WORD(var_s0, 0x0) = (s32) ((*temp_a0_4 & 0xFFFFFF) | 0x09000000);
                                        *temp_a0_4 = (s32)var_s0 & 0xFFFFFF;
                                        gProjectedPrimitiveCount += 1;
                                        var_s0 += 0x28;
                                    }
                                }
                            }
                            goto loop_38;
                        }
loop_50:
                        if (var_s6 > 0) {
                            flags = WORD(var_s2, 0x0);
                            if ((flags & 0xFDFF0000) == 0x29010000) {
                temp_s1 = (POLY_F4 *)var_s0;
                                temp_a1_3 = var_s2;
                                gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 8), (SVECTOR *)temp_s5 + HALF(var_s2, 10), (SVECTOR *)temp_s5 + HALF(var_s2, 12));
                                var_s2 = (u8 *) ((u8 *) var_s2 + 0x10);
                                var_s6 -= 1;
                                gte_rtpt_b();
                                *(s32 *)&temp_s1->r0 = (s32) WORD(temp_a1_3, 0x4);
                                gte_stflg(&sp10);
                                if (sp10 >= 0) {
                                    gte_nclip_b();
                                    gte_stopz(&sp10);
                                    if (sp10 > 0) {
                                        gte_stsxy3_f4(temp_s1);
                                        gte_ldv0((SVECTOR *)temp_s5 + HALF(temp_a1_3, 14));
                                        gte_rtps();
                                        setcode(var_s0, (((flags >> 0x18) & 2) | 0x28));
                                        gte_stflg(&sp10);
                                        if (sp10 >= 0) {
                                            gte_stsxy(&WORD(var_s0, 20));
                                            if (HasQuadVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0xC), WORD(var_s0, 0x10), WORD(var_s0, 0x14)) != 0) {
                                                gte_avsz4_b();
                                                var_s0 += 0x18;
                                                gte_stotz(&sp10);
                                                if (!(flags & 0x8000)) {
                                                    sp10 += 0x23;
                                                }
                                                if (flags & 0x4000) {
                                                    sp10 += 1;
                                                }
                                                temp_a0_5 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                                WORD(temp_s1, 0x0) = (s32) ((*temp_a0_5 & 0xFFFFFF) | 0x05000000);
                                                *temp_a0_5 = (s32)temp_s1 & 0xFFFFFF;
                                                gProjectedPrimitiveCount += 1;
                                            }
                                        }
                                    }
                                }
                                goto loop_50;
                            }
loop_63:
                            if (var_s6 > 0) {
                                flags = WORD(var_s2, 0x0);
                                if ((flags & 0xFDFF0000) == 0x39010000) {
                temp_s1_2 = (POLY_G4 *)var_s0;
                                    temp_a1_4 = var_s2;
                                    gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 20), (SVECTOR *)temp_s5 + HALF(var_s2, 22), (SVECTOR *)temp_s5 + HALF(var_s2, 24));
                                    var_s2 = (u8 *) ((u8 *) var_s2 + 0x1C);
                                    var_s6 -= 1;
                                    gte_rtpt_b();
                                    *(s32 *)&temp_s1_2->r0 = (s32) WORD(temp_a1_4, 0x4);
                                    *(s32 *)&temp_s1_2->r1 = (s32) WORD(temp_a1_4, 0x8);
                                    gte_stflg(&sp10);
                                    if (sp10 >= 0) {
                                        gte_nclip_b();
                                        WORD(var_s0, 0x14) = (s32) WORD(temp_a1_4, 0xC);
                                        gte_stopz(&sp10);
                                        if (sp10 > 0) {
                                            gte_stsxy3_g4(temp_s1_2);
                                            gte_ldv0((SVECTOR *)temp_s5 + HALF(temp_a1_4, 26));
                                            gte_rtps();
                                            WORD(var_s0, 0x1C) = (s32) WORD(temp_a1_4, 0x10);
                                            gte_stflg(&sp10);
                                            if (sp10 >= 0) {
                                                gte_stsxy(&WORD(var_s0, 32));
                                                if (HasQuadVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x10), WORD(var_s0, 0x18), WORD(var_s0, 0x20)) != 0) {
                                                    gte_avsz4_b();
                                                    setcode(var_s0, (((flags >> 0x18) & 2) | 0x38));
                                                    var_s0 += 0x24;
                                                    gte_stotz(&sp10);
                                                    if (!(flags & 0x8000)) {
                                                        sp10 += 0x23;
                                                    }
                                                    if (flags & 0x4000) {
                                                        sp10 += 1;
                                                    }
                                                    temp_a0_6 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                                    WORD(temp_s1_2, 0x0) = (s32) ((*temp_a0_6 & 0xFFFFFF) | 0x08000000);
                                                    *temp_a0_6 = (s32)temp_s1_2 & 0xFFFFFF;
                                                    gProjectedPrimitiveCount += 1;
                                                }
                                            }
                                        }
                                    }
                                    goto loop_63;
                                }
loop_76:
                                if (var_s6 > 0) {
                                    flags = WORD(var_s2, 0x0);
                    
                                    if ((flags & 0xFDFF0000) == 0x2D010000) {
                        packet = (u8 *)var_s0;
                                        temp_s1_3 = var_s2;
                                        gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 24), (SVECTOR *)temp_s5 + HALF(var_s2, 26), (SVECTOR *)temp_s5 + HALF(var_s2, 28));
                                        var_s2 = (u8 *) ((u8 *) var_s2 + 0x20);
                                        var_s6 -= 1;
                                        gte_rtpt_b();
                                        *(s32 *)&((POLY_FT4 *)packet)->r0 = (s32) WORD(temp_s1_3, 0x14);
                                        *(s32 *)&((POLY_FT4 *)packet)->u0 = (s32) WORD(temp_s1_3, 0x4);
                                        gte_stflg(&sp10);
                                        if (sp10 >= 0) {
                                            gte_nclip_b();
                                            WORD(var_s0, 0x1C) = (s32) WORD(temp_s1_3, 0xC);
                                            gte_stopz(&sp10);
                                            if (sp10 > 0) {
                                                gte_stsxy3_ft4(packet);
                                                gte_ldv0((SVECTOR *)temp_s5 + HALF(temp_s1_3, 30));
                                                gte_rtps();
                                                WORD(var_s0, 0x24) = (s32) WORD(temp_s1_3, 0x10);
                                                gte_stflg(&sp10);
                                                if (sp10 >= 0) {
                                                    gte_stsxy(&WORD(var_s0, 32));
                                                    if (HasQuadVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x10), WORD(var_s0, 0x18), WORD(var_s0, 0x20)) != 0) {
                                                        gte_avsz4_b();
                                                        WORD(var_s0, 0x14) = (s32) WORD(temp_s1_3, 0x8);
                                                        setcode(var_s0, (((flags >> 0x18) & 2) | 0x2C));
                                                        gte_stotz(&sp10);
                                                        if (!(flags & 0x8000)) {
                                                            sp10 += 0x23;
                                                        }
                                                        if (flags & 0x4000) {
                                                            sp10 += 1;
                                                        }
                                                        temp_a0_7 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                                        WORD(var_s0, 0x0) = (s32) ((*temp_a0_7 & 0xFFFFFF) | 0x09000000);
                                                        *temp_a0_7 = (s32)var_s0 & 0xFFFFFF;
                                                        gProjectedPrimitiveCount += 1;
                                                        var_s0 += 0x28;
                                                    }
                                                }
                                            }
                                        }
                                        goto loop_76;
                                    }
loop_89:
                                    if (var_s6 != 0) {
                                        flags = WORD(var_s2, 0x0);
                    
                                        if ((flags & 0xFDFF0000) == 0x3D010000) {
                        packet = (u8 *)var_s0;
                                            temp_s1_4 = var_s2;
                                            gte_ldv3((SVECTOR *)temp_s5 + HALF(var_s2, 36), (SVECTOR *)temp_s5 + HALF(var_s2, 38), (SVECTOR *)temp_s5 + HALF(var_s2, 40));
                                            var_s2 = (u8 *) ((u8 *) var_s2 + 0x2C);
                                            var_s6 -= 1;
                                            gte_rtpt_b();
                                            *(s32 *)&((POLY_GT4 *)packet)->r0 = (s32) WORD(temp_s1_4, 0x14);
                                            *(s32 *)&((POLY_GT4 *)packet)->r1 = (s32) WORD(temp_s1_4, 0x18);
                                            gte_stflg(&sp10);
                                            if (sp10 >= 0) {
                                                gte_nclip_b();
                                                WORD(var_s0, 0x1C) = (s32) WORD(temp_s1_4, 0x1C);
                                                gte_stopz(&sp10);
                                                if (sp10 > 0) {
                                                    gte_stsxy3_gt4(packet);
                                                    gte_ldv0((SVECTOR *)temp_s5 + HALF(temp_s1_4, 42));
                                                    gte_rtps();
                                                    WORD(var_s0, 0x28) = (s32) WORD(temp_s1_4, 0x20);
                                                    gte_stflg(&sp10);
                                                    if (sp10 >= 0) {
                                                        gte_stsxy(&WORD(var_s0, 44));
                                                        if (HasQuadVertexXInBounds(WORD(var_s0, 0x8), WORD(var_s0, 0x14), WORD(var_s0, 0x20), WORD(var_s0, 0x2C)) != 0) {
                                                            gte_avsz4();
                                                            WORD(var_s0, 0xC) = (s32) WORD(temp_s1_4, 0x4);
                                                            WORD(var_s0, 0x18) = (s32) WORD(temp_s1_4, 0x8);
                                                            WORD(var_s0, 0x24) = (s32) WORD(temp_s1_4, 0xC);
                                                            WORD(var_s0, 0x30) = (s32) WORD(temp_s1_4, 0x10);
                                                            setcode(var_s0, (((flags >> 0x18) & 2) | 0x3C));
                                                            gte_stotz(&sp10);
                                                            if (!(flags & 0x8000)) {
                                                                sp10 += 0x23;
                                                            }
                                                            if (flags & 0x4000) {
                                                                sp10 += 1;
                                                            }
                                                            temp_a0_8 = (s32 *)(temp_s7 + (sp10 / 4) * 4);
                                                            WORD(var_s0, 0x0) = (s32) ((*temp_a0_8 & 0xFFFFFF) | 0x0C000000);
                                                            *temp_a0_8 = (s32)var_s0 & 0xFFFFFF;
                                                            gProjectedPrimitiveCount += 1;
                                                            var_s0 += 0x34;
                                                        }
                                                    }
                                                }
                                            }
                                            goto loop_89;
                                        }
                                    }
                                }
                            }
                        }
                    }
                }
            }
        }
        gProjectedPrimitiveContext->field_118 = (s32)var_s0;
    }
}

#define X_RANGE_BIAS 19
#define X_PRECHECK_SIZE 419
#define X_BOUNDS_SIZE 339

/*
 * Return 1 if any projected triangle vertex has an X coordinate in
 * [-19, 319]. Each argument is a packed GTE SXY value (the SDK DVECTOR
 * layout), with signed X in the low half and signed Y in the high half.
 *
 * The preliminary pass tests the wider interval [-20, 398]. That interval
 * contains the final interval, so the pass is redundant to the observable
 * result, but it is retained to reproduce the original function.
 */
s32 HasTriangleVertexXInBounds(s32 sxy0, s32 sxy1, s32 sxy2) {
    s32 precheckX;
    s32 vertexX;

    precheckX = (s16)(sxy0 + 1);
    precheckX += X_RANGE_BIAS;
    if ((u16)precheckX < X_PRECHECK_SIZE) {
        goto CHECK_X_BOUNDS;
    }

    precheckX = (s16)(sxy1 + 1);
    precheckX += X_RANGE_BIAS;
    if ((u16)precheckX < X_PRECHECK_SIZE) {
        goto CHECK_X_BOUNDS;
    }

    precheckX = (s16)(sxy2 + 1);
    precheckX += X_RANGE_BIAS;
    if ((u16)precheckX < X_PRECHECK_SIZE) {
        goto CHECK_X_BOUNDS;
    }

    return 0;

CHECK_X_BOUNDS:
    vertexX = (s16)sxy0;
    if ((u16)(vertexX + X_RANGE_BIAS) < X_BOUNDS_SIZE) {
        return 1;
    }

    vertexX = (s16)sxy1;
    if ((u16)(vertexX + X_RANGE_BIAS) < X_BOUNDS_SIZE) {
        return 1;
    }

    vertexX = (s16)sxy2;
    return (u16)(vertexX + X_RANGE_BIAS) < X_BOUNDS_SIZE;
}

/* Return 1 if any packed GTE SXY argument has X in [-19, 319]. */
s32 HasQuadVertexXInBounds(s32 sxy0, s32 sxy1, s32 sxy2, s32 sxy3) {
    union {
        unsigned long long wide;
        struct {
            u32 low;
            s32 high;
        } words;
    } t;
    s16 x;

    t.words.high = sxy0 << 16;
    x = t.words.high >> 16;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy1;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy2;
    if (x >= -19 && x < 320) {
        return 1;
    }
    x = sxy3;
    return x >= -19 && x < 320;
}

void BuildProjectedClipPlanes(VECTOR *arg0, VECTOR *arg1) {
    MATRIX matrices[4];
    VECTOR direction;
    VECTOR delta;
    s16 *out;
    s16 *row;
    MATRIX *matrix;
    s32 i;

    out = gProjectedClipNormals;
    delta.vx = arg1->vx - arg0->vx;
    delta.vy = arg1->vy - arg0->vy;
    delta.vz = arg1->vz - arg0->vz;
    VectorNormal(&delta, &direction);
    gProjectedClipOrigin[0] = arg0->vx - direction.vx * 2;
    gProjectedClipOrigin[1] = arg0->vy - direction.vy * 2;
    gProjectedClipOrigin[2] = arg0->vz - direction.vz * 2;
    matrices[0] = GsIDMATRIX;
    matrices[1] = GsIDMATRIX;
    matrices[2] = GsIDMATRIX;
    matrices[3] = GsIDMATRIX;
    RotMatrixY(gProjectedClipAngles[0], &matrices[0]);
    RotMatrixY(gProjectedClipAngles[1], &matrices[1]);
    RotMatrixX(gProjectedClipAngles[2], &matrices[2]);
    RotMatrixX(gProjectedClipAngles[3], &matrices[3]);
    PushMatrix();
    for (i = 0; i < 4; i++) {
        matrix = &matrices[i];
        row = matrix->m[i / 2];
        gte_SetRotMatrix(matrix);
        gte_ldclmv((s16 *)&gCameraViewMatrix);
        gte_mvmva_core(0x4A49E012);
        gte_stclmv(matrix);
        gte_ldclmv((s16 *)&gCameraViewMatrix + 1);
        gte_mvmva_core(0x4A49E012);
        *out++ = *row++;
        gte_stclmv(&matrix->m[0][1]);
        gte_ldclmv((s16 *)&gCameraViewMatrix + 2);
        gte_mvmva_core(0x4A49E012);
        *out++ = *row++;
        gte_stclmv(&matrix->m[0][2]);
        *out++ = *row++;
    }
    PopMatrix();
}

/* GP-relative global owned by this TU. */

s32 SetProjectedRotationScale(s32 numerator, s32 denominator) {
    gProjectedRotationScale.zNumerator = numerator;
    gProjectedRotationScale.yNumerator = numerator;
    gProjectedRotationScale.xNumerator = numerator;
    gProjectedRotationScale.zDenominator = denominator;
    gProjectedRotationScale.yDenominator = denominator;
    gProjectedRotationScale.xDenominator = denominator;
    gProjectedRotationScaleEnabled = 1;
    return 1;
}

/* Per-column setter variant of SetProjectedRotationScale: takes the three
 * numerator/denominator pairs separately and then enables scaling. */
void func_8001D678(s32 xNumerator, s32 xDenominator, s32 yNumerator, s32 yDenominator, s32 zNumerator, s32 zDenominator) {
    gProjectedRotationScale.xNumerator = xNumerator;
    gProjectedRotationScale.xDenominator = xDenominator;
    gProjectedRotationScale.yNumerator = yNumerator;
    gProjectedRotationScale.yDenominator = yDenominator;
    gProjectedRotationScale.zNumerator = zNumerator;
    gProjectedRotationScale.zDenominator = zDenominator;
    gProjectedRotationScaleEnabled = 1;
}

void DisableProjectedRotationScale(void) {
    gProjectedRotationScaleEnabled = 0;
}

void ApplyProjectedRotationScale(void) {
    MATRIX m;
    s32 i;

    gte_ReadRotMatrix(&m);
    for (i = 0; i < 3; i++) {
        if (gProjectedRotationScale.xNumerator & ~1) {
            if (gProjectedRotationScale.xDenominator >= 2) {
                m.m[i][0] = m.m[i][0] * gProjectedRotationScale.xNumerator / gProjectedRotationScale.xDenominator;
            }
        }
        if (gProjectedRotationScale.yNumerator & ~1) {
            if (gProjectedRotationScale.yDenominator >= 2) {
                m.m[i][1] = m.m[i][1] * gProjectedRotationScale.yNumerator / gProjectedRotationScale.yDenominator;
            }
        }
        if (gProjectedRotationScale.zNumerator & ~1) {
            if (gProjectedRotationScale.zDenominator >= 2) {
                m.m[i][2] = m.m[i][2] * gProjectedRotationScale.zNumerator / gProjectedRotationScale.zDenominator;
            }
        }
    }
    gte_SetRotMatrix(&m);
}
