#include "common.h"
#include "ovl11_object_state_callbacks.h"

void Ovl11SetStateDebitAndDirection(u16 debit, u16 direction) {
    gOvl11StateValueDebit = debit;
    gOvl11StateEntryDirection = direction;
}
