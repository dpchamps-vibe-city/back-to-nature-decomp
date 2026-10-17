#include "common.h"

void ovl_23_func_800BB0E8(void);
s32 func_8001FABC(s16 arg0);
void ovl_23_func_800BB150(void);
void ovl_23_func_800B92E8(void);
void ovl_23_func_800B8B94(void);
void ovl_23_func_800B8C18(void);
void ovl_23_func_800B8FA4(void);
void func_80013328(s16 arg0);
void func_8001FE34(s32 arg0);
s32 func_80013394(void);
s32 func_8001FE6C(void);
void func_8001FAE8(s32 arg0);
s32 func_8001FBBC(s16 arg0);
s32 func_80020B80(s32 arg0, s32 arg1);
void func_80011EF0(s32 arg0);
void ovl_23_func_800BB7A0(s16 arg0);
void ovl_23_func_800BB800(s16 arg0);
void ovl_23_func_800B9310(void);

void ovl_23_func_800B87F8(void) {
    struct Ovl23ScreenHeader *state = &Ovl23_D_800BF87C_header;
    struct Ovl23ActorDecisionView *race;
    u8 *systemData;
    u8 *save;
    s16 readyCount;
    s32 paused;
    s32 result;
    s32 i;
    s32 offset;
    s16 *actorState;
    s16 *recordState;

    if (state->unkC == 0) {
        if (*(s32 *)((u8 *)D_8005E3A8 + 8) & 0x800) {
            paused = state->unk4 != 1;
            state->unk4 = paused;
            if (paused == 1) {
                ovl_23_func_800BB0E8();
                state->unk414 = func_8001FABC(0x36);
            } else {
                ovl_23_func_800BB150();
            }
        }
    }
    state = &Ovl23_D_800BF87C_header;
    readyCount = 0;
    if (state->unk4 == 1) {
        ovl_23_func_800B92E8();
        return;
    }
    state->unk14 = (u16)(state->unk14 + 1);
    i = 0;
    race = (struct Ovl23ActorDecisionView *)state;
    recordState = &race->records[0].unk2;
    actorState = &race->actors[0].unk2;
    offset = 0;
    ovl_23_func_800B8B94();
    ovl_23_func_800B8C18();
    ovl_23_func_800B8FA4();
    systemData = (u8 *)D_8006C838;
    save = D_80074838;
    for (; i < 6; i++) {
        if (((struct Ovl23DecisionView *)((u8 *)race + offset))->actors[0].unk38 >=
            (race->unk39C << 12)) {
            *recordState = *actorState = 3;
            result = ((struct Ovl23ScreenHeader *)race)->unk8;
            if (result == 0) {
                ((struct Ovl23ScreenHeader *)race)->unk8 = 1;
                ((struct Ovl23ScreenHeader *)race)->unk414 = func_8001FABC(0x43);
                if (i == 2) {
                    *(s32 *)(save + 0x6630) = 1;
                } else {
                    u16 choice = Ovl23_D_800BF87C_header.unk16[i];

                    *(s32 *)(save + 0x6630) = 0;
                    *(u16 *)(save + 0x6634) = choice;
                }
            }
            if (i == 2) {
                state = &Ovl23_D_800BF87C_header;
                if (((struct Ovl23ActorDecisionView *)state)->actors[2].unk14 ==
                    ((((struct Ovl23ActorDecisionView *)state)->unk39C + 0x37) << 12)) {
                    if (state->unkC == 0) {
                        state->unkC = 1;
                        func_80013328(10);
                        func_8001FE34(10);
                    } else {
                        result = func_80013394();
                        if (result == 1) {
                            readyCount += 1;
                        }
                        result = func_8001FE6C();
                        if (result == 0) {
                            readyCount += 1;
                        }
                        if (readyCount == 2) {
                            if (state->unk414 != -1) {
                                func_8001FAE8((s32)state->unk414);
                            }
                            func_8001FBBC(0);
                            func_80020B80(2, 0);
                            func_80020B80(1, 0);
                            *(s32 *)(systemData + 0x448C) = 0xFF;
                            func_80011EF0(6);
                            *(s32 *)(systemData + 0xC) |= 0x80000;
                            if (*(s32 *)(save + 0x6630) == 1) {
                                ovl_23_func_800BB7A0(1);
                                ovl_23_func_800BB7A0(2);
                                ovl_23_func_800BB7A0(4);
                                ovl_23_func_800BB7A0(6);
                                ovl_23_func_800BB7A0(0xF);
                                ovl_23_func_800BB7A0(0x10);
                                ovl_23_func_800BB7A0(0x11);
                                ovl_23_func_800BB7A0(0x13);
                                ovl_23_func_800BB7A0(0x15);
                                ovl_23_func_800BB7A0(0x16);
                                ovl_23_func_800BB7A0(0x1A);
                                ovl_23_func_800BB7A0(0x1C);
                                ovl_23_func_800BB800(3);
                                ovl_23_func_800BB800(7);
                                ovl_23_func_800BB800(0x14);
                            }
                        }
                    }
                }
            }
        }
        actorState += 0x28;
        offset += 0x50;
        recordState += 0x22;
    }
    {
        struct Ovl23ScreenHeader *camera = &Ovl23_D_800BF87C_header;

        if (((struct Ovl23ActorDecisionView *)camera)->records[2].unkC > 0x3FFFF) {
            camera->unk10 = ((struct Ovl23ActorDecisionView *)camera)->records[2].unkC;
        }
        if (camera->unk10 <= 0x40000) {
            camera->unk10 = 0x40000;
        } else {
            result = ((struct Ovl23ActorDecisionView *)camera)->unk39C << 12;
            if (camera->unk10 >= result) {
                camera->unk10 = result;
            }
        }
        ovl_23_func_800B9310();
    }
}
