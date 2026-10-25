/* Integer exponentiation, original exe:0x8001F250-0x8001F278.
 * Independent of the neighboring camera-offset and interpolation routines. */
#include "common.h"
#include "integer_power.h"

s32 pow_int(s32 base, s32 exponent) {
    s32 result;

    result = 1;
    if (exponent > 0) {
        do {
            exponent -= 1;
            result *= base;
        } while (exponent != 0);
    }
    return result;
}
