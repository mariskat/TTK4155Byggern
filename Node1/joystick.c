/** @file joystick.c
 *  @brief C-file for the joystick on the USB multiboard's behaviour.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "joystick.h"

// ADC_channel with hexadecimal representation of channel wanted from ADC
#define X_AXIS_CHANNEL 5
#define Y_AXIS_CHANNEL 4

// Slack variable for neutral position of joystick
#define SLACK 5

// Resolution of joystick
#define RESOLUTION_START 0
#define RESOLUTION_END 255

// Origo of joystick area is approximately at (128, 128) with a resolution scale 0-255, can be calibrated using calibrate function
int resolution_left = 128;
int resolution_right = 128;


/** Function for detecting if joystick button is pressed.
 *  @return int joystick_button - Returns 0 if joystick button is pressed, 1 otherwise.
 */
int joystick_button_not_pressed(void) {
    int joystick_button = (PINB & (1 << PINB2));
    _delay_ms(100);
    return joystick_button;
}

/** Function for detecting if touch button is pressed.
 * @return bool - Returns 1 if touch button is pressed, 0 otherwise.
 */
bool touch_button_pressed() {
    return ((PINB & (1 << PINB0)) || (PINB & (1 << PINB1)));
}

/** Function for calibrating the joystick neutral position (neutral = origo).
 */
void joystick_calibrate(void) {
    joystick position;

    position.x = selected_channel_output(X_AXIS_CHANNEL);
    position.y = selected_channel_output(Y_AXIS_CHANNEL);

    resolution_left = position.x;
    resolution_right = RESOLUTION_END - position.x;
}

/** Function for returning the quadrant the joystick is position in by reading x-and y-position.
 *  @param struct Joystick position - Struct that yields the x- and y-values respectively.
 *  @return int 1-4 - Quadrant the joystick is in.
 */
int get_quadrant(joystick position) {
    if (position.x < SLACK && position.x > -SLACK && position.y < SLACK && position.y > -SLACK) {
        return 0;
    }
    else if (position.x >= 0 && position.y > 0) {
        return 1;
    }
    else if (position.x < 0 && position.y >= 0) {
        return 2;
    }
    else if (position.x <= 0 && position.y < 0) {
        return 3;
    }
    else if (position.x > 0 && position.y <= 0) {
        return 4;
    }
    else {
        return 10000;
    }
}

/** Function checks whether the direction is up or down, as opposed to right or left.
 *  @param struct Joystick position - Struct that yields the x- and y-values respectively.
 *  @return bool true or false - up or down (true), right or left (false).
 */
bool is_vertical_direction(joystick position) {
    int abs_x = abs(position.x);
    int abs_y = abs(position.y);

    if (abs_y > abs_x) {
        return true;
    }
    else {
        return false;
    }
}

/** Function converts digital signal from joysticks x- and y-values with voltage resolution 0-255 to percent-representation -100-100.
 *  @return joystick position - Struct containing x-and-y-positions of joysticks represented as percentage of displacement -100-100.
 */
joystick joystick_position(void) {
    joystick position;

    // Converting from 0-255 resolution to -100-100 resolution in x-axis
    if ((selected_channel_output(X_AXIS_CHANNEL) >= (resolution_left)) && (selected_channel_output(X_AXIS_CHANNEL) <= (resolution_left))) {
        position.x = 0;
    }
    else if (selected_channel_output(X_AXIS_CHANNEL) < resolution_left) {
        position.x = -(((resolution_left-selected_channel_output(X_AXIS_CHANNEL))*100)/resolution_left);
    }
    else {
        position.x = (((selected_channel_output(X_AXIS_CHANNEL)-resolution_left)*100)/resolution_right);
    }

    // Converting from 0-255 resolution to -100-100 resolution in y-axis
    if ((selected_channel_output(Y_AXIS_CHANNEL) >= (resolution_left)) && (selected_channel_output(Y_AXIS_CHANNEL) <= (resolution_left))) {
        position.y = 0;
    }

    else if (selected_channel_output(Y_AXIS_CHANNEL) < resolution_left) {
        position.y = -(((resolution_left-selected_channel_output(Y_AXIS_CHANNEL))*100)/resolution_left);
    }

    else {
        position.y = (((selected_channel_output(Y_AXIS_CHANNEL)-resolution_left)*100)/resolution_right);
    }


    return position;
}

/** Function takes the direction and quadrant into consideration to decide in which direction the joystick is pointing (the most) towards.
 *  @return enum direction - direction, either UP, DOWN, LEFT, RIGHT, NEUTRAL or UNKNOWN.
 */
direction joystick_direction(void) {

    joystick position = joystick_position();

    int quadrant = get_quadrant(position);

    bool upDown = is_vertical_direction(position);

    switch(quadrant){
        case 0:
            return NEUTRAL;

        case 1:
            if (upDown){
                return UP;
            }
            else {
                return RIGHT;
            }

        case 2:
            if (upDown){
                return UP;
            }
            else {
                return LEFT;
            }

        case 3:
            if (upDown) {
                return DOWN;
            }
            else {
                return LEFT;
            }

        case 4:
            if (upDown){
                return DOWN;
            }
            else {
                return RIGHT;
            }

        default:
            return UNKNOWN;

    }

}

/** Test function for reading joystick position.
 */
void test_read_joystick_position(void) {
    _delay_ms(500);

    joystick joy_position = joystick_position();

    printf("X-value: %i\n\r", joy_position.x);
    printf("Y-value: %i\n\r", joy_position.y);

}
