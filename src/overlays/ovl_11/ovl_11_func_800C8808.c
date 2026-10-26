#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "game_flags.h"
#include "ovl11_primary_object_requests.h"

struct_80076220 *ovl_11_func_800BF1D0(s16 *arg0, void *arg1);
s32 ovl_11_func_80118A68(s32 arg0);
void ovl_11_func_800F0FB0(s32 arg0);

s32 ovl_11_func_800C8808(Ovl11C8808Object *arg0, s16 *arg1, s16 *arg2) {
    s16 temp_v1;
    struct_80076220 *temp_v0;

    temp_v0 = ovl_11_func_800BF1D0(arg1, arg0->point);
    if (temp_v0 != NULL) {
        temp_v1 = arg2[2];
        if (temp_v1 == 0x2E) {
            if (*(s16 *) temp_v0 != ((struct Ovl11C8808State *) D_8006C838)->selected) {
                return 0;
            }
        } else if (*(s16 *) temp_v0 != temp_v1) {
            return 0;
        }
        if ((arg0->unk38 == arg2[4]) || (arg2[4] == 5)) {
            Ovl11ResetPrimaryObjectRequest();
            if ((*(s16 *) temp_v0 == 6) && (((struct Ovl11SpriteWork *) &D_8007AFF0)->header[1] == 0x1B)) {
                ovl_11_func_80118A68(0x16);
                return 1;
            }
            arg0->unk6C |= 0x800000;
            ovl_11_func_800F0FB0(3);
            arg0->unk22 = *(u16 *) temp_v0;
            SetGameFlag(0x46U, 1U);
            return 1;
        }
        return 0;
    }
    return 0;
}
