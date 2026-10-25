#include "common.h"
#include "two_choice_prompt.h"

s32 ovl_11_func_80118B84(s32 arg0, s16 arg1, s16 arg2) {
    if (arg1 == -1) {
        gOvl11CompactChoiceSelection = 0;
        return 0;
    }
    if (arg1 == -2) {
        gOvl11CompactChoiceSelection = 1;
        return 0;
    }
    Ovl11DrawCompactChoicePrompt(arg0, arg1, arg2, gOvl11CompactChoiceSelection);
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
