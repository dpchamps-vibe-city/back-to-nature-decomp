/*
 * state_entry_parameters — reconstructed translation unit.
 *
 * Parameter latch for object-state entry debit and frame-state-06 direction.
 * Stores raw halfword bits without entering a state or invoking a callback.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "ovl11_object_state_callbacks.h"

void Ovl11SetStateDebitAndDirection(u16 debit, u16 direction) {
    gOvl11StateValueDebit = debit;
    gOvl11StateEntryDirection = direction;
}
