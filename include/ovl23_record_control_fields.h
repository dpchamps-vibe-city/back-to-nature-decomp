#ifndef OVL23_RECORD_CONTROL_FIELDS_H
#define OVL23_RECORD_CONTROL_FIELDS_H

#include "common.h"
#include "game_types.h"

/* Store state/direction/controlValue into signed halfwords +4/+6/+8.
 * Inputs narrow to their low 16 bits; no validation, allocation or other
 * state change occurs. The record prefix before +4 is untouched.
 * Actor clients use the last halfword as a decision countdown; anchor-record
 * clients preserve/pass their own control value. Direction 2/6 moves toward
 * opposite X directions; other actor clients use the wider 0..7 convention.
 * Separate original entries are retained despite their identical bodies. */
void Ovl23SetActorControl(Ovl23Func800BB0C8Arg *record,
                          s32 state, s32 direction, s32 controlValue);
void Ovl23SetAnchorRecordControl(Ovl23Func800BB0C8Arg *record,
                                 s32 state, s32 direction, s32 controlValue);

#endif
