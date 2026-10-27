#ifndef OVL31_MEMCARD_UTILITY_H
#define OVL31_MEMCARD_UTILITY_H

/* Scene 18 callback, valid only while overlay 31 is loaded. Reads pad-0
 * pressed-button words, emits debug-font text and runs synchronous card/PDA
 * operations on slot zero. A nonzero result waits for acknowledgement;
 * Start requests scene zero. Returns no value and does not pace frames. */
void Ovl31UpdateMemoryCardUtility(void);

#endif
