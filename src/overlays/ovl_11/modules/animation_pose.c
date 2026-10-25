/* Handler-13 deferred pose protocol and the shared pose-update service.
 * These callbacks share the caller's flags/pose state, not private storage.
 * Their boundary to the fixed playlist band is documented in
 * notes/file-groupings.md; the callback contracts are in the public header. */
#include "ovl11_animation_pose.h"

void ovl_11_func_800D12B8(Recon_ovl_11_func_800D2594_A0View *object,
                        s32 period, s32 mode);

s32 Ovl11SubmitDeferredAnimationPose(Recon_ovl_11_func_800E0220_A0View *object) {
    object->unk34 = object->unk34 | 0x800;
    if (((object->unk34 | 0x800) & 0x2000) == 0) {
        object->unk34 = (object->unk34 | 0x800) & -0x801;
        object->unk2C = 0x12C;
        return 1;
    } else {
        object->unk2E = 0;
        Ovl11SubmitAnimationPose((Recon_ovl_11_func_800D2594_A0View *)object, (s32)object);
        return 0;
    }
}

s32 Ovl11SubmitAnimationPose(Recon_ovl_11_func_800D2594_A0View *object, s32 unused) {
    s32 isHandler13;
    s32 period;
    s32 mode;
    s32 selector;

    isHandler13 = (*(s16 *)((u8 *)object + 0x26)) == 0xD;
    selector = *(u16 *)((u8 *)object + 0xB2);
    period = selector ? 2 : 6;
    mode = 0x8031;
    if (isHandler13 != 0) {
        mode = 0x8030;
        if (selector == 0) {
            object->unk34 |= 0x04000000;
        }
    } else if ((((struct Ovl11HandlerState *)D_8006C838)->field_52C6 == 5) && (((struct Ovl11HandlerState *)D_8006C838)->field_E4C8 == 0)) {
        if (selector == 0) {
            object->unk34 |= 0x04000000;
        }
    } else {
        object->unk34 &= 0xFBFFFFFF;
    }
    ovl_11_func_800D12B8(object, period, mode);
    if (((*(s32 *)((u8 *)object + 0x48)) == 0) && (object->unk4C == 0) && ((*(s32 *)((u8 *)object + 0x50)) == 0)) {
        if (isHandler13 != 0) {
            object->unk34 &= ~0x2000;
        }
        (*(s16 *)((u8 *)object + 0x24)) = 0;
    } else {
        (*(s16 *)((u8 *)object + 0x24)) = 1;
    }
    object->unk34 &= ~0x800;
    return 1;
}
