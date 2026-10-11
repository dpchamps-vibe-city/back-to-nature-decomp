#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"




s32 ovl_11_func_80111400(s16 arg0) {
    switch (arg0) {
    case 0x46:
        return 0xC;
    case 0x47:
        return 0xD;
    case 0x48:
        return 0xE;
    case 0x49:
        return 0xF;
    case 0x4A:
        return 0x10;
    case 0x52:
        return 0x11;
    case 0x56:
        return 0x12;
    case 0xE8:
        return 0x1C;
    case 0x8B:
        return 0x1D;
    case 0x8C:
        return 0x1E;
    case 0x8D:
        return 0x1F;
    case 0xE3:
        return 0x20;
    case 0x91:
        return 0x21;
    case 0x92:
        return 0x22;
    case 0x93:
        return 0x23;
    case 0xE7:
        return 0x24;
    case 0x122:
    case 0x123:
    case 0x124:
        return 0x1B;
    }
    return -1;
}
