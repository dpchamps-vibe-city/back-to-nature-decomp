/* Adapt an object resource ID to the retained sprite-block bank's index.
 * This is GPU readback, not an archive read or a new buffer allocation. */

#include "common.h"
#include "ovl11_sprite_block_bank.h"
#include "ovl11_object_sprite_recovery.h"

u8 *Ovl11RecoverObjectSpriteBlock(u16 *scratch, s32 resourceId) {
    resourceId -= 0x15F;
    return Ovl11ReadSpriteBlock(scratch, gOvl11SpriteBankVramXOffset, 0, resourceId);
}
