/* Four-name memory-card lookup. Strings/table remain in the original data
 * image; this service only copies the selected NUL-terminated filename.
 * Original ovl_15:80137544–80137584; contract: ovl15_card_filename.h. */
#include "common.h"
#include "psyq/strings.h"
#include "ovl15_card_filename.h"

void Ovl15BuildCardFilename(char *destination, s32 slot) {
    if ((u32)slot >= 4U) {
        slot = 0;
    }
    strcpy(destination, (char *)gOvl15CardFilenameTable[slot]);
}
