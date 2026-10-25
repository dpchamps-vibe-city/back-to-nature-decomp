#include "common.h"
#include "game_types.h"
#include "screen_fade.h"
#include "sound_commands.h"
#include "ovl11_clock_calendar.h"
#include "message_window.h"

s32 ovl_11_func_800DD1D0 (s16 *arg0);
void ovl_11_func_800CCF58 (Ovl11C7010Object *arg0);
s32 func_8001AF44 (u32 arg0);
void func_8001AF70 (u16 arg0, u16 arg1);
void ovl_11_func_800C498C (Ovl11C498CObject *arg0);
s32 ovl_11_func_800CB4E0 (void);
void ovl_11_func_800C47F0 (void);
s32 ovl_11_func_80107DEC (Ov11Timer *arg0);
void ovl_11_func_800CD6F4 (void *arg0);
s32 ovl_11_func_800C9424 (void);
s32 ovl_11_func_8011FFA8 (Ovl11SelectionObject *obj);
s32 ovl_11_func_800C6AFC (Ovl11C7010Object *arg0);
void ovl_11_func_800C70CC(Ovl11C7010Object *arg0, s32 arg1, s32 arg2, s32 arg3);
s32 ovl_11_func_800C580C(Ovl11C580CObject *);
s32 ovl_11_func_800CE654(void);

void ovl_11_func_800C401C(void) {
    Ovl11PlayerUpdate *obj;
    Ovl11UpdateInput *input;
    s32 var_a1;
    u32 temp_v1;
    s32 flags;

    obj = &((Ovl11UpdateState *)D_8006C838)->player;

    if ((*(s32 *) ((u8 *) obj + -0x51BC)) & 0x08000000) {
        if (D_80128C70 != -1) {
            StopSoundEffectVoice((s32) D_80128C70);
            D_80128C70 = -1;
        }
    } else if ((Ovl11IsClockAdvanceActive() != 1) && ((obj->flags & 0x800002) || (IsScreenFadeComplete() != 0)) && !(obj->flags & 0x400) && (ovl_11_func_800DD1D0(obj->collision) == 0)) {
        if (obj->flags & 0x2000) {
            ovl_11_func_800CCF58((Ovl11C7010Object *) obj);
            obj->flags = (s32) (obj->flags & ~0x2000);
        }
        if (func_8001AF44(0xADU) == 1) {
            func_8001AF70(0xADU, 0U);
            RequestMessageWindow(0, 0x724);
        }
        ovl_11_func_800C498C((Ovl11C498CObject *) obj);
        temp_v1 = obj->flags;
        if ((temp_v1 & 0x20000) && !(obj->flags & 0x20000010)) {
            ovl_11_func_800CB4E0();
            D_80123158 = 0;
            obj->flags = (s32) (obj->flags & 0xFFFDFFFF);
        }
        obj->flags36 = (u16) (obj->flags36 & 0xFFFB);
        if (obj->flags & 0x800000) {
            D_801231F4 = ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_522C;
            D_801231F8 = ((struct struct_8006C838_800E8F78 *)D_8006C838)->field_5230;
            goto block_22;
        }
        input = (Ovl11UpdateInput *)gPad0Input;
        D_801231F4 = input->held;
        D_801231F8 = input->pressed;
        if (ovl_11_func_800CE654() == 0) {
            ovl_11_func_800C47F0();
            if (obj->flags36 & 0x40) {
                if (D_801231F4 & 0x20) {
                    D_801231F4 &= ~0x20;
                    D_801231F8 &= ~0x20;
                } else {
                    obj->flags36 = (u16) (obj->flags36 & 0xFFBF);
                }
            }
block_22:
            ovl_11_func_80107DEC((Ov11Timer *)&obj->effectTimer);
            D_80128CB0 = 0;
            if (!(obj->flags & 0x20000000)) {
                switch ((u32)obj->flags & 0x800010) {
                case 0x800000:
                    ovl_11_func_800CD6F4(obj);
                    if (ovl_11_func_800C9424() != 0) {
                        D_80128C64 = 0;
                    }
                    flags = obj->flags;
                    if (flags & 8) {
                        temp_v1 = (u16)obj->timer;
                        if (obj->timer != 0) {
                            obj->timer = temp_v1 - 1;
                        } else {
                            goto clear_flags;
                        }
                    }
                    break;
                case 0x10:
                    ovl_11_func_8011FFA8((Ovl11SelectionObject *)obj);
                    return;
                case 0:
                    ovl_11_func_800CD6F4(obj);
                    D_80128CB0 = ovl_11_func_800C9424();
                    if (D_80128CB0 != 0) {
                        D_80128C64 = 0;
                        D_80128D30 = 0;
                    } else if (D_80128D30 == 0x10) {
                        D_80128D30 = 0;
                        D_80128CF8 = 1;
                        if ((obj->direction == 2) || (obj->direction == 0)) {
                            D_80128CF8 = 0;
                        }
                    } else {
                        D_80128D30 += 1;
                    }
                    flags = obj->flags;
                    if (flags & 8) {
                        if (obj->timer == 0) {
clear_flags:
                            obj->flags = flags & ~0xA;
                        } else {
                            obj->timer--;
                        }
                    }
                    break;
                }
            } else {
                ovl_11_func_800C6AFC((Ovl11C7010Object *)obj);
            }
            var_a1 = 0;
            if (obj->countdown != -2) {
                if (obj->countdown != -3) {
                    obj->countdown = (s16) ((u16) obj->countdown - 1);
                }
                if (obj->countdown == -1) {
                    var_a1 = 1;
                }
            } else if ((obj->flags36 & 0x10) || (obj->flags262 & 0x100)) {
                var_a1 = 1;
            }
            if (var_a1 != 0) {
                if (ovl_11_func_800C580C((Ovl11C580CObject *)obj) != 0) {
                    ovl_11_func_800C70CC((Ovl11C7010Object *) obj, 1U, 0, -2);
                    obj->flags = (s32) ((obj->flags & 0xDFFFFFFF) | 0x04000000);
                }
            }
            if (((Ovl11UpdateState *)D_8006C838)->tick & 1) {
                obj->historyA[9] = obj->historyA[8];
                obj->historyA[8] = obj->historyA[7];
                obj->historyA[7] = obj->historyA[6];
                obj->historyA[6] = obj->historyA[5];
                obj->historyA[5] = obj->historyA[4];
                obj->historyA[4] = obj->historyA[3];
                obj->historyA[3] = obj->historyA[2];
                obj->historyA[2] = obj->historyA[1];
                obj->historyA[1] = obj->historyA[0];
                obj->historyA[0] = obj->position;
            }
            if ((((Ovl11UpdateState *)D_8006C838)->tick & 7) == 7) {
                obj->historyB[9] = obj->historyB[8];
                obj->historyB[8] = obj->historyB[7];
                obj->historyB[7] = obj->historyB[6];
                obj->historyB[6] = obj->historyB[5];
                obj->historyB[5] = obj->historyB[4];
                obj->historyB[4] = obj->historyB[3];
                obj->historyB[3] = obj->historyB[2];
                obj->historyB[2] = obj->historyB[1];
                obj->historyB[1] = obj->historyB[0];
                obj->tags[9] = obj->tags[8];
                obj->tags[8] = obj->tags[7];
                obj->tags[7] = obj->tags[6];
                obj->tags[6] = obj->tags[5];
                obj->tags[5] = obj->tags[4];
                obj->tags[4] = obj->tags[3];
                obj->tags[3] = obj->tags[2];
                obj->tags[2] = obj->tags[1];
                obj->tags[1] = obj->tags[0];
                if (!(obj->flags & 0x100)) {
                    obj->historyB[0] = obj->position;
                    obj->tags[0].phase = (s16) ((u32) (((Ovl11UpdateState *)D_8006C838)->tick & 8) >> 3);
                    obj->tags[0].direction = (u16) obj->direction;
                    return;
                }
                obj->tags[0].direction = 5U;
            }
        }
    }
}
