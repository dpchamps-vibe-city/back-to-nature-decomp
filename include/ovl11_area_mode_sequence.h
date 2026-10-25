#ifndef OVL11_AREA_MODE_SEQUENCE_H
#define OVL11_AREA_MODE_SEQUENCE_H

/* ovl_11 must be resident. These entry points operate on the current
 * work-area selection/asset record and shared scene, fade and sound state;
 * no explicit parameters or return values. They are not reentrant. */

/* Step the area-mode machine once on the existing scratchpad stack.
 * Commits selection state, polls archive reads, waits for fades and may
 * request a resident scene change. Do not call recursively on that stack. */
void Ovl11UpdateAreaMode(void);

/* Phase-table callbacks. Reset clears the pending mode and entry step,
 * and advances the scene phase. Run performs the current entry step:
 * 0..7 is normal entry; 9..15 is the alternate path. It advances only
 * when prerequisites finish, and may synchronously drive update loops. */
void Ovl11ResetAreaModeStep(void);
void Ovl11RunAreaEntrySteps(void);

/* Choose the current bank, synchronously fade out an old selection when
 * needed, rewind/queue its replacement and record whether replay is needed.
 * Queuing does not imply the transfer has completed. */
void Ovl11RefreshAreaSoundBank(void);

/* Apply the asset/clock-dependent sequence volume, restarting sequence 0
 * on the selected bank when requested, otherwise ramping its slot volume.
 * Reads the installed asset header and sound-restart flag. */
void Ovl11ApplyAreaSoundVolume(void);

#endif
