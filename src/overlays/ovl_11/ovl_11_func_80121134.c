#include "common.h"
#include "game_types.h"

s32 ovl_11_func_80120CC4(Ovl11TextureCoordinates *coordinates, s32 arg1);
s32 ovl_11_func_80120F30(s16 column, s16 row, Ovl11TextureCoordinates *coordinates);

void ovl_11_func_80121134(void) {
    s32 row;
    s32 column;

    for (row = 0; row < 25; row++) {
        for (column = 0; column < 45; column++) {
            if (ovl_11_func_80120CC4(&D_8012DB38,
                    ((Ovl11D93C8MapView *) D_8006C838)->cells[row][column].kind) == 1) {
                ovl_11_func_80120F30(column, row, &D_8012DB38);
            }
        }
    }
}
