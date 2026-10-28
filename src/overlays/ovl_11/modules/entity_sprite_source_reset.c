/*
 * entity_sprite_source_reset — reconstructed translation unit.
 *
 * Unbind all ten entity sprite sources and clear their parallel spawn
 * records, then suppress the current-object sprite until reinitialized.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "ovl11_object_load_resets.h"
#include "ovl11_spawn_record_access.h"

void Ovl11ResetEntitySpriteSources(void) {
    s32 *sourceHeader;
    s32 *work;
    s32 *farWork;
    s32 slot;

    slot = 9;
    sourceHeader = (s32 *)gOvl11EntitySpriteSources;
    /* Last slot's +14 header address: 9 * 30 + 14, in hexadecimal. */
    sourceHeader = (s32 *)((char *)sourceHeader + 0x1C4);
    while (slot >= 0) {
        *sourceHeader = 0;
        sourceHeader = (s32 *)((char *)sourceHeader - 0x30);
        slot--;
    }
    work = (s32 *)&D_8006C838;
    farWork = work + (0x8000 >> 2);
    Ovl11ClearSpawnRecords((Ovl11DispatchRecord *)farWork[0x5DD0 >> 2],
                          *(s16 *)((char *)farWork + 0x5DFA));
    *(s16 *)((char *)work + 0x4476) = 1;
}
