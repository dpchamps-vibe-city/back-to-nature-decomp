#ifndef OVL19_PROJECTED_ACTOR_RENDER_H
#define OVL19_PROJECTED_ACTOR_RENDER_H

#include "common.h"

/* Resident ovl_19 with initialized actor records, sprite banks, projection
 * camera and active render context required. Draw two 0x48-byte paired actor
 * records, three 0x40-byte auxiliary records, then nine fixed-position sprites.
 * Actor +2 state selects animation, +4 selects the variant and +6 is compared
 * with the scene's tick thresholds. States 7/8/10 can copy the other actor's
 * variant; state 2 chooses a fixed frame. Some states also draw animation 0x24
 * from sceneWork+1D0, with projected Y offset -24. Other actor Y offsets are
 * 0 or -10. All other geometry is delegated to ovl19_projected_sprite.h.
 * Mutates animation/frame state and, for those states, actor variant halfwords;
 * appends frame packets, but does not step the scene's behavior/tick counters.
 * Raw state codes and independent signed/unsigned reads are preserved. */
void Ovl19DrawProjectedActorSprites(void);

#endif
