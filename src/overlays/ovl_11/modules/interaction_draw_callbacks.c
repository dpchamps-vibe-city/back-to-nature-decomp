/*
 * interaction_draw_callbacks — reconstructed translation unit.
 *
 * All seven non-null interaction draw callbacks, in draw-mode order. Compose
 * existing value, selection and quantity panels using the active render
 * context and caller-managed shared records. The eight-word draw table at
 * 800BB228 has a null slot 0 followed by these seven callbacks. They take no
 * arguments, return 0, and do not advance the interaction or draw mode.
 * Panel coordinates are pixels; appended panels follow the preceding-entry
 * count at fourteen pixels per row. Shared text scratch/packets require an
 * initialized render context and make these callbacks non-reentrant.
 * The byte-identical mode-2/mode-3 entries remain distinct table identities;
 * no public header is needed because the callback table is their interface.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "ovl11_selection_list_panel.h"
#include "ovl11_value_panels.h"
#include "ovl11_selection_record_access.h"

s32 Ovl11DrawValuePanelWithQuantity(void) {
    Ovl11DrawCompactValuePanel();
    Ovl11DrawQuantityPromptRows(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8, gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, 0x10);
    return 0;
}

s32 Ovl11DrawCompactValuePanelOnly(void) {
    Ovl11DrawCompactValuePanel();
    return 0;
}

s32 Ovl11DrawCompactValuePanelOnlyVariant(void) {
    Ovl11DrawCompactValuePanel();
    return 0;
}

s32 Ovl11DrawCompactValuePanelWithSelection(void) {
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x10);
    return 0;
}

s32 Ovl11DrawCompactValuePanelWithSelectionAndQuantity(void) {
    s16 y;

    y = (Ovl11CountPrecedingSelectionEntries(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x10);
    Ovl11DrawQuantityPromptRows(gOvl11QuantityPrompt, gOvl11QuantityPrompt.unk8,
                       gOvl11QuantityPrompt.unk8 * gOvl11QuantityPrompt.unkA, 0x10, y);
    return 0;
}

s32 Ovl11DrawCompactValuePanelWithSecondarySelection(void) {
    s16 y;

    y = (Ovl11CountPrecedingSelectionEntries(*(StructD548 *)gOvl11PrimarySelectionRecord) + 1) * 14 + 20;
    Ovl11DrawCompactValuePanel();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11SecondarySelectionRecord, 0x10, y);
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x10);
    return 0;
}

s32 Ovl11DrawWideValuePanelWithSelection(void) {
    Ovl11DrawWideValuePanel();
    Ovl11DrawSelectionListPanel((s32 *)gOvl11PrimarySelectionRecord, 0x10, 0x30);
    return 0;
}
