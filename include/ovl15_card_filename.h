#ifndef OVL15_CARD_FILENAME_H
#define OVL15_CARD_FILENAME_H

#include "common.h"

/* Copy the original filename for slot 0..3, including its NUL terminator.
 * Any other signed value, including a negative one, selects entry 0.
 * Entries 0..2 end in FARM0/FARM1/FARM2; entry 3 is the distinct FARMP name.
 * destination needs enough writable storage (existing callers use 32 bytes).
 * No allocation, path prefix, card selection or capacity check is performed. */
void Ovl15BuildCardFilename(char *destination, s32 slot);

#endif
