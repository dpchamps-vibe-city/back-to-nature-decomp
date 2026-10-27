#ifndef OVL25_SCENE_MODE_HANDLERS_H
#define OVL25_SCENE_MODE_HANDLERS_H

#include "common.h"

/* Three callbacks selected by the outer lifecycle's mode table. Shared work,
 * initial-record tables, message/sound/render services and record divisors must
 * already be initialized. All operate on shared state and are non-reentrant.
 * Counters count update calls, not elapsed milliseconds.
 */

/* Mode 0: draw, dispatch the current event phase, and advance its modulo-10000
 * signed-halfword timer. Event phase 6 waits for the fade and enters mode 1.
 */
void Ovl25DispatchEventPhase(void);

/* Mode 1: start message 35 and a record timer, then after 60 calls reinitialize
 * record state, start an effect voice, seed the target/sequence anchor and
 * enter mode 2. This is a counter wait, not a message-completion wait.
 */
void Ovl25RunDialogueMode(void);

/* Mode 2: pad bit 800 toggles pause and sprite sets. Paused calls draw without
 * running the input, target, record-choice, timer/update or completion chains.
 * Active calls run those in their original order and advance the modulo timer.
 * Each record's signed word at +0C is clamped to 0..32000 (hex); its units and
 * broader gameplay meaning remain neutral. Record 0 alone owns effect-voice
 * transitions and volume changes. Completion can advance the outer phase.
 */
void Ovl25UpdateActiveSceneMode(void);

/* Render six scene and two auxiliary records plus indicators/paired/sequence
 * sprites. The final sequence service advances state as well as drawing, so
 * this is not a read-only or idempotent rendering operation.
 */
void Ovl25DrawSceneSprites(void);

#endif
