#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "pad_actuator_sequences.h"
#include "message_window.h"

void *ovl_11_func_800BDCE8(u8 *arg0, Ovl11Func800BDCE8Arg1 *arg1, s32 arg2);
s32 ovl_11_func_80107C80(s16 arg0, u16 arg1, s32 *arg2, s32 arg3);
void ovl_11_func_800CD670(s16 arg0);
s32 ovl_11_func_800CB4E0(void);
s32 func_8001AF44(u32 arg0);
void func_8001AF70(u16 arg0, u16 arg1);

s32 ovl_11_func_800CD9A0(Ovl11C7010Object *arg0) {
    s32 messages[2];
    s16 lookupCode;
    s16 recordDirection;
    s16 code;
    s32 flags;
    s32 result;
    s32 mode;
    u16 direction;
    u16 recordFlags;
    Ovl11CD9A0Record *record;

    if (((struct Ovl11C7F0Flags *)D_8006C838)->flagsC & 0x08000000) {
        return 2;
    }
    if (!(arg0->unk6C & 0x40000008)) {
        if (arg0->unk6C & 0x20000000) {
            return 0;
        }
    }
    if (arg0->unk6C & 0x50) {
        return 0;
    }
    if (D_80128CB0 == 2) {
        return 0;
    }
    if (((struct Ovl11C7F0Flags *)D_8006C838)->flags10 & 0x20000000) {
        return 2;
    }
    record = ovl_11_func_800BDCE8(D_8009F78C, (Ovl11Func800BDCE8Arg1 *) &arg0->unk100, 1);
    D_801287F0 = record;
    if (record != NULL) {
        flags = arg0->unk6C;
        mode = 1;
        if (flags & 0x102) {
            mode = 0;
        }
        if (flags & 0x800000) {
            mode = 1;
        }
        if (flags & 0x100) {
            if (record->unkA == 3) {
                mode = 2;
            }
            if (record->unk2 == 0) {
                mode = 2;
            }
        }
        if (mode != 0) {
            recordDirection = D_801287F0->unk8;
            if ((recordDirection == 0x3E7) || (recordFlags = D_801287F0->unk0, ((recordFlags & 0x1200) != 0x1000))) {
                if (D_801287F0->unk0 & 0x200) {
                    lookupCode = D_801287F0->unkA;
                    if ((lookupCode != 0x63) && (ovl_11_func_80107C80(lookupCode, (u16) D_801287F0->unk8, &messages[0], 0) != 1)) {
                        if (messages[0] != -1) {
                            arg0->unk38 = (u16) D_801287F0->unk8;
                            RequestMessageWindow(2, messages[0]);
                            ovl_11_func_800CD670(0x14);
                            ovl_11_func_800CB4E0();
                        }
                        return 0;
                    }
                }
                if (func_8001AF44(0x15U) != 1) {
                    if (func_8001AF44(0x16U) != 1) {
                        ((struct Ovl11SelectionResetMode *)&D_8007AFF0)->mode = 3;
                        arg0->unk6C |= 0x400;
                        ovl_11_func_800CB4E0();
                        return 1;
                    }
                    return 2;
                }
                return 2;
            }
            if (D_80128C50 & 0x2000) {
                if (recordDirection == arg0->unk38) {
                    if (!(D_801231F8 & 0x40)) {
                        return 0;
                    }
                    goto check_direction;
                }
                return 2;
            }
            direction = arg0->unk38;
            if (D_801287F0->unkA != 0x2F) {
                if (mode != 2) {
                    if (!(recordFlags & 0x200) && !(D_801231F8 & 0x40)) {
                        return (recordDirection != direction) * 2;
                    }
                    goto check_direction;
                }
                if (recordDirection != direction) {
                    return 2;
                }
                goto activate;
            }
        check_direction:
            if (D_801287F0->unk8 == arg0->unk38) {
            activate:
                messages[1] = 0;
                code = D_801287F0->unkA;
                if (code == 0x63) {
                    return 1;
                }
                if (code == 0x2E) {
                    if (arg0->unk6C & 0x18008000) {
                        return 0;
                    }
                }
                result = ovl_11_func_80107C80(D_801287F0->unkA, (u16)D_801287F0->unk8, &messages[1], 0);
                if (result == 1) {
                    RequestPadActuatorSequence(0, 2);
                    ((struct Ovl11SelectionResetMode *)&D_8007AFF0)->mode = result;
                    ovl_11_func_800CD670(0xA);
                    ovl_11_func_800CB4E0();
                    if (D_801287F0->unk2 != 0xFFFE) {
                        func_8001AF70(3U, 1U);
                        return 1;
                    }
                    return 1;
                }
                if (messages[1] != -1) {
                    RequestMessageWindow(2, messages[1]);
                }
                ovl_11_func_800CD670(5);
                ovl_11_func_800CB4E0();
                return 0;
            }
            return 2;
        }
        if (D_801287F0->unk8 == arg0->unk38) {
            ovl_11_func_800CD670(0xA);
            return 2;
        }
        return 2;
    }
    return 2;
}
