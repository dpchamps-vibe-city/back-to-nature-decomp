/* Item-interaction queue, ovl_11:800F4B5C–800F4CAC.
 * One resident-work halfword is the latch; it is also read by volume-action
 * and projected-record clients. The updater validates its season and decays
 * the signed timer, even on the tick that invalidates the queued item.
 * Membership and evidence-limited boundaries: notes/file-groupings.md. */
#include "common.h"
#include "ovl11_queued_item_interaction.h"

s32 Ovl11QueueItemInteraction(s16 itemId) {
    struct struct_8006C838_button *base =
        (struct struct_8006C838_button *)&D_8006C838;

    if (!(base->field_44F8 & 2)) {
        return 0;
    }
    if (base->field_44D6 != 0) {
        return 0;
    }
    if (itemId < 0x56) {
        if (itemId >= 0x51) {
            base->field_44D6 = (u16) itemId;
            return 1;
        }
    }
    return 0;
}

void Ovl11UpdateQueuedItemInteraction(void) {
    if (((struct struct_8006C838_button *)D_8006C838)->field_44D6 == 0) {
        return;
    }

    switch (((struct struct_8006C838_button *)D_8006C838)->field_44D6) {
    case 0x51:
    case 0x55:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 0) {
            Ovl11ClearQueuedItemInteraction();
        }
        break;
    case 0x52:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 1) {
            Ovl11ClearQueuedItemInteraction();
        }
        break;
    case 0x53:
    case 0x54:
        if (((struct struct_8006C838_button *)D_8006C838)->field_44BA != 2) {
            Ovl11ClearQueuedItemInteraction();
        }
        break;
    }

    ((struct struct_8006C838_button *)D_8006C838)->field_51DE -= 5;
    if (((struct struct_8006C838_button *)D_8006C838)->field_51DE < 0) {
        ((struct struct_8006C838_button *)D_8006C838)->field_51DE = 0;
    }
}

void Ovl11ClearQueuedItemInteraction(void) {
    gQueuedItemInteractionId = 0;
}
