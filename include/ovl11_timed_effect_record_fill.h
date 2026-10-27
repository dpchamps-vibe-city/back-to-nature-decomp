#ifndef OVL11_TIMED_EFFECT_RECORD_FILL_H
#define OVL11_TIMED_EFFECT_RECORD_FILL_H
#include "common.h"
#include "game_types.h"

/* Copy XYZ words into the SHARED 0xF0-byte record at 80075AD4 (+38/+3C/+40),
 * then pair.second/first to that record's +30/+2E. Reset the timed effect at
 * effectRecord +A8. The reset pointer is independently supplied: callers
 * normally pass the shared record, but the stores are not relative to it.
 * No allocation, bounds, null or alias checks. Inputs must remain readable
 * across the sequential stores; native coordinate units are copied unchanged.
 * This mutates shared storage and is not reentrant. */
void Ovl11FillTimedEffectRecord(u8 *effectRecord, Ovl11EffectRecordPair *pair,
                              Vec3 *position);
#endif
