#ifndef OVL11_DISPATCH_RECORD_FLAGS_H
#define OVL11_DISPATCH_RECORD_FLAGS_H

#include "common.h"
#include "game_types.h"

/* ovl_11:800F5210. Shared dispatch-pool slot 10 must contain at least thirteen
 * 0x18-byte records. Update their +4 flags bit 0 as follows:
 *   kind 0x64/0x21: derived progress mask 0x20/0x100;
 *   kind 0x128/0x129/0x12A: season 0/1/2 OR nonzero area variant.
 * Other kinds keep the bit. Then, for EVERY record with flags mask 0x8000,
 * derived mask 0x40 selects kind 0x6D, else mask 0x80 selects kind 0x6E
 * (both enable the record), else disable it. This second rule takes precedence.
 * IDs (+0) and other fields are unchanged. The selector parameter is unused:
 * season/variant/derived flags are read from shared work, not from it.
 * No allocation, pointer/count checks or saved-state restoration. */
void Ovl11UpdateList10RecordGates(s16 unusedSelector);

/* ovl_11:800F53BC. For count records, flags mask 0x20 sets bit 0; otherwise
 * mask 0x40 clears bit 0. Thus 0x20 wins if both are set. With neither, preserve it.
 * All other fields/bits are unchanged; count<=0 writes nothing. The caller
 * supplies the list and its bounds; this helper owns no pool or state. */
void Ovl11ApplyDispatchRecordFlagRules(s16 count, Ovl11DispatchRecord *records);

#endif
