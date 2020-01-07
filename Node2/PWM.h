/** @file PWM.h
 *  @brief Header file for the PWM
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef PWM_H
#define PWM_H

#include <avr/io.h>
#include <stdint.h>

#include "CAN.h"
#include "bit_operations.h"

#define PWM_T 20


// Prescaler divider, chosen when setting clock frequency
#define N 8

// Frequency of PWM signal, calculated based on period (T = 20 ms) => f = 1/T = 50 Hz
#define F_PWM 50


#define MAX_DEFLECTION_ANGLE_LEFT 0.900
#define MAX_DEFLECTION_ANGLE_RIGHT 2.100

// Clock frequency
#define F_CLK 16000000

/** Function for initializing PWM on the ATmega2560.
 */
void PWM_init(void);

/** Function for converting joystick position (-100 to 100) to a pulse width (1.0 - 2.0 ms) and finding the duty cycle (PW/T)
 *  @param message position - CAN message containing the position of the joystick.
 *  @return double PWM_D - duty cycle.
 */
double PWM_joystick_to_duty_cycle(message position);

/** Function for setting output compare register in the ATmega2560 to wanted duty cycle.
 *  @param double duty_cycle - the duty cycle to be set.
 */
void PWM_set_duty_cycle(double duty_cycle);

/** Function for testing that the joystick movement actually moves the servo.
 */
void test_joystick_to_servo(void);


#endif
