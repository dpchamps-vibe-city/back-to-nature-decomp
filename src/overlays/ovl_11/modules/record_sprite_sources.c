/*
 * record_sprite_sources — reconstructed translation unit.
 *
 * Install the loaded record set into the shared sprite-source bank. The
 * count-prefixed offset list is resolved against the loaded sprite data blob;
 * each selected header initializes one 0x30-byte source record.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sprite_source.h"
#include "ovl11_record_sprite_sources.h"

void Ovl11InstallRecordSpriteSources(s32 recordSet) {
    SpriteSourceData *destination;
    s32 count;
    s32 index;
    s32 entryAddress;

    count = *gOvl11SpriteRecordSets[recordSet];
    destination = &gSpriteSourceBankBase;
    if (count >= 0x2D) {
        count = 0x2C;
    }
    index = 0;
    if (count > 0) {
        do {
            entryAddress = index * 4;
            index += 1;
            entryAddress = (s32)((u8 *)gOvl11SpriteRecordSets[recordSet] + entryAddress);
            /* Word zero is the count; the following words are blob offsets. */
            InitSpriteSourceData(destination,
                (SpriteDataHeader *)(((s32 *)entryAddress)[1] + (s32)gLoadedSpriteDataBlob));
            destination += 1;
        } while (index < count);
    }
}
