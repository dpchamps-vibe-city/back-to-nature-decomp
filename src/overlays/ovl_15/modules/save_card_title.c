/*
 * Save-card title text, original ovl_15:80134770-80134A34.
 * One title builder and its private Shift-JIS digit/season helpers share
 * scratch buffers. The surrounding record selection and save-image packing
 * code are clients, not part of this formatting protocol.
 */
#include "common.h"
#include "game_types.h"
#include "ovl15_save_card_title.h"
#include "psyq/strings.h"

s8 *Ovl15FormatTitleDigits(s16 value, s8 *destination, s16 digits);
void Ovl15FormatTitleSeason(s16 season, char *destination);

s8 *Ovl15BuildSaveCardTitle(s16 slot) {
    Ovl15SaveTitleDate date;
    struct Ovl15SaveHeader *entries;
    Ovl15SaveTitleDate *source;

    memset(Ovl15CardHeader.title, 0, sizeof(Ovl15CardHeader.title));
    entries = D_801376E0_headers;
    source = (Ovl15SaveTitleDate *)&entries[slot].words;
    date.season = source->season;
    date.field6 = source->field6;
    date.field4 = source->field4 + 1;
    date.year = source->year + 1;
    Ovl15CardHeader.title[0] = 0;
    strcat(Ovl15CardHeader.title, gOvl15CardTitlePrefix);
    Ovl15FormatTitleDigits(slot + 1, (s8 *)gOvl15CardTitlePart, 1);
    strcat(Ovl15CardHeader.title, gOvl15CardTitlePart);
    strcat(Ovl15CardHeader.title, gOvl15CardTitleYearLabel);
    Ovl15FormatTitleDigits(date.year, (s8 *)gOvl15CardTitlePart, 2);
    strcat(Ovl15CardHeader.title, gOvl15CardTitlePart);
    strcat(Ovl15CardHeader.title, gOvl15CardTitleSpace);
    Ovl15FormatTitleSeason(date.season, gOvl15CardTitlePart);
    return (s8 *)strcat(Ovl15CardHeader.title, gOvl15CardTitlePart);
}

/* Shift-JIS decimal glyphs are two bytes. A missing tens digit is a fullwidth
 * space; adding the decimal value to the zero glyph's second byte selects
 * the digit. The original 16-bit-shift test is retained for signed inputs. */
s8 *Ovl15FormatTitleDigits(s16 value, s8 *destination, s16 digits) {
    s16 quotient;

    *destination = 0;
    gOvl15CardTitleGlyphScratch[0] = 0;
    if (digits != 1) {
        if (((value / 10) << 16) == 0) {
            strcat((char *)gOvl15CardTitleGlyphScratch, (char *)gOvl15CardTitleSpace);
        } else {
            strcat((char *)gOvl15CardTitleGlyphScratch, (char *)gOvl15CardTitleZeroGlyph);
            gOvl15CardTitleGlyphScratch[1] = gOvl15CardTitleGlyphScratch[1] + value / 10;
        }
        strcat((char *)destination, (char *)gOvl15CardTitleGlyphScratch);
    }
    gOvl15CardTitleGlyphScratch[0] = 0;
    strcat((char *)gOvl15CardTitleGlyphScratch, (char *)gOvl15CardTitleZeroGlyph);
    quotient = value / 10;
    gOvl15CardTitleGlyphScratch[1] = gOvl15CardTitleGlyphScratch[1] + (value - quotient * 10);
    return (s8 *)strcat((char *)destination, (char *)gOvl15CardTitleGlyphScratch);
}

/* This replaces destination with the selected string; it does not append
 * to a string already in destination. Out-of-range seasons clamp to an end. */
void Ovl15FormatTitleSeason(s16 season, char *destination) {
    s16 index;

    *destination = 0;
    if (season >= 0) {
        index = (season < 4) ? season : 3;
    } else {
        index = 0;
    }
    strcat(destination, gOvl15CardTitleSeasonNames[index]);
}
