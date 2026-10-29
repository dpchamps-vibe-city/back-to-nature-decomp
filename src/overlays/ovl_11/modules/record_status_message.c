/*
 * record_status_message — reconstructed translation unit.
 *
 * Publish record text and a two-position formatted count to shared text
 * substitution slots, then select and request a status message from three
 * record-field threshold bands.
 *
 * The evidence for its membership and boundaries is in
 * notes/file-groupings.md.
 */

#include "ovl11_record_status_message.h"
#include "message_window.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libapi.h"
#include "psyq/libgpu.h"
#include "text_engine.h"

/* Both clients ignore the message request's result; this service only
 * publishes the selected message and the two text substitution records. */
void Ovl11RequestRecordStatusMessage(UnkStruct800DF4F0 *record) {
    s16 bandValue;
    s32 messageCode;
    u16 stateAE;
    u16 countB2;

    SetTextSourcePointer(0, (s32) ((u8 *) record + 4));
    *(u16 *) WriteDecimalText(0x15 - (*(u16 *) ((u8 *) record + 0xB2)),
                         &gOvl11RecordStatusCountText, 2) = 0xFFFF;
    SetTextSourcePointer(1, (s32) &gOvl11RecordStatusCountText);
    stateAE = *(u16 *) ((u8 *) record + 0xAE);
    messageCode = 0x6C1;
    if (stateAE < 4U) {
        messageCode = 0x6C0;
        if (stateAE == 0) {
            countB2 = *(u16 *) ((u8 *) record + 0xB2);
            if (countB2 == 1) {
                messageCode = 0x6C3;
            } else {
                if (countB2 < 0x15U) {
                    messageCode = 0x6C2;
                    if (countB2 == 0) {
                        bandValue = *(s16 *) ((u8 *) record + 0x16);
                        messageCode = 0x6BE;
                        if (bandValue >= 9) {
                            messageCode = 0x6BD;
                            if (bandValue >= 0xC9) {
                                messageCode = 0x6BF;
                            }
                        }
                    }
                } else {
                    messageCode = 0x6C4;
                }
            }
        }
    }
    RequestMessageWindow(0, messageCode);
}
