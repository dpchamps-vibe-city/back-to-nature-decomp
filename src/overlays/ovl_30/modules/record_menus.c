/*
 * Descriptor-driven debug value/flag menus, ovl_30:80133454..80133F70.
 * Both display nine rows, share a caller-managed cursor and publish the
 * selected descriptor id. Value editing wraps across descriptor bounds;
 * flag editing sets/clears a mask. See ovl30_record_menus.h for input and
 * descriptor constraints. The helper remains externally linked as before.
 * Membership and boundaries are documented in notes/file-groupings.md.
 */
#include "common.h"
#include "ovl30_record_menus.h"
#include "game_types.h"
#include "psyq/stddef.h"
#include "psyq/libgte.h"
#include "psyq/libgpu.h"
#include "sound_commands.h"

void Ovl30DrawAndEditValueMenu(Ovl30ValueMenuEntry *entries, s32 count) {
    char *labels[2];
    s32 i;
    s32 label;
    s32 buttons;
    Ovl30ValueMenuEntry *entry;
    Ovl30ValueMenuEntry *selected;

    labels[0] = gOvl30RowKindMarkers[0];
    labels[1] = gOvl30RowKindMarkers[1];
    for (i = gOvl30RecordMenuCursor / 9 * 9; i < gOvl30RecordMenuCursor / 9 * 9 + 9; i++) {
        entry = &entries[i];
        if (i < count) {
            label = entry->id == -2;
            switch (entry->width) {
            case 0:
            default:
                KanjiFntPrint(gOvl30HeaderRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text);
                break;
            case 1:
                if (entry->lower == 0) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *entry->value);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *(s8 *)entry->value);
                }
                break;
            case 2:
                if (entry->lower == 0) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *(u16 *)entry->value);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *(s16 *)entry->value);
                }
                break;
            case 4:
                if (entry->lower == 0) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *(u32 *)entry->value);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, *(s32 *)entry->value);
                }
                break;
            }
        } else {
            KanjiFntPrint(gOvl30MenuBlankLine);
        }
    }
    buttons = gPad0Input->repeatButtons;
    if (buttons & 0x1000) {
        gOvl30RecordMenuCursor--;
        if (gOvl30RecordMenuCursor < 0) {
            gOvl30RecordMenuCursor = count - 1;
        }
    } else if (buttons & 0x4000) {
        gOvl30RecordMenuCursor++;
        if (gOvl30RecordMenuCursor > count - 1) {
            gOvl30RecordMenuCursor = 0;
        }
    } else if (buttons & 0x8000) {
        gOvl30RecordMenuCursor -= 9;
        if (gOvl30RecordMenuCursor < 0) {
            gOvl30RecordMenuCursor = 0;
        }
    } else if (buttons & 0x2000) {
        gOvl30RecordMenuCursor += 9;
        if (gOvl30RecordMenuCursor > count - 1) {
            gOvl30RecordMenuCursor = count - 1;
        }
    } else {
        goto edit_entry;
    }
    PlaySoundEffect(5);
    return;

edit_entry:
    if (gPad0Input->pressedButtons & 0x100) {
        gOvl30RecordMenuSelectedId = entries[gOvl30RecordMenuCursor].id;
        return;
    }
    selected = (Ovl30ValueMenuEntry *)(gOvl30RecordMenuCursor * sizeof(*entries) + (u32)entries);
    if (selected->id != -2) {
        if (buttons & 0x20) {
            Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, 1);
        } else if (buttons & 0x40) {
            Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, -1);
        } else if (buttons & 0x10) {
            if (gPad0Input->heldButtons & 0x100) {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, 100);
            } else if (gPad1Input->heldButtons & 8) {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, 1000);
            } else {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, 10);
            }
        } else if (buttons & 0x80) {
            if (gPad0Input->heldButtons & 0x100) {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, -100);
            } else if (gPad1Input->heldButtons & 8) {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, -1000);
            } else {
                Ovl30StepFieldValue(selected->value, selected->width, selected->upper, selected->lower, -10);
            }
        }
    }
}

/* Crossing a bound selects the opposite bound, not a clamp or modulo.
 * Preserve the shared comparison path: the upper-negative arm computes the
 * same lower test before joining it. This is the original control shape. */
void Ovl30StepFieldValue(u8 *value, s32 width, s32 upper, s32 lower, s32 delta) {
    s32 next;
    s32 belowLower;
    s32 current;

    if (width != 0) {
        current = 0;
        switch (width) {
        case 1:
            if (lower == 0) {
                current = *value;
            } else {
                current = *(s8 *)value;
            }
            break;
        case 2:
            if (lower == 0) {
                current = *(u16 *)value;
            } else {
                current = *(s16 *)value;
            }
            break;
        case 4:
            current = *(s32 *)value;
            break;
        }
        next = current + delta;
        if (upper < 0) {
            if (upper < next) {
                goto wrap_to_lower;
            }
            belowLower = next < lower;
            goto test_lower;
        }
        if (upper < next) {
wrap_to_lower:
            next = lower;
        } else {
test_lower:
            belowLower = next < lower;
            if (belowLower != 0) {
                next = upper;
            }
        }
        switch (width) {
        case 1:
            *value = (u8)next;
            return;
        case 2:
            *(u16 *)value = next;
            return;
        case 4:
            *(s32 *)value = next;
            return;
        }
    }
}

void Ovl30DrawAndEditFlagMenu(Ovl30FlagMenuEntry *entries, s32 count) {
    char *labels[2];
    s32 i;
    s32 label;
    s32 buttons;
    Ovl30FlagMenuEntry *entry;
    Ovl30FlagMenuEntry *selected;

    labels[0] = gOvl30RowKindMarkers[0];
    labels[1] = gOvl30RowKindMarkers[1];
    for (i = gOvl30RecordMenuCursor / 9 * 9; i < gOvl30RecordMenuCursor / 9 * 9 + 9; i++) {
        entry = &entries[i];
        if (i < count) {
            label = entry->id == -2;
            switch (entry->width) {
            case 0:
            default:
                KanjiFntPrint(gOvl30HeaderRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text);
                break;
            case 1:
                if (*(u8 *)entry->value & entry->mask) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 0);
                }
                break;
            case 2:
                if (*(s16 *)entry->value & entry->mask) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 0);
                }
                break;
            case 4:
                if (*(s32 *)entry->value & entry->mask) {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 1);
                } else {
                    KanjiFntPrint(gOvl30ValueRowFormat, i == gOvl30RecordMenuCursor ? gOvl30CursorRowMarker : gOvl30IdleRowMarker, labels[label], entry->text, 0);
                }
                break;
            }
        } else {
            KanjiFntPrint(gOvl30MenuBlankLine);
        }
    }
    buttons = gPad0Input->repeatButtons;
    if (buttons & 0x1000) {
        gOvl30RecordMenuCursor--;
        if (gOvl30RecordMenuCursor < 0) {
            gOvl30RecordMenuCursor = count - 1;
        }
    } else if (buttons & 0x4000) {
        gOvl30RecordMenuCursor++;
        if (gOvl30RecordMenuCursor > count - 1) {
            gOvl30RecordMenuCursor = 0;
        }
    } else if (buttons & 0x8000) {
        gOvl30RecordMenuCursor -= 9;
        if (gOvl30RecordMenuCursor < 0) {
            gOvl30RecordMenuCursor = 0;
        }
    } else if (buttons & 0x2000) {
        gOvl30RecordMenuCursor += 9;
        if (gOvl30RecordMenuCursor > count - 1) {
            gOvl30RecordMenuCursor = count - 1;
        }
    } else {
        goto edit_entry;
    }
    PlaySoundEffect(5);
    return;

edit_entry:
    {
        if (gPad0Input->pressedButtons & 0x100) {
            gOvl30RecordMenuSelectedId = entries[gOvl30RecordMenuCursor].id;
            return;
        }
        selected = (Ovl30FlagMenuEntry *)(gOvl30RecordMenuCursor * sizeof(*entries) + (u32)entries);
        if (selected->id != -2) {
            if (buttons & 0x20) {
                switch (selected->width) {
                case 0:
                    break;
                case 1:
                    *(u8 *)selected->value |= (u8)selected->mask;
                    break;
                case 2:
                    *(u16 *)selected->value |= (u16)selected->mask;
                    break;
                case 4:
                    *(s32 *)selected->value |= selected->mask;
                    break;
                }
            } else if (buttons & 0x40) {
                switch (selected->width) {
                case 0:
                    break;
                case 1:
                    *(u8 *)selected->value &= ~(u8)selected->mask;
                    break;
                case 2:
                    *(u16 *)selected->value &= ~(u16)selected->mask;
                    break;
                case 4:
                    *(s32 *)selected->value &= ~selected->mask;
                    break;
                }
            }
        }
        return;
    }
}
