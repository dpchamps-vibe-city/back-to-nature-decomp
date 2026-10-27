#include "common.h"
#include "ovl11_selection_record_reset.h"

s32 ovl_11_func_8011B210(void) {
    Ovl11ClearSelectionRows((s32 *) gOvl11PrimarySelectionRecord);
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5A)) = 0xB;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5B)) = 0x10;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x28)) = 0x9F;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x5C)) = 0;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3C)) = 0;
    (*(s8 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x3D)) = 0;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x46)) = 0xB2;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x2A)) = 0xA0;
    (*(s16 *) ((u8 *) gOvl11PrimarySelectionRecord + 0x48)) = 0xB3;
    gOvl11InteractionDrawMode = 0;
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 0)) = (void *) ((u8 *) D_80051AA8 + *D_80054BC0);
    (*(void **) ((u8 *) gOvl11PrimarySelectionRecord + 4)) = (void *) ((u8 *) (D_80051AA8 + 0xA) + *D_80054BC0);
    return 1;
}
