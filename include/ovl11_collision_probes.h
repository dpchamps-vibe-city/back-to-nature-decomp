#ifndef OVL11_COLLISION_PROBES_H
#define OVL11_COLLISION_PROBES_H

#include "common.h"
#include "game_types.h"

/* Ordered collision query. position is three signed world-coordinate words;
 * tag is the existing four-byte, halfword-aligned region/tag pair, by value.
 * excludedRecord is an address encoded as a word, ignored by most probes.
 * Nonzero means blocked/hit, zero means no blocking result. No budget is
 * consumed here; ovl11_collision_query_budget.h provides counted wrappers.
 *
 * queryMask enables probes, in order: 0x20 tiles, 0x8000 item pool, 0x80
 * actors, 1 primary object, 0x800/2/4/8/0x200/0x100/0x400 placed-record
 * pools, 0x10 region limits, 0x1000 special cells. Bits 2/4 are suppressed
 * outside the shared-work mode zero. Bit 0x40 accepts tile result 2 rather
 * than treating it as blocked; 0x20000 selects alternate region limits.
 *
 * hitObject/hitFlags are writable output locations, not initialized here.
 * Some probes overwrite flags, others OR bits into the existing word. A tile
 * probe can write outputs and still return zero; the region-limit probe can
 * return one WITHOUT writing either output. A zero result does not promise
 * unchanged outputs. Initialize them when the caller needs defined defaults.
 * Actor probing can clear per-record flag 4 even when no hit is returned.
 * Uses shared map/item/actor state; not reentrant with its writers. */
s32 Ovl11DispatchCollisionProbes(s32 *position, Ovl11CollisionTag tag,
                               void **hitObject, s32 *hitFlags,
                               s32 excludedRecord, s32 queryMask);

/* Tile-only entry used by the offset/retry query builder. queryWords points
 * to an Ovl11CollisionQuery via its original pointer-word view; it is NOT an
 * arbitrary pointer array. Position/tag/output pointers and field_18 must be
 * initialized by the caller. Returns 0, 1 or 2; may update outputs on zero.
 * columnWork is overwritten by the computed byte offset; its incoming value
 * does not control the test. Neither entry validates indices or pointers. */
s32 Ovl11ProbeTileMap(s32 **queryWords, s32 columnWork);

#endif
