#include "common.h"
#include "game_types.h"

s32 ovl_11_func_800D740C(u16 arg0);
void ovl_11_func_800CBB9C(Ovl11C7010Object *arg0, s32 arg1, s32 arg2);
void ovl_11_func_800F3898(u16 arg0, u16 arg1);
void ovl_11_func_8011D084(s32 arg0, s32 arg1);

void ovl_11_func_801168C0(StructD548 *arg0) {
    s16 selection;
    s16 threshold;

    gOvl11InteractionDrawMode = 0;
    threshold = ovl_11_func_800D740C((u16)arg0->data[arg0->index]);
    ((Ovl11SelectedThresholdState *)D_8006C838)->value -= threshold;
    selection = arg0->index;
    if (selection == 0) {
        ((Ovl11SelectedThresholdState *)D_8006C838)->flags |= 0x10;
        ovl_11_func_800CBB9C((Ovl11C7010Object *)((Ovl11SelectedThresholdState *)D_8006C838)->object, 0x67, 2);
        ovl_11_func_8011D084(9, 0xB);
    } else if (selection == 1) {
        ovl_11_func_800F3898(3, 0x1F4);
        ovl_11_func_8011D084(9, 0xB);
    } else {
        ovl_11_func_8011D084(9, 0xC);
    }
}
