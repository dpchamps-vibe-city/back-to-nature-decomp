#ifndef OVL11_PRIMARY_OBJECT_ITEM_SPRITE_SOURCE_H
#define OVL11_PRIMARY_OBJECT_ITEM_SPRITE_SOURCE_H

#include "common.h"
#include "game_types.h"

/* Initialize the sprite source at object+0x260 and select animation zero.
 * The unsigned code at +0x3C selects bank/index (code-0x3B), narrowed to s16.
 * Any nonzero loadBank loads that indexed bank first; zero reuses its current
 * contents at 8007BFF8. The private u16 table gives BYTE offsets into the bank.
 * Caller must supply a valid code, loaded bank, and enough object storage for
 * the embedded 0x30-byte source; no bounds/header/storage checks are added here.
 * This shares bank storage and is not reentrant. Exactly two ABI arguments. */
void Ovl11InitPrimaryObjectItemSpriteSource(
    Recon_ovl_11_func_800CCFF8_A0View *object, s32 loadBank);

#endif
