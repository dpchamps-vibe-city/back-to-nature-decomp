#ifndef OVL11_TIME_OF_DAY_FADE_H
#define OVL11_TIME_OF_DAY_FADE_H

/* Scene-kind-15 presentation except scene id E5. Require season 0..3 and
 * initialized clock/GPU/packet work. Seasonal sunset hours are {18,19,18,17}.
 * Init captures VRAM rows into the shared 16-color row buffer; destination
 * is the 16x1 row at (320,220). Overwrites shared blend state, not reentrant. */
void Ovl11InitTimeOfDayFade(void);

/* Dawn/day draw progress zero; dusk draws the old progress then samples
 * in-game minutes since sunset, capped at 113. At 113 reinitialize the
 * night rows (222/221) once and mark progress 114. Night advances a bouncing
 * 32-step blend once per call. No wall-clock duration or palette identity
 * beyond these observed rows is assumed. Call Init before Update. */
void Ovl11UpdateTimeOfDayFade(void);

#endif
