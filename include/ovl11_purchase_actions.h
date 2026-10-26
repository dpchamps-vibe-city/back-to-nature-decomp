#ifndef OVL11_PURCHASE_ACTIONS_H
#define OVL11_PURCHASE_ACTIONS_H

#include "common.h"

/* Overlay-11 selection-table callbacks 4..14, in original code/table order.
 * Service once per interaction tick: 0=pending, nonzero=finished (including
 * a gate rejecting the request). All reuse the interaction step/draw mode,
 * message window and selection scratch. Only one transaction may be active;
 * initialize those services and their item/price tables before dispatch.
 * Catalog numbers are opaque IDs, not inferred gameplay item names.
 * The underlying services can debit money and update item/object state.
 */

/* Mode-0 catalog requests, bank 2, prompt IDs 1DD/1DE/D9/DA/DB.
 * Original identities: ovl_11:80119338/364/390/3BC/3E8. */
s32 Ovl11PurchaseCatalogEntry99(void);
s32 Ovl11PurchaseCatalogEntry9A(void);
s32 Ovl11PurchaseCatalogEntryA5(void);
s32 Ovl11PurchaseCatalogEntryA6(void);
s32 Ovl11PurchaseCatalogEntryA7(void);

/* Mode 1: status bit 0x40 selects ID 6D, else bit 0x80 selects 6E;
 * neither set completes without starting a request. 80119414. */
s32 Ovl11PurchaseFlagSelectedEntry(void);

/* Mode 2, ID 64: requires gate bit 0x20. Status bit 0x800 instead emits
 * bank-2 message D6 and completes; otherwise uses prompt D5. 80119470. */
s32 Ovl11PurchaseGatedEntry64(void);

/* Descriptor-driven quantity requests using gOvl11PrimarySelectionRecord.
 * Option variants complete immediately unless the current option matches
 * 0/1/2, except that nonzero +44D4 bypasses that gate and substitutes message
 * 275. Base descriptor prompts are BE/273/274; start/next/cancel are
 * C0/BB/C2. Fixed uses 164 with start/next/cancel 16E/170/171, without a gate.
 * All messages use bank 2; the descriptor selects interaction draw mode 4.
 * Original identities: ovl_11:801194DC/801195BC/8011969C/8011977C. */
s32 Ovl11PurchaseQuantityOption0(void);
s32 Ovl11PurchaseQuantityOption1(void);
s32 Ovl11PurchaseQuantityOption2(void);
s32 Ovl11PurchaseQuantityFixed(void);

#endif
