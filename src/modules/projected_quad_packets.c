/*
 * Legacy projected textured-quad packet paths (exe:8001DCA8-8001DFD4).
 * Both paths project four vertices and consult the acceptance hook before
 * linking a POLY_FT4. One computes/caches an OT depth, the other takes it
 * from its caller. The hook always rejects in this build, so neither path
 * links a packet or updates the depth cache. Projection and scratch-packet
 * writes before the hook still occur; this is not a side-effect-free API.
 *
 * Both unused emitter bodies retain their existing dead assembly stubs;
 * no new assembly implementation or public C signature is introduced.
 * The accessor returns the raw cached depth, not a packet pointer.
 * Membership and conservative boundaries: notes/file-groupings.md.
 */

#include "common.h"
#include "include_asm.h"

/* Called with four packed screen-coordinate words by both legacy emitters.
 * They are unread; retain the existing no-parameter C contract. */
s32 AcceptProjectedQuad(void) {
    return 0;
}

INCLUDE_ASM("build/asm/nonmatchings/modules/projected_quad_packets", EmitProjectedQuadPacket);

INCLUDE_ASM("build/asm/nonmatchings/modules/projected_quad_packets", EmitProjectedQuadPacketAtDepth);

s32 gProjectedQuadOtDepth;

s32 GetProjectedQuadOtDepth(void) {
    return gProjectedQuadOtDepth;
}
