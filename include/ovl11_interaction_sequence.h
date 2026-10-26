#ifndef OVL11_INTERACTION_SEQUENCE_H
#define OVL11_INTERACTION_SEQUENCE_H

/* Interaction-volume dispatch entries. All reset shared interaction/menu
 * work, texture-stream cursor and cell cache; not reentrant with that menu.
 * Z/Y starts set game flag 0x71/0x70, phase 6/7 and pad-0 actuator preset 2.
 * The service waits for the corresponding scene rotation ramp's held phase
 * before requesting archive-3 message 0x378/0x379. */
void Ovl11StartSceneZRotationInteraction(void);
void Ovl11StartSceneYRotationInteraction(void);

/* Immediately request archive-3 message 0x37A, phase 4. Like the ramp-ready
 * path, mark resident control bit 0x20000, play SFX 3, disable portraits and
 * text input. Requires initialized message/texture/render state. */
void Ovl11StartMessageInteraction(void);

/* Per-frame service: ramp/message waits, sprite-grid drawing and texture
 * streaming, then the shared menu controller. Phase 0xFF is idle; phase 0xFE
 * waits for message status 2, resets it, marks resident exit flags and clears
 * game flags 0x70/0x71. The menu controller temporarily uses text palette 3. */
void Ovl11ServiceInteractionSequence(void);

#endif
