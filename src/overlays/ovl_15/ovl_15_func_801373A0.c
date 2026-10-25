#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"

/* ovl_15 sprite-data loader.
 *
 * Waits for the display to drain, clears the ordering table, loads the
 * 0x8E90-byte ovl_15 sprite-data blob into D_80137830 (polling
 * func_80014CBC until the transfer completes), runs func_8001719C over the
 * blob's tail, then copies the 0x2D74-byte blob body ending at D_8013A5A4
 * into D_801428F0. Finally it initializes the five 0x30-byte
 * SpriteSourceData records at D_80145670 from the D_801428F0 blob, using the
 * D_8013766C offset table to locate each sprite-data header. */
void func_8001719C(u8 *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
u8 *func_80014CBC(s32 arg0, s32 arg1, s32 arg2, u8 *arg3, s32 arg4, s32 arg5);

void ovl_15_func_801373A0(void) {
    u32 i;

    DrawSync(0);
    ClearOTagR((u_long *) gActiveRenderContext->field_120, 0x800);
    func_80014CBC(0xE, 0, 0x8E90, D_80137830, 1, 1);
    do {

    } while (func_80014CBC(0xE, 0, 0x8E90, D_80137830, 1, 0) == 0);
    func_8001719C(D_8013A5A4);
    memcpy(D_801428F0, &D_8013A5A4[-0x2D74], 0x2D74);
    for (i = 0; i < 5; i++) {
        func_80015704(&D_80145670[i], (SpriteDataHeader *) &D_801428F0[D_8013766C[i]]);
    }
}
