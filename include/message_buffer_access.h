#ifndef MESSAGE_BUFFER_ACCESS_H
#define MESSAGE_BUFFER_ACCESS_H

#include "common.h"

/* Return the shared read buffer at primary state +0xD8 when the byte at
 * +0xCD is zero, otherwise NULL. This is an availability test, not a lock:
 * it changes neither the byte nor the buffer. The buffer is also used by
 * message-window and sprite-archive loading; callers must coordinate use. */
s32 *GetAvailableMessageReadBuffer(void);

/* Clear only the availability byte. Does not erase the buffer or wait for
 * an outstanding CD read/upload. */
void ClearMessageReadBufferBusy(void);

#endif
