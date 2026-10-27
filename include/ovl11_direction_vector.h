#ifndef OVL11_DIRECTION_VECTOR_H
#define OVL11_DIRECTION_VECTOR_H

#include "common.h"
#include "game_types.h"

/* Add three signed words, in X/Y/Z order. Units belong to the caller.
 * Zero retainIncrement clears the increment AFTER all three additions.
 * If both pointers alias, the doubled vector is consequently cleared. */
void Ovl11AccumulateDirectionVector(Recon800D0408A1View *position,
                                   Recon800D0408A1View *increment,
                                   s32 retainIncrement);

/* Clear the vector, then select one signed component of magnitude:
 * 0:-Z, 1:-X, 2:+Z, 3:+X, -1:-Y, -2:+Y; 4/other: all zero.
 * Only the signed low 16 bits of direction select the component. The word
 * interface accepts both signed and unsigned halfword loads without changing
 * their promotion at callers. There is no return-value contract. */
void Ovl11SetDirectionVector(s32 direction, Recon800D0408A1View *vector,
                            s32 magnitude);

#endif
