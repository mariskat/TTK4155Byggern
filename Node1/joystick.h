/** @file joystick.h
 *  @brief H-file considering the joystick on the USB-multiboards behaviour.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef JOYSTICK_H
#define JOYSTICK_H

#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "ADC.h"
#include "CAN.h"
#include "slider.h"


/** Struct joystick representing the x- and y-axis respectively.
 */
typedef struct {
    int x;
    int y;
} joystick;

/** Enum direction representing the directions of the joystick.
 */
typedef enum {LEFT, RIGHT, UP, DOWN, NEUTRAL, UNKNOWN} direction;


/** Function for detecting if joystick button is pressed.
 *  @return int joystick_button - Returns 0 if joystick button is pressed, 1 otherwise.
 */
int joystick_button_not_pressed(void);

/** Function for detecting if touch button is pressed.
 * @return bool - Returns 1 if touch button is pressed, 0 otherwise.
 */
bool touch_button_pressed();

/** Function for calibrating the joystick neutral position (neutral = origo).
 */
void joystick_calibrate(void);

/** Function for returning the quadrant the joystick is position in by reading x-and y-position.
 *  @param struct Joystick position - Struct that yields the x- and y-values respectively.
 *  @return int 1-4 - Quadrant the joystick is in.
 */
int get_quadrant(joystick position);

/** Function checks whether the direction is up or down, as opposed to right or left.
 *  @param struct Joystick position - Struct that yields the x- and y-values respectively.
 *  @return bool true or false - up or down (true), right or left (false).
 */
bool is_vertical_direction(joystick position);

/** Function converts digital signal from joysticks x- and y-values with voltage resolution 0-255 to percent-representation -100-100.
 *  @return joystick position - Struct containing x-and-y-positions of joysticks represented as percentage of displacement -100-100.
 */
joystick joystick_position(void);

/** Function takes the direction and quadrant into consideration to decide in which direction the joystick is pointing (the most) towards.
 *  @return enum direction - direction, either UP, DOWN, LEFT, RIGHT, NEUTRAL or UNKNOWN.
 */
direction joystick_direction(void);

/** Test function for reading joystick position.
 */
void test_read_joystick_position(void);

#endif
