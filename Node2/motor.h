/** @file motor.h
 *  @brief Header-file for enabling and controlling the motor position.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef MOTOR_H
#define MOTOR_H

#define F_CPU 16000000

#include "bit_operations.h"
#include "TWI_Master.h"
#include "encoder.h"

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

typedef enum {LEFT, RIGHT} direction;

/** Function for initializing motor.
 */
void motor_init(void);

/** Function for calibrating the motors position before starting game.
 */
void motor_calibrate(void);

/** Function for finding max right and left position and resetting the encoder range.
 */
void motor_set_range(void);

/** Function for moving the motor, controlling its direction and speed.
 * @param int16_t speed - The speed of which the motor is to move with.
 */
void motor_move(int16_t speed);

/** Function for transferring the voltage of which the speed is defined.
 * @param uint8_t voltage - Level of voltage to move the motor.
 */
void motor_set_voltage(uint8_t voltage);

/** Function for changing the direction of the motor.
 *  @param direction dir - Enum type representing which direction the motor is supposed to move in.
 */
void motor_set_direction (direction dir);

/** Function for changing resolution of encoder readings so that it corresponds to the sliders measurement (0-255).
 *  @return uint8_t position - The position of the encoder in the range of 0-255.
 */
uint8_t motor_position(void);

#endif
