#ifndef OVL11_DEFAULT_POSITION_H
#define OVL11_DEFAULT_POSITION_H

#include "common.h"

/* Write {selected X, 0, selected Z} as three signed words. X/Z are
 * sign-extended from the three-pair table; coordinate units are unchanged.
 * The shared selector at exe:80070D06 must be 0..2 and position must have
 * room for three words. No allocation, selector update or object commit is
 * performed. Script and reset callers decide where to apply the position. */
void Ovl11ReadDefaultPosition(s32 *position);

#endif
