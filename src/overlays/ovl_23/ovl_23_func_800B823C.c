#include "common.h"
#include "ovl23_record_trails.h"

void ovl_23_func_800B8290(void);
void ovl_23_func_800B8354(void);

void ovl_23_func_800B823C(void) {
    struct Ovl23ScreenHeader *state = &Ovl23_D_800BF87C_header;

    state->unk4 = 0;
    state->unk8 = 0;
    state->unkC = 0;
    state->unk414 = -1;
    state->unk10 = 0x40000;
    state->unk14 = 0;
    ovl_23_func_800B8290();
    ovl_23_func_800B8354();
    Ovl23ResetRecordTrails();
}
