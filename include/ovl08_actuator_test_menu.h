#ifndef OVL08_ACTUATOR_TEST_MENU_H
#define OVL08_ACTUATOR_TEST_MENU_H

/* ovl_08:800B8400. Service one frame of the independent test page. Requires
 * initialized pad-0 input, actuator sequencer and debug font. Pressed bits
 * 0x08/0x02 increment/decrement the retained preset in 0..10; 0x40 requests
 * it on software sequence slot 0, and 0x80 stops that slot immediately.
 * Tests are independent and run in that order (request then stop if both).
 * Prints PADNO=<preset> each call; does not flush the font or poll input.
 * No retail caller was found; this is not part of the scene-menu dispatch.
 */
void Ovl08UpdateActuatorTestMenu(void);

#endif
