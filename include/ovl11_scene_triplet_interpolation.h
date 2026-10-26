#ifndef OVL11_SCENE_TRIPLET_INTERPOLATION_H
#define OVL11_SCENE_TRIPLET_INTERPOLATION_H

/* Publish the start triplet's low halfwords to scene record zero (+8/+A/+C)
 * and reset progress. Does NOT set the enabling game flag 8. */
void Ovl11ResetSceneTripletInterpolation(void);

/* With flag 8 set, interpolate three signed words across 1200 update steps,
 * publish their low halfwords, then advance progress. Progress 1200 publishes
 * the endpoint and clears flag 8; there are 1201 enabled publications from
 * progress zero. No-op while disabled. Shared state is not reentrant.
 * Require scene-record storage and Reset before a new transition. */
void Ovl11StepSceneTripletInterpolation(void);

#endif
