/*
 * Legacy projected textured-quad packet paths (exe:8001DCA8-8001DFD4).
 * Both compiled emitters project four vertices of a texture-package quad into
 * the scratch POLY_FT4 at the render context's packet cursor and consult the
 * acceptance hook before linking it into the ordering table. One derives and
 * caches the OT depth from the projected z, the other takes the depth from its
 * caller. The hook always rejects in this build, so neither emitter links a
 * packet or updates a depth cache from here; the projection and the scratch
 * packet writes before the hook still happen, so this is not a side-effect-free
 * API. The context pointer is cached in a GP-relative word by each emitter.
 *
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/inline_c.h"
#include "psyq/inline_c_gas.h"
#include "render_packet_pool.h"
#include "game_types.h"

/* Acceptance hook shared by both emitters: called with the four packed screen
 * coordinates of the projected quad and always declines in this build. */
s32 AcceptProjectedQuad(s32 sxy0, s32 sxy1, s32 sxy2, s32 sxy3) {
    return 0;
}

/* Render context the last emitter call projected into. */
struct_8005E3C0 *gProjectedQuadDepthContext;
struct_8005E3C0 *gProjectedQuadFixedDepthContext;

/* Project the quad, then link it at a depth derived from the projected z:
 * the cached context's field_114 says how many low bits the ordering table
 * ignores. depthBias is added to the z before the shift. */
s32 EmitProjectedQuadPacket(ProjectedQuadInput *quad, s32 depthBias) {
    POLY_FT4 *packet;
    s32 flagOrDepth;
    s32 depth;
    s32 color;

    gte_ldv3(&quad->v0, &quad->v1, &quad->v2);
    packet = (POLY_FT4 *)((struct_8005E3C0 *)&gRenderContexts)->field_118;
    gProjectedQuadDepthContext = (struct_8005E3C0 *)&gRenderContexts;
    gte_rtpt_b();
    *(s32 *)&packet->u0 = quad->uv0;
    *(s32 *)&packet->u1 = quad->uv1;
    gte_stflg(&flagOrDepth);
    if (flagOrDepth < 0) {
        return 0;
    }
    gte_nclip_b();
    *(s32 *)&packet->u2 = quad->uv2;
    gte_stopz(&flagOrDepth);
    if (flagOrDepth <= 0) {
        return 0;
    }
    gte_stsxy3_ft4(packet);
    gte_ldv0(&quad->v3);
    gte_rtps();
    *(s32 *)&packet->u3 = quad->uv3;
    gte_stflg(&flagOrDepth);
    if (flagOrDepth < 0) {
        return 0;
    }
    gte_stsxy(&packet->x3);
    gte_avsz4();
    if (AcceptProjectedQuad(*(s32 *)&packet->x0, *(s32 *)&packet->x1,
                            *(s32 *)&packet->x2, *(s32 *)&packet->x3) == 0) {
        return 0;
    }
    gte_stotz(&flagOrDepth);
    color = quad->color;
    setlen(packet, 9);
    *(s32 *)&packet->r0 = color;
    setcode(packet, 0x2C);
    depth = (flagOrDepth + depthBias) >> (14 - gProjectedQuadDepthContext->field_114);
    gProjectedQuadOtDepth = depth;
    LinkAndReservePacket(depth, sizeof(POLY_FT4));
    return (s32)packet;
}

/* Same projection and packet build; the caller supplies the ordering-table
 * index instead of letting the emitter derive it from the projected z. */
s32 EmitProjectedQuadPacketAtDepth(ProjectedQuadInput *quad, s32 otIndex) {
    POLY_FT4 *packet;
    s32 flagOrDepth;
    s32 color;

    gte_ldv3(&quad->v0, &quad->v1, &quad->v2);
    packet = (POLY_FT4 *)((struct_8005E3C0 *)&gRenderContexts)->field_118;
    gProjectedQuadFixedDepthContext = (struct_8005E3C0 *)&gRenderContexts;
    gte_rtpt_b();
    *(s32 *)&packet->u0 = quad->uv0;
    *(s32 *)&packet->u1 = quad->uv1;
    gte_stflg(&flagOrDepth);
    if (flagOrDepth < 0) {
        return 0;
    }
    gte_nclip_b();
    *(s32 *)&packet->u2 = quad->uv2;
    gte_stopz(&flagOrDepth);
    if (flagOrDepth <= 0) {
        return 0;
    }
    gte_stsxy3_ft4(packet);
    gte_ldv0(&quad->v3);
    gte_rtps();
    *(s32 *)&packet->u3 = quad->uv3;
    gte_stflg(&flagOrDepth);
    if (flagOrDepth < 0) {
        return 0;
    }
    gte_stsxy(&packet->x3);
    gte_avsz4();
    if (AcceptProjectedQuad(*(s32 *)&packet->x0, *(s32 *)&packet->x1,
                            *(s32 *)&packet->x2, *(s32 *)&packet->x3) == 0) {
        return 0;
    }
    gte_stotz(&flagOrDepth);
    color = quad->color;
    setlen(packet, 9);
    *(s32 *)&packet->r0 = color;
    setcode(packet, 0x2C);
    LinkAndReservePacket(otIndex, sizeof(POLY_FT4));
    return (s32)packet;
}

/* Depth cached by the last accepted EmitProjectedQuadPacket call. */
s32 gProjectedQuadOtDepth;

s32 GetProjectedQuadOtDepth(void) {
    return gProjectedQuadOtDepth;
}
