#include "common.h"
#include "game_types.h"
#include "pad_actuator_sequences.h"
#include "sound_commands.h"

u32 Rand(s32 arg0);
void func_80015840(ObjectState *obj, u8 arg1);
s32 ovl_11_func_800CD3C4(s32 arg0);
s32 ovl_11_func_800D622C(s8 arg0, s8 arg1, s16 arg2, s32 arg3);
void ovl_11_func_800CD08C(Recon_ovl_11_func_800CD08C_A0View *arg0);
s32 ovl_11_func_800CB540(u16 *arg0);
s16 ovl_11_func_80120358(Ovl11SelectArg *arg0);
s16 ovl_11_func_80120414(s32 *arg0);
void ovl_11_func_80120850(s32 arg0);

s32 ovl_11_func_8011FFA8(Ovl11SelectionObject *obj) {
    s32 *state;
    u16 flags;
    s32 chance;
    s32 threshold;
    s16 item[3];
    struct Ovl11SpriteWork *work;

    state = D_8012DB48;
    flags = *(u16 *)&D_801231F4;
    switch (obj->phase) {
    case 0x45:
        if (obj->flags & 0x40) return 0;
        obj->phase = 0;
        state[2] = 0;
        if (!(obj->flags & 8)) {
            ovl_11_func_800D622C(-1, (s8)ovl_11_func_800CD3C4(0), 0, 0);
        }
        work = (struct Ovl11SpriteWork *)&D_8007AFF0;
        state[2] = 4;
        if (work->header[1] == 0x2C) state[2] = 0;
        if (work->header[1] == 0xD) state[2] = 1;
        if (work->header[1] == 0xC) state[2] = 2;
        if (work->header[1] == 0x42) state[2] = 3;
        state[1] = ovl_11_func_80120358((Ovl11SelectArg *)state);
        if (D_8012DB80 == 0) {
            state[2] = -1;
            obj->phase = 3;
            func_80015840(&obj->animation, (u8)(obj->direction + 4));
            StopPadActuatorSequence(0);
            PlaySoundEffect(15);
        } else {
            D_8012DB80 = 0;
            RequestPadActuatorSequence(0, 6);
        }
        break;
    case 0:
        if (obj->flags & 0x40) return 0;
        if (!(flags & 0x80)) {
            obj->phase = 2;
            func_80015840(&obj->animation, (u8)(obj->direction + 4));
            PlaySoundEffect(15);
        } else {
            if (state[1] == 0) {
                chance = 10;
                if (obj->id == 0x39) chance = 20;
                threshold = chance * 2;
                if ((s32)Rand(100) < threshold) {
                    obj->phase = 1;
                    func_80015840(&obj->animation, (u8)(obj->direction + 9));
                    RequestPadActuatorSequence(0, 0);
                    return 0;
                }
                state[1] = ovl_11_func_80120358((Ovl11SelectArg *)state);
            } else if (state[2] != -1) {
                state[1]--;
            }
            func_80015840(&obj->animation, 8);
            obj->animation.field_5 = obj->direction;
            ovl_11_func_80120850(0);
        }
        break;
    case 2:
        if (obj->animation.field_2 & 0x100) {
            obj->flags &= ~0x30;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *)obj);
        }
        break;
    case 3:
        if (obj->animation.field_2 & 0x100) {
            item[2] = 1;
            obj->flags &= ~0x30;
            ovl_11_func_800CD08C((Recon_ovl_11_func_800CD08C_A0View *)obj);
            if (state[2] != -1) {
                item[0] = ovl_11_func_80120414(state);
                if (item[0] != 0) ovl_11_func_800CB540((u16 *)item);
            }
        }
        break;
    case 1:
        if (!(flags & 0x80)) {
            obj->phase = 3;
            func_80015840(&obj->animation, (u8)(obj->direction + 4));
            StopPadActuatorSequence(0);
            PlaySoundEffect(15);
        } else if (state[0] == 0) {
            obj->phase = 0;
            state[1] = ovl_11_func_80120358((Ovl11SelectArg *)state);
            StopPadActuatorSequence(0);
        } else {
            state[0]--;
            ovl_11_func_80120850(1);
        }
        break;
    }
    return 0;
}
