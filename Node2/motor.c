/** @file motor.c
 *  @brief C-file for enabling and controlling the motor position.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "motor.h"

#define MAX_RESOLUTION 255

int16_t MIN_ENCODER_VALUE = 0;
int16_t MAX_ENCODER_VALUE = 0;

/** Function for initializing motor.
 */
void motor_init(void) {
    TWI_Master_Initialise();

    // Set enable pin
    set_bit(DDRH, PH4);
    set_bit(PORTH, PH4);

    // Set direction pin
    set_bit(DDRH, PH1);

    // Initialize encoder
    encoder_init();

    // Data bits
    clear_bit(DDRK, PK0);
    clear_bit(DDRK, PK1);
    clear_bit(DDRK, PK2);
    clear_bit(DDRK, PK3);
    clear_bit(DDRK, PK4);
    clear_bit(DDRK, PK5);
    clear_bit(DDRK, PK6);
    clear_bit(DDRK, PK7);

    // Reseting encoder in a known position (to the far left)
    motor_calibrate();

    // Finding min and max encoder values
    motor_set_range();

}

/** Function for calibrating the motors position before starting game.
 */
void motor_calibrate(void) {
    // Moves motor to the left
    motor_move(-100);
    _delay_ms(1000);

    int16_t rotation = encoder_read();
    int16_t prev_rotation = rotation + 300;

    while (rotation != prev_rotation) {
        prev_rotation = rotation;
        _delay_ms(100);
        rotation = encoder_read();
    }

    motor_move(0);
    _delay_ms(180);
    encoder_reset();
}


/** Function for finding max right and left position and resetting the encoder range.
 */
void motor_set_range(void) {
    motor_move(-100);
    _delay_ms(1000);
    MIN_ENCODER_VALUE = encoder_read();

    motor_move(100);
    _delay_ms(1000);
    MAX_ENCODER_VALUE = (-1) * encoder_read();

    motor_move(0);

    printf("MIN ENCODER VALUE %i \n\r", MIN_ENCODER_VALUE);
    printf("MAX ENCODER VALUE %i \n\r", MAX_ENCODER_VALUE);
}


/** Function for moving the motor, controlling its direction and speed.
 * @param int16_t speed - The speed of which the motor is to move with.
 */
void motor_move(int16_t speed) {

    uint8_t voltage = 0b0;

    if (speed < 0) {
        motor_set_direction(LEFT);
        voltage = (-1) * speed;
    }

    else if (speed >= 0){
        motor_set_direction(RIGHT);
        voltage = speed;
    }

    motor_set_voltage(voltage);
}

/** Function for transferring the voltage of which the speed is defined.
 * @param uint8_t voltage - Level of voltage to move the motor.
 */
void motor_set_voltage(uint8_t voltage) {
    uint8_t address = 0b01010000;
    uint8_t command = 0b00;

    uint8_t message[3];
    message[0] = address;
    message[1] = command;
    message[2] = voltage;

    TWI_Start_Transceiver_With_Data(message, 3);
}

/** Function for changing the direction of the motor.
 *  @param direction dir - Enum type representing which direction the motor is supposed to move in.
 */
void motor_set_direction (direction dir) {
    switch (dir) {
        case LEFT:
            // Sets direction to left
            clear_bit(PORTH, PH1);
            break;

        case RIGHT:
            // Sets direction to right
            set_bit(PORTH, PH1);
            break;

        default:
            break;
    }
}

/** Function for changing resolution of encoder readings so that it corresponds to the sliders measurement (0-255).
 *  @return uint8_t position - The position of the encoder in the range of 0-255.
 */
uint8_t motor_position(void) {

    uint16_t encoder_value = (-1) * encoder_read();

    encoder_value -= MIN_ENCODER_VALUE;

    uint8_t position = ((double)encoder_value/(double)MAX_ENCODER_VALUE) * MAX_RESOLUTION;

    return position;
}
