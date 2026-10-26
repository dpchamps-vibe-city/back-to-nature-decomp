#include "common.h"
#include "ovl11_save_counters.h"
#include "ovl11_balance_accounting.h"

typedef struct {
    /* 0x00 */ char pad0[0x8];
    /* 0x08 */ s16 unk8;
    /* 0x0A */ s16 unkA;
} UnkStruct801152BC;

void ovl_11_func_801152BC(UnkStruct801152BC *arg0) {
    Ovl11AdjustSpendableBalance(-(arg0->unkA * arg0->unk8), 1);
    Ovl11AddSaveCounter18(2, arg0->unk8);
}
