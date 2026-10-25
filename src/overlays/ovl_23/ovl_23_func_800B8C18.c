#include "common.h"
#include "game_types.h"
#include "psyq/stdlib.h"
#include "ovl23_record_random_selection.h"

void ovl_23_func_800BB0D8(Ovl23Func800BB0C8Arg *arg0, s32 arg1, s32 arg2, s32 arg3);
void ovl_23_func_800BA610(Ovl23Func800BB0C8Arg *arg0);
void ovl_23_func_800B9A00(Ovl23Func800BB0C8Arg *arg0);

void ovl_23_func_800B8C18(void) {
    s32 i;
    s32 excluded;
    s32 offset;
    s16 temp_a0;
    s32 test;
    struct Ovl23DecisionRecord *var_s1;
    struct Ovl23DecisionRecord *recordsBase;
    struct Ovl23DecisionView *base2;
    s16 diff;
    s32 distance;
    struct Ovl23DecisionView *base;
    struct Ovl23DecisionWindow *record;

    for (i = 0; i < 6; i++) {
        if (i != 2) {
            base = (struct Ovl23DecisionView *)Ovl23_D_800BF87C_draw;
            record = (struct Ovl23DecisionWindow *)((u8 *)base + i * 0x44);
            if (record->unk206 == 1) {
                if (record->unk20C > 0) {
                    record->unk20C--;
                } else {
                    record->unk20C = record->unk20E;
                    diff = base->actors[i].unk38 / 4096 - base->records[i].unkC / 4096;
                    distance = abs(diff);
                    if (distance < base->actors[i].unk2E) {
                        switch (Ovl23ChooseRecordWeightedResult(i, 0)) {
                        case 1:
                            if (diff > 0) {
                                ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 2, record->unk20C);
                            } else {
                                ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 6, record->unk20C);
                            }
                            break;
                        case 2:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(0, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(1, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                }
                            }
                            break;
                        case 3:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(0, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(1, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                }
                            }
                            break;
                        }
                    } else {
                        switch (Ovl23ChooseRecordWeightedResult(i, 1)) {
                        case 1:
                            if (diff > 0) {
                                ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 2, record->unk20C);
                            } else {
                                ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 1, 6, record->unk20C);
                            }
                            break;
                        case 2:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(2, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(3, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 6, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 2, 2, record->unk20C);
                                }
                            }
                            break;
                        case 3:
                            if (diff > 0) {
                                if (Ovl23TestRecordRandomThreshold(2, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                }
                            } else {
                                if (Ovl23TestRecordRandomThreshold(3, i) == 1) {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 6, record->unk20C);
                                } else {
                                    ovl_23_func_800BB0D8((Ovl23Func800BB0C8Arg *)&D_800BFA80[i], 3, 2, record->unk20C);
                                }
                            }
                            break;
                        }
                    }
                }
            }
        }
    }
    i = 0;
    excluded = 2;
    var_s1 = D_800BFA80;
    do {
        recordsBase = D_800BFA80;
        if (i != excluded) {
            offset = i * 0x44;
            base2 = (struct Ovl23DecisionView *)((u8 *)recordsBase - 0x204);
            temp_a0 = ((struct Ovl23DecisionWindow *)((u8 *)base2 + offset))->unk206;
            offset = 1;
            test = temp_a0;
            if (test == offset) {
                ovl_23_func_800BA610((Ovl23Func800BB0C8Arg *)var_s1);
            } else {
                test = 3;
                if (temp_a0 == test) {
                    ovl_23_func_800B9A00((Ovl23Func800BB0C8Arg *)&recordsBase[i]);
                }
            }
        }
        var_s1++;
        i += 1;
    } while (i < 6);
}
