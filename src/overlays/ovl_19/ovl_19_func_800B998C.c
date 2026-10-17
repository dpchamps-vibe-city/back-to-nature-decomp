#include "common.h"
#include "game_types.h"

void ovl_19_func_800B9AB0(s16 *arg0);
void ovl_19_func_800B9DC8();
s32 ovl_19_func_800B9DD0(Recon_ovl_19_func_800B9DD0_A0View *arg0);
void ovl_19_func_800B9E14(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800B9E78(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800B9EDC(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800B9F40(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800B9FB0(Recon_ovl_19_func_800BA0A0_A0View *arg0);
void ovl_19_func_800BA054(Recon_ovl_19_func_800BA054_A0View *arg0);

void ovl_19_func_800B998C(void) {
    s16 *temp_s0;
    s16 temp_v1;
    s32 var_s1;

    for (var_s1 = 0; var_s1 < 2; var_s1++) {
        temp_s0 = (s16 *) ((u8 *) D_800BF4D0 + (var_s1 * 0x48));
        temp_v1 = *(s16 *) ((u8 *) temp_s0 + 2);
        switch (temp_v1) {
        case 0:
        case 1:
        case 2:
            break;
        case 3:
            ovl_19_func_800B9AB0(temp_s0);
            break;
        case 4:
            ovl_19_func_800B9DC8(temp_s0);
            break;
        case 5:
            ovl_19_func_800B9DD0((Recon_ovl_19_func_800B9DD0_A0View *) temp_s0);
            break;
        case 6:
            ovl_19_func_800B9E14((Recon_ovl_19_func_800BA0A0_A0View *) temp_s0);
            break;
        case 7:
            ovl_19_func_800B9E78((Recon_ovl_19_func_800BA0A0_A0View *) temp_s0);
            break;
        case 8:
            ovl_19_func_800B9EDC((Recon_ovl_19_func_800BA0A0_A0View *) temp_s0);
            break;
        case 9:
            ovl_19_func_800B9F40((Recon_ovl_19_func_800BA0A0_A0View *) temp_s0);
            break;
        case 10:
            ovl_19_func_800B9FB0((Recon_ovl_19_func_800BA0A0_A0View *) temp_s0);
            break;
        case 11:
            ovl_19_func_800BA054((Recon_ovl_19_func_800BA054_A0View *) temp_s0);
            break;
        }
        if ((*(s16 *) ((u8 *) temp_s0 + 6)) > 0) {
            (*(s16 *) ((u8 *) temp_s0 + 6))--;
        }
    }
}
