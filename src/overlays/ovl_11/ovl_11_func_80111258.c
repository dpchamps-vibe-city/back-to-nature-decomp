#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"




s32 ovl_11_func_80111258(s16 arg0) {
    switch (arg0) {
    case 0x41:
        return 7;
    case 0x42:
        return 8;
    case 0x43:
        return 9;
    case 0x44:
        return 0xA;
    case 0x45:
        return 0xB;
    case 0x51:
        return 0xC;
    case 0x55:
        return 0xD;
    case 0xE8:
        return 0x17;
    case 0x8B:
        return 0x18;
    case 0x8C:
        return 0x19;
    case 0x8D:
        return 0x1A;
    case 0xE3:
        return 0x1B;
    case 0x91:
        return 0x1C;
    case 0x92:
        return 0x1D;
    case 0x93:
        return 0x1E;
    case 0xE7:
        return 0x1F;
    case 0x122:
    case 0x123:
    case 0x124:
        return 0x16;
    }
    return -1;
}
