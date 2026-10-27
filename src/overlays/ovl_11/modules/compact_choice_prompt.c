/*
 * compact_choice_prompt — reconstructed translation unit.
 *
 * Stateful compact two-choice prompt controller. Sentinel x values choose the
 * initial selection; ordinary calls draw and process input, returning a
 * one-based accepted choice and resetting the shared selection.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"
#include "two_choice_prompt.h"

s32 Ovl11UpdateCompactChoicePrompt(s32 ot, s16 x, s16 y) {
    if (x == -1) {
        gOvl11CompactChoiceSelection = 0;
        return 0;
    }
    if (x == -2) {
        gOvl11CompactChoiceSelection = 1;
        return 0;
    }
    Ovl11DrawCompactChoicePrompt(ot, x, y, gOvl11CompactChoiceSelection);
    if (Ovl11ProcessTwoChoiceInput((u16 *)&gOvl11CompactChoiceSelection) == 1) {
        if (gOvl11CompactChoiceSelection == 0) {
            gOvl11CompactChoiceSelection = 0;
            return 1;
        }
        gOvl11CompactChoiceSelection = 0;
        return 2;
    }
    return 0;
}
