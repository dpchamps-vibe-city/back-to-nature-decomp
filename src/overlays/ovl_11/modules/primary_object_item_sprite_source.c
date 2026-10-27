/*
 * primary_object_item_sprite_source — reconstructed translation unit.
 *
 * Bind the embedded sprite source at object +0x260 from its code and a
 * private byte-offset table, optionally loading the indexed sprite bank
 * first.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_primary_object_item_sprite_source.h"
#include "sprite_source.h"
#include "ovl11_area_resource_load.h"

/* Only the object and load flag are read; the held-item caller supplies two
 * arguments. Live a2/a3 at other call sites are not additional parameters. */
void Ovl11InitPrimaryObjectItemSpriteSource(
    Recon_ovl_11_func_800CCFF8_A0View *object, s32 loadBank) {
    SpriteSourceData *source;
    s16 bankIndex;

    bankIndex = object->objectCode - 0x3B;
    if (loadBank != 0) {
        Ovl11LoadLargeIndexedSpriteBank(bankIndex);
    }
    source = (SpriteSourceData *) ((u8 *) object + 0x260);
    InitSpriteSourceData(source, (SpriteDataHeader *) D_8007BFF8);
    SetSpriteSourceRecordAddress((SomeStruct *) source,
        (s32) &D_8007BFF8[gOvl11ItemSpriteRecordOffsets[bankIndex]]);
    SelectSpriteAnimation((ObjectState *) source, 0);
}
