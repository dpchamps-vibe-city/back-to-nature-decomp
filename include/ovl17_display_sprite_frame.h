#ifndef OVL17_DISPLAY_SPRITE_FRAME_H
#define OVL17_DISPLAY_SPRITE_FRAME_H

/* Select animation 4 on gOvl17DisplaySpriteSource, then draw its current
 * animation/frame bytes (+4/+5) at screen pixel (0,0), using the active
 * ordering-table word at base+4. The source and render context must already
 * be initialized. Selection resets frame/tick to zero and clears flags 300
 * on EVERY call, even if animation 4 was already selected.
 * Shares the display sprite with record_backdrop; does not own its storage. */
void Ovl17DrawDisplaySpriteFrameRects(void);

#endif
