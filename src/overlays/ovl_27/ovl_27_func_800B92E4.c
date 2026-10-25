#include "common.h"
#include "game_types.h"
#include "ovl27_backdrop_sprite_pool.h"

void ovl_27_func_800B92E4(void) {
    Ovl27DrawTiledBackdrop();
    Ovl27UpdateEffectSlot0();
    Ovl27UpdateEffectSlot1();
    Ovl27UpdateEffectSlot2();
    Ovl27UpdateEffectSlots3To5();
}
