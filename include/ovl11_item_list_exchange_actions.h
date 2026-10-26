#ifndef OVL11_ITEM_LIST_EXCHANGE_ACTIONS_H
#define OVL11_ITEM_LIST_EXCHANGE_ACTIONS_H

#include "common.h"
#include "game_types.h"

/* Operate on the shared primary-object lists, while inspecting/publishing the
 * supplied object's fields (callers pass that same primary object). Both lists
 * contain eight six-byte records. The direction argument is ignored: both
 * always exchange from the first entry, even when the caller supplies 1.
 * No return value reports whether anything happened.
 * Stack list +90/selected +84: accept empty or eligible selection, compact,
 * exchange, then state 0x15/animation 0x47/timing 0xA if selected is nonempty.
 * Secondary +C0/selected +8A: an eligible nonempty selection services its
 * pending request instead; an empty selection exchanges then starts state 0xA.
 * Both clear request bits 18000000 before exchange, copy selected to action
 * payload +F0, set flag 8000 and play sound 7 on the action path. The fixed
 * exchange wrappers return 1, so the inherited != -1 tests always pass.
 * Requires initialized primary object/item metadata; no allocation. */
void Ovl11TryBeginStackListItemAction(Recon_ovl_11_func_800C7270_A0View *object,
                                     s32 unusedDirection);
void Ovl11TryBeginSecondaryListItemAction(Recon_ovl_11_func_800C7270_A0View *object,
                                         s32 unusedDirection);

#endif
