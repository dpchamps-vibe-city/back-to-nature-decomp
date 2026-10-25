#include "common.h"
#include "game_types.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B9224(void) {
    Ovl27DrawBackdropAnim31(0);
    Ovl27DrawBackdropAnim32(0);
    Ovl27DrawTiledBackdrop();
    Ovl27UpdateEffectSlot0();
    Ovl27UpdateEffectSlot1();
    Ovl27UpdateEffectSlot2();
    Ovl27UpdateEffectSlots3To5();
    Ovl27DrawBackdropAnim36();
    Ovl27DrawBackdropAnim34();
    Ovl27DrawBackdropAnim35();
}
