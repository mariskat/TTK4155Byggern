/** @file solenoid.c
 *  @brief c-file for controlling the solenoid.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "solenoid.h"

/** Function for initializing the solenoid by enabling pins.
 */
void solenoid_init() {
    set_bit(DDRB, PB4);
    set_bit(PORTB, PB4);
}

/** Function for executing a solenoid punch if button is pressed.
 */
void solenoid_control(message msg) {
    if (msg.data[2] == 1) {
        solenoid_punch();
    }
}

/** Function for toggling the solenoid pins and executing a pulse-movement.
 */
void solenoid_punch(void){
    clear_bit(PORTB, PB4);
    _delay_ms(300);
    set_bit(PORTB, PB4);
}
