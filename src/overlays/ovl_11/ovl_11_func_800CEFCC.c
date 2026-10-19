#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"

s32 ovl_11_func_800BDB88(s32 *arg0, s32 arg1);

s32 ovl_11_func_800CEFCC(Ovl11CEFCCObject *arg0, Ovl11CollisionTag *arg1, VECTOR *arg2) {
    arg0->flags &= ~0x20000;
    arg0->collision = *arg1;
    *(VECTOR *)arg0->position = *arg2;
    ovl_11_func_800BDB88((s32 *)&arg0->position, 0);
    return 1;
}
