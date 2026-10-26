/* Read/cache flags from the last scene-geometry query's hit descriptor.
 * All three consumers share the resident getter; the cache result preserves
 * its existing low-status/high-flag register pair. Membership and boundaries
 * are documented in notes/file-groupings.md. */

#include "common.h"
#include "game_types.h"
#include "ovl11_geometry_hit_flags.h"

s32 GetVal8005E524(void);

s32 Ovl11GetGeometryHitHighBit(void) {
    u8 *hit;

    hit = (u8 *)GetVal8005E524();
    return ((u32)hit[0]) >> 7;
}

s32 Ovl11ClassifyGeometryHit(void) {
    u32 *p;
    u32 v;
    u32 r;

    p = GetVal8005E524();
    if (p == 0) {
        return 0;
    }
    v = *p;
    if (v & 4) {
        return 2;
    }
    if (v & 0x40) {
        return 3;
    }
    r = v & 2;
    return r != 0;
}

/* The resident getter carries the address in its established s32 ABI.
 * Word-packing unions preserve the existing v0/v1 result: status-only paths
 * specify only the low word. Consumers must not treat their high word as a
 * Boolean or expect it to be zero. */

static __inline__ unsigned long long flag_result(u32 high) {
    union {
        struct {
            u32 low;
            u32 high;
        } words;
        unsigned long long wide;
    } result = {{0, high}};

    return result.wide;
}

static __inline__ unsigned long long status_result(u32 low) {
    union {
        u32 low;
        unsigned long long wide;
    } result = {low};

    return result.wide;
}

unsigned long long Ovl11CacheGeometryHit(void) {
    u32 *p;
    u32 v;
    u32 low;
    unsigned long long result;

    p = GetVal8005E524();
    gOvl11CachedGeometryHit = p;
    if (p == 0) {
        return status_result(1);
    }
    v = *p;
    low = v & 0xFFFF;
    gOvl11CachedGeometryFlags = low;
    if (v & 0x2000) {
        result = status_result(0);
    } else {
        result = flag_result(0x8000);
    }
    return result;
}
