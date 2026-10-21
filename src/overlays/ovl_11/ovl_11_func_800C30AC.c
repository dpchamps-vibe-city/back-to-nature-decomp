#include "common.h"
#include "game_types.h"

void ovl_11_func_800C2F74(Ovl11AnimationObject *arg0);
void ovl_11_func_800C3590(Ovl11AnimationObject *arg0);

void ovl_11_func_800C30AC(void) {
    s32 i;
    struct_80076220 *obj;

    obj = &D_80076220;
    D_80122F88++;
    if (!(D_8006C848 & 0x02000000)) {
        for (i = 0; i < 37; i++) {
            ovl_11_func_800C2F74((Ovl11AnimationObject *) obj);
            if (!(obj->unk1E & 0x1000)) {
                if (obj->unk1E & 0x100) {
                    ovl_11_func_800C3590((Ovl11AnimationObject *) obj);
                }
            }
            obj++;
        }
    }
}
