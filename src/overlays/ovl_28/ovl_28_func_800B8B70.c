#include "common.h"
#include "game_types.h"
#include "psyq/memory.h"
#include "cd_loader.h"

/* Callee prototypes as this caller TU saw them. func_80015704's real
 * definition takes four parameters, but this caller passes two (the a2/a3
 * slots are left holding the last copied words), matching func_800229F4. */
void func_8001719C(u8 *arg0);
void func_80015704(SpriteSourceData *out, SpriteDataHeader *header);
void func_80015840(ObjectState *obj, u8 arg1);

s32 ovl_28_func_800B8B70(void) {
    if (CdReadArchiveBytes(0, 0x03D77000, 0x6000, (u8 *)(D_8005E3B0 + 0x4290), 1, 0) != 0) {
        func_8001719C((u8 *)(D_8005E3B0 + 0x4900));
        memcpy(&D_800B9750, (void *)(D_8005E3B0 + 0x4290), 0x670);
        func_80015704((SpriteSourceData *)D_800B9720, (SpriteDataHeader *)&D_800B9750);
        func_80015840((ObjectState *)D_800B9720, 0);
        return 1;
    }
    return 0;
}
