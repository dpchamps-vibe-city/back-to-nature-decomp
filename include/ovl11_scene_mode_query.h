#ifndef OVL11_SCENE_MODE_QUERY_H
#define OVL11_SCENE_MODE_QUERY_H

#include "common.h"

/* Return 1 iff the shared far-work scene-mode word (+254A0) equals 3, else 0.
 * Read-only object-command predicate; does not advance or request a mode.
 * Other modules own the transitions. The gameplay meaning of mode 3 remains
 * unspecified rather than inferred from this equality test.
 */
s32 Ovl11IsSceneMode3(void);

#endif
