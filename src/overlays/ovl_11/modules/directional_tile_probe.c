/* Directional tile-map sampling, independent of the volume/exit-table
 * searches on either side in original link order. The second sample refines
 * only result 2; it never commits a position change. See the public contract
 * in ovl11_directional_tile_probe.h. Original ovl_11:800BDF98–800BE118. */
#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_collision_probes.h"
#include "ovl11_directional_tile_probe.h"

#include "ovl11_grid_position.h"

s32 Ovl11ProbeDirectionalTile(VECTOR position, Ovl11CollisionTag tag,
                             s16 direction, s32 secondSample) {
    Ovl11CollisionQuery query;
    u16 column;
    u16 row;
    void *tile;
    void **tilePtr = &tile;
    s32 flags;
    s32 result;
    s32 checkedResult;

    query.position = &position.vx;
    query.object = &tag;
    query.tile = (void **) &tilePtr;
    query.flags = &flags;
    query.field_C = 0;
    query.field_14 = 0;
    Ovl11LookupMode1GridPosition(&position.vx, &column, &row);
    if ((s16) column == 0x2C && direction == 3) {
        return 1;
    }
    checkedResult = Ovl11ProbeTileMap((s32 **) &query, 0);
    result = checkedResult;
    if (checkedResult == 2) {
        if (secondSample == 0) {
            switch (direction) {
            case 0:
                position.vz -= 0x190;
                break;
            case 1:
                position.vx -= 0x190;
                break;
            case 2:
                position.vz += 0x190;
                break;
            case 3:
                position.vx += 0x190;
                break;
            }
            if (Ovl11ProbeDirectionalTile(position, tag, direction, 1) == 0) {
                return 2;
            }
            result = 1;
        } else {
            return 2;
        }
    }
    return result;
}
