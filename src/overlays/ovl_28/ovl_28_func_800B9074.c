#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"

void ovl_28_func_800B9074(void) {
    POLY_F4 *poly;
    DR_MODE *mode;
    u_long *ot;
    s32 tpage;
    Ovl28B9628View *rect;

    if (D_800B9626 != 0) {
        if (D_800B9626 == 1) {
            D_800BAB0C -= 4;
            if (D_800BAB0C <= 0) {
                D_800BAB0C = 0;
            }
        } else if (D_800B9626 == 2) {
            D_800BAB0C += 4;
            if (D_800BAB0C >= 255) {
                D_800BAB0C = 255;
            }
        }
        poly = (POLY_F4 *)D_800BAB10;
        SetPolyF4(poly);
        setRGB0(poly,
            D_800BAB0C * 180 / 255 + 75,
            D_800BAB0C * 155 / 255 + 100,
            D_800BAB0C * 105 / 255 + 150);
        rect = &D_800B9628;
        setXYWH(poly, rect->unk0, rect->unk2, rect->unk4, rect->unk6);
        SetSemiTrans(poly, 1);
        ot = (u_long *)D_8005E3C0->field_D8;
        addPrim(ot + 3, poly);
        tpage = GetTPage(0, 2, 0, 0);
        mode = (DR_MODE *)D_800BAB30;
        SetDrawMode(mode, 0, 1, tpage, NULL);
        addPrim(ot + 3, mode);
        if (D_800BAB0C == 0 || D_800BAB0C == 255) {
            D_800B9626 = 0;
        }

    }
}
