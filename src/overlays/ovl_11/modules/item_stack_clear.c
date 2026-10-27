/* Empty a six-byte stack slot; independent of global item-table lookup.
 * Original entry: ovl_11:800D5740. Evidence: notes/file-groupings.md. */
#include "ovl11_item_stack_clear.h"

void Ovl11ClearItemStackEntry(s16 *entry) {
    entry[0] = 0;
    entry[1] = 0;
    entry[2] = 0;
}
