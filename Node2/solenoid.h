/** @file solenoid.h
 *  @brief Header-file for controlling the solenoid.
 *  @author: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef SOLENOID_H
#define SOLENOID_H

#include "CAN.h"
#include "bit_operations.h"

#include <avr/io.h>
#include <stdbool.h>

/** Function for initializing the solenoid by enabling pins.
 */
void solenoid_init();

/** Function for executing a solenoid punch if button is pressed.
 */
void solenoid_control(message msg);

/** Function for toggling the solenoid pins and executing a pulse-movement.
 */
void solenoid_punch(void);


#endif
