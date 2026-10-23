#include "common.h"

/* ovl_19_func_800BB08C is a GCC nested function of ovl_19_func_800BB108 and
 * is defined inside that function in ovl_19_func_800BB108.c. That translation
 * unit emits the helper immediately before its parent, as the original did,
 * so this file deliberately defines nothing: a second definition would emit
 * the helper twice and shift every later function in the overlay.
 *
 * The nested definition also produces the entry `sw $v0,0x18($sp)` naturally
 * -- it is GCC storing the incoming static chain into the nested function's
 * frame -- so the register-capture construct the standalone match needed is
 * gone.
 */
