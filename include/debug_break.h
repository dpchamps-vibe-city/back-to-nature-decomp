#ifndef DEBUG_BREAK_H
#define DEBUG_BREAK_H

#include "common.h"

/* Execute breakpoint 1, then return 1 if a debugger resumes execution.
 * Callers deliberately repeat this call while an allocation/range invariant
 * is violated; it neither logs nor repairs the failed condition. */
s32 DebugBreakAndReturnTrue(void);

#endif
