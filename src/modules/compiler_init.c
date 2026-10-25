/*
 * compiler_init — reconstructed translation unit.
 *
 * Empty GCC initialization hook. The compiler inserts a call to __main in
 * main; this build has no constructor work.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "common.h"

/* GCC calls this before main's body. There are no constructors to run. */
void __main(void) {
}
