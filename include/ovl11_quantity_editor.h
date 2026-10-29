#ifndef OVL11_QUANTITY_EDITOR_H
#define OVL11_QUANTITY_EDITOR_H

#include "common.h"

/* Six-row quantity editor over the current work-area bank. Selector 3 uses
 * bank 2. Begin snapshots quantities and available value, resets selection,
 * and enters row selection. Update draws/services only while active; it also
 * handles input, guarded commit/cancel and the completion message.
 * The inactive predicate returns one only after the dialog has closed.
 * The sum predicate tests the stored six-field sum, not each field separately.
 * These entry points are ovl_11-local services; they take no arguments. */
s32 Ovl11IsSelectedQuantitySumZero(void);
s32 Ovl11IsQuantityEditorInactive(void);
void Ovl11BeginQuantityEditor(void);
void Ovl11UpdateQuantityEditor(void);

#endif
