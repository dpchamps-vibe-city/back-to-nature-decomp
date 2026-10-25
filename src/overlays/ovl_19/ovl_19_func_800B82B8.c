#include "common.h"
#include "random.h"

void ovl_19_func_800B843C(void);

void ovl_19_func_800B82B8(void) {
    s32 i;
    s32 j;
    s32 r;

    i = 0;
    gOvl19SceneWork[2] = 0;
    gOvl19SceneWork[3] = 0;
    gOvl19SceneWork[4] = 0;
    gOvl19SceneWork[5] = 0;
    for (; i < 8; i++) {
        (*(gOvl19SceneWork + 0xB0 + i)) = 0xFF;
        r = Rand(0x14);
        for (j = 0; j <= i; j++) {
            if (gOvl19SceneWork[0xB0 + j] == r) {
                r++;
                if (r >= 0x14) {
                    r = 0;
                }
                j = 0;
            }
        }
        (*(gOvl19SceneWork + 0xB0 + i)) = r;
    }
    ovl_19_func_800B843C();
    gOvl19SceneWork[0xB9] = 0x2D;
    gOvl19SceneWork[0xBA] = 0x190;
    gOvl19SceneWork[0xB8] = 6;
    gOvl19SceneWork[0xBE] = 0x50;
    gOvl19SceneWork[0xBF] = 0xA;
    gOvl19SceneWork[0xC0] = 0xA;
    gOvl19SceneWork[0xC1] = 0x7D;
    gOvl19SceneWork[0xC2] = 0x32;
    gOvl19SceneWork[0xC3] = 0x73;
    gOvl19SceneWork[0xC4] = 0x3C;
    gOvl19SceneWork[0xC5] = 0x73;
    gOvl19SceneWork[0xBB] = 2;
    gOvl19SceneWork[0xBC] = 0x5A;
    gOvl19SceneWork[0xBD] = 0xF;
    gOvl19SceneWork[0xC6] = 0x3C;
    gOvl19SceneWork[0xC7] = 0x5A;
    gOvl19SceneWork[0xC8] = 0x78;
    gOvl19SceneWork[0xC9] = 0x5A;
    gOvl19SceneWork[0xCA] = 0x1E;
    gOvl19SceneWork[0xCB] = 0xA;
    gOvl19SceneWork[0xCC] = 0xA;
    gOvl19SceneWork[0xCD] = 0xB;
    gOvl19SceneWork[0xCE] = 0xA;
    gOvl19SceneWork[0xCF] = 0xB;
}
