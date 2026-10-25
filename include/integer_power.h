#ifndef INTEGER_POWER_H
#define INTEGER_POWER_H

#include "common.h"

/* Return base raised to exponent by repeated signed 32-bit multiplication.
 * Any exponent <= 0 returns 1, including 0^0. No overflow check is performed;
 * callers must keep intermediate products representable. No state changes. */
s32 pow_int(s32 base, s32 exponent);

#endif
