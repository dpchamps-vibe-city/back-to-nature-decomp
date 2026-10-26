#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "ovl11_interaction_volume_query.h"
#include "ovl11_surface_height.h"

void ovl_11_func_800F4FC8(void);
void ovl_11_func_800F4CEC(void);

void ovl_11_func_800F4E84(void) {
    Ovl11Func800BDCE8Arg1 *position;
    s32 hit;
    Ovl11PoolGateRecord *entry;
    s32 type;
    u8 *base;

    if (((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_0 == 0 &&
        (((Ovl11PoolGateState *)D_8006C838)->flags & 0x02000000)) {
        entry = ((Ovl11PoolGateState *)D_8006C838)->entry;
        position = (Ovl11Func800BDCE8Arg1 *) entry;
        if (((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2 != (*(s16 *) ((u8 *) position + 0x18))) {
            return;
        }
        position = (Ovl11Func800BDCE8Arg1 *) ((u8 *)position + 0x20);
        hit = Ovl11ProbeInteractionVolume(((Ovl11FarInitView *)&D_8007AFF0)->data, position, 0) != NULL;
        if (Ovl11ResolveSurfaceHeight(&position->unk0, 0xC000) != 0) {
            hit = 1;
        }
        if (hit != 0) {
            ovl_11_func_800F4FC8();
        }
    }
    if (((Ovl11PoolGateState *)D_8006C838)->flags & 0x800) {
        base = (u8 *)D_8006C838;
        base += 0x8000;
        type = ((Ovl11CollisionWorkView *)&D_8007AFF0)->collision.field_2;
        entry = *(Ovl11PoolGateRecord **)(base + 0x5DD4);
        if (type == entry->type) {
            position = (Ovl11Func800BDCE8Arg1 *) entry->position;
            hit = Ovl11ProbeInteractionVolume(((Ovl11FarInitView *)&D_8007AFF0)->data, position, 0) != NULL;
            if (Ovl11ResolveSurfaceHeight(&position->unk0, 0xC000) != 0) {
                hit = 1;
            }
            if (hit != 0) {
                ovl_11_func_800F4CEC();
            }
        }
    }
}
