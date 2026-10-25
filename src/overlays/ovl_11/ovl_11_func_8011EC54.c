#include "common.h"
#include "sound_commands.h"

void func_80022738(void);

void ovl_11_func_8011EC54(void) {
    func_80022738();
    PlaySoundEffect(3);
    D_80128420 = 8;
}
