#ifndef OVL11_GAME_FLAG_COMMAND_H
#define OVL11_GAME_FLAG_COMMAND_H

#include "common.h"

/* ovl_11 adapter for the resident game flag bank. testOnly == 0 sets/clears
 * flagId's LOW 16 bits according to enabled != 0 and returns 1. Nonzero
 * testOnly reads that flag and returns 0/1, ignoring enabled. No slot lookup,
 * bounds check, persistence or notification is added; initialize state memory
 * first. Original identity: ovl_11:800E9384. */
s32 Ovl11GameFlagCommand(s16 testOnly, s16 enabled, s32 flagId);

#endif
