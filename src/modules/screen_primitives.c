/*
 * Screen-space primitives: packet-work wrappers followed by their paired
 * point/line/rectangle builders, in original exe:0x80014E90-0x80015704 order.
 * This closed helper family owns no global state. Each builder initializes
 * and prepends one SDK packet, returning the following packet address; the
 * final builder adds a draw-mode packet too. No helper is made static without
 * evidence of original private linkage. See notes/file-groupings.md.
 */

#include "common.h"
#include "packet_work.h"
#include "screen_primitives.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

/* Builders retain external linkage, but only the drawing wrappers are
 * published: no external C caller uses the builders directly. */
TILE_1 *BuildScreenPointPacket(TILE_1 *, u_long *, s16, s16, s32, s16);
LINE_F2 *BuildScreenLinePacket(LINE_F2 *, u_long *, s16, s16, s16, s16, s32, s16);
void *BuildScreenGradientRectPacket(POLY_G4 *, s32 *, s16, s16, s16, s16,
                                    s32, s32, s32, s32, s16);
void *BuildScreenFlatRectPacket(POLY_F4 *, s32 *, s16, s16, s16, s16, s32, s16);
TILE *BuildScreenTileRectPacket(TILE *, u_long *, s16, s16, s16, s16, s32, s16);
void *BuildScreenFlatRectWithBlendModePacket(POLY_F4 *, s32 *, s32, s32,
                                             s16, s16, s32, s16, u8);

void DrawScreenPoint(u32 *ot, s16 x, s16 y, s32 color, s16 semitransparent) {
    BuildScreenPointPacket(ReservePacketWorkBytes(0xC), ot, x, y, color,
                           (s16)(s32)semitransparent);
}

void DrawScreenLine(u32 *ot, s16 x0, s16 y0, s16 x1, s16 y1,
                    s32 color, s16 semitransparent) {
    BuildScreenLinePacket(ReservePacketWorkBytes(0x10), ot, x0, y0,
                          (s16)(s32)x1, (s16)(s32)y1, color,
                          (s16)(s32)semitransparent);
}

void DrawScreenGradientRect(s32 *ot, s16 x, s16 y, s16 width, s16 height,
                            s32 topLeft, s32 topRight, s32 bottomLeft,
                            s32 bottomRight, s16 semitransparent) {
    BuildScreenGradientRectPacket(ReservePacketWorkBytes(0x24), ot, x, y,
                                  (s16)(s32)width, (s16)(s32)height,
                                  topLeft, topRight, bottomLeft, bottomRight,
                                  (s16)(s32)semitransparent);
}

void DrawScreenFlatRect(s32 *ot, s16 x, s16 y, s16 width, s16 height,
                        s32 color, s16 semitransparent) {
    BuildScreenFlatRectPacket(ReservePacketWorkBytes(0x18), ot, x, y,
                              (s16)(s32)width, (s16)(s32)height, color,
                              (s16)(s32)semitransparent);
}

void DrawScreenTileRect(u32 *ot, s16 x, s16 y, s16 width, s16 height,
                        s32 color, s16 semitransparent) {
    BuildScreenTileRectPacket(ReservePacketWorkBytes(0x10), ot, x, y,
                              (s16)(s32)width, (s16)(s32)height, color,
                              (s16)(s32)semitransparent);
}

void DrawScreenFlatRectWithBlendMode(s32 *ot, s16 x, s16 y, s16 width,
                                     s16 height, s32 color,
                                     s16 semitransparent, u8 blendMode) {
    SetPacketWorkCursor((s32)BuildScreenFlatRectWithBlendModePacket(
        ReservePacketWorkBytes(0), ot, x, y, (s16)(s32)width, (s16)(s32)height,
        color, (s16)(s32)semitransparent, blendMode));
}

/* All builders preserve branch-local code stores through a shared code
 * variable; their join shape is sensitive to moving that store outside. */
TILE_1 *BuildScreenPointPacket(TILE_1 *p, u_long *ot, s16 x0, s16 y0,
                               s32 color, s16 semitransparent) {
    s32 code;

    setTile1(p);
    if (semitransparent != 0) {
        code = 0x6A;
        setcode(p, code);
    } else {
        code = 0x68;
        setcode(p, code);
    }
    setXY0(p, x0, y0);
    setRGB0(p, (u8)(color >> 16), (u8)(color >> 8), (u8)color);
    addPrim(ot, p);
    return p + 1;
}

LINE_F2 *BuildScreenLinePacket(LINE_F2 *p, u_long *ot, s16 x0, s16 y0,
                               s16 x1, s16 y1, s32 color,
                               s16 semitransparent) {
    s32 code;
    u8 r0, g0, b0;

    setLineF2(p);
    if (semitransparent != 0) {
        code = 0x42;
        setcode(p, code);
    } else {
        code = 0x40;
        setcode(p, code);
    }
    r0 = (u8)(color >> 16);
    g0 = (u8)(color >> 8);
    b0 = (u8)color;
    setXY0(p, x0, y0);
    p->x1 = x1;
    p->y1 = y1;
    p->r0 = r0;
    p->g0 = g0;
    p->b0 = b0;
    addPrim(ot, p);
    return p + 1;
}

/* Vertex/color order is top-left, top-right, bottom-left, bottom-right.
 * Keep the staged coordinate and color stores: the compiler schedules them
 * together with the tag link. Branch-local code stores share one code value. */
void *BuildScreenGradientRectPacket(POLY_G4 *p, s32 *ot, s16 x, s16 y, s16 width, s16 height,
                    s32 topLeft, s32 topRight, s32 bottomLeft, s32 bottomRight,
                    s16 semitransparent) {
    s32 right;
    s32 bottom;
    s8 code;

    setPolyG4(p);
    if (semitransparent != 0) {
        code = 0x3A;
        setcode(p, code);
    } else {
        code = 0x38;
        setcode(p, code);
    }
    right = x + width;
    p->y0 = y;
    p->y1 = y;
    bottom = y + height;
    p->x1 = (s16)right;
    p->x3 = (s16)right;
    p->y2 = (s16)bottom;
    p->y3 = (s16)bottom;
    p->r0 = (s8)(topLeft >> 16);
    p->g0 = (s8)(topLeft >> 8);
    p->r1 = (s8)(topRight >> 16);
    p->g1 = (s8)(topRight >> 8);
    p->r2 = (s8)(bottomLeft >> 16);
    p->g2 = (s8)(bottomLeft >> 8);
    p->x0 = x;
    p->x2 = x;
    p->b0 = (s8)topLeft;
    p->b1 = (s8)topRight;
    p->b2 = (s8)bottomLeft;
    p->r3 = (s8)(bottomRight >> 16);
    p->g3 = (s8)(bottomRight >> 8);
    p->b3 = (s8)bottomRight;
    addPrim(ot, p);
    return (void *)((char *)p + 0x24);
}

void *BuildScreenFlatRectPacket(POLY_F4 *p, s32 *ot, s16 x, s16 y, s16 width, s16 height,
                    s32 color, s16 semitransparent) {
    s32 addressMask;
    s32 bottom;
    s32 rightAndTag;
    s32 code;

    setPolyF4(p);
    setRGB0(p, (s8)(color >> 16), (s8)(color >> 8), (s8)color);
    /* Both branches store the shared code value. Moving the store after the
     * branch changes the compiler's constant hoist and the resulting diamond. */
    if (semitransparent != 0) {
        code = 0x2A;
        setcode(p, (s8)code);
    } else {
        code = 0x28;
        setcode(p, (s8)code);
    }
    addressMask = 0xFFFFFF;
    rightAndTag = x + width;
    bottom = y + height;
    p->x1 = (s16)rightAndTag;
    p->x3 = (s16)rightAndTag;
    p->x0 = x;
    p->y0 = y;
    p->y1 = y;
    p->x2 = x;
    p->y2 = (s16)bottom;
    p->y3 = (s16)bottom;
    /* Expanded addPrim: preserve packet/OT length bytes while replacing
     * their low 24-bit links. Keep the existing signed tag access and reused
     * coordinate/tag temporary, which also constrain coordinate scheduling. */
    rightAndTag = (*(s32 *)p & 0xFF000000) | (*ot & addressMask);
    *(s32 *)p = rightAndTag;
    *ot = (*ot & 0xFF000000) | ((s32)p & addressMask);
    return (void *)((char *)p + 0x18);
}

/*
 * Initialize and link a TILE primitive, returning the next packet slot.
 * The shared code variable lets crossjump merge the branch-local code stores.
 * Keeping setXY0 after the join preserves the signed coordinate conversions.
 */
TILE *BuildScreenTileRectPacket(TILE *p, u_long *ot, s16 x0, s16 y0,
                                s16 w, s16 h, s32 color, s16 semitransparent) {
    s32 code;

    setTile(p);
    setRGB0(p, (u8)(color >> 16), (u8)(color >> 8), (u8)color);
    if (semitransparent != 0) {
        code = 0x62;
        setcode(p, code);
    } else {
        code = 0x60;
        setcode(p, code);
    }
    setXY0(p, x0, y0);
    setWH(p, w, h);
    addPrim(ot, p);
    return p + 1;
}

void *BuildScreenFlatRectWithBlendModePacket(POLY_F4 *p, s32 *ot,
                                             s32 x, s32 y, s16 width,
                                             s16 height, s32 color,
                                             s16 semitransparent, u8 blendMode) {
    p = BuildScreenFlatRectPacket(p, ot, (s16)x, (s16)y, width, height,
                                  color, semitransparent);
    setDrawTPage(p, 0, 1, (blendMode & 3) << 5);

    /* The last prepend executes first: establish the blend mode for the quad. */
    addPrim(ot, p);

    p = (POLY_F4 *)((char *)p + 8);
    return p;
}
