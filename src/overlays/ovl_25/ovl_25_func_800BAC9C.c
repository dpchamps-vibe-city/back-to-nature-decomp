#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "psyq/memory.h"
#include "cd_loader.h"

void func_8001719C(u8 *arg0);
/* Only the two consumed arguments are supplied by this caller. */
void func_80015704();
void func_80015840(ObjectState *obj, u8 arg1);

void ovl_25_func_800BAC9C(void) {
    DrawSync(0);
    ClearOTagR((u_long *)gActiveRenderContext->field_120, 0x800);
    CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 1);
    do {
    } while (CdReadArchiveBytes(0, 0x25000, 0x5800, (u8 *)(gGraphicsHeapBase + 0x4290), 1, 0) == 0);
    func_8001719C((u8 *)(gGraphicsHeapBase + 0x566C));
    memcpy(D_800BCDD4, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    memcpy(&gSharedSpriteData, (void *)(gGraphicsHeapBase + 0x4290), 0x13DC);
    func_80015704((SpriteSourceData *)D_800C021C, (SpriteDataHeader *)D_800BCDD4);
    func_80015840((ObjectState *)D_800C021C, 0x17);
}
