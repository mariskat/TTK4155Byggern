/** @package main.c
 *  main function for running the program - Controller. Node 2.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "CAN.h"
#include "IR.h"
#include "motor.h"
#include "PID.h"
#include "PWM.h"
#include "solenoid.h"
#include "SPI.h"
#include "USART.h"

#include <util/delay.h>
#include <avr/interrupt.h>

#define F_CPU 16000000

#define MAX_MISSES 3


void main() {

    sei();
    USART_init(9600);
    CAN_init();

    IR_init();

    PWM_init();
    solenoid_init();
    motor_init();

    PID *pid;
    PID_init(pid);

    uint8_t curr_number_of_misses = 0;

    message msg = CAN_data_receive();
    uint8_t game_state = msg.data[5];
    uint8_t position = motor_position();

    while (1) {

        message msg = CAN_data_receive();

        // Set PID parameters
        PID_set_parameters(pid, msg.data[6]);

        position = motor_position();

        if (game_state == 1) {
            // Check if ball miss
            curr_number_of_misses = counting_goals();

            // Have you reached game over
            if (curr_number_of_misses >= MAX_MISSES) {
                game_state = 0;

                // Send game over to Node 1
                CAN_transmit_game_info(1);
                reset_goals();

                _delay_ms(200);

                // Send not game over to Node 1
                CAN_transmit_game_info(0);
            }

            else {
                // Control servo based on joystick signal (x-axis)
                double duty_cycle = PWM_joystick_to_duty_cycle(msg);
                PWM_set_duty_cycle(duty_cycle);

                // Control the motor based on the left slider movement.
                PID_controller(pid, msg);

                // Punch solenoid when left button pressed.
                solenoid_control(msg);

                // Update game state
                game_state = msg.data[5];
            }

        }

        // If game is ended
        else if (game_state == 0) {
            // Reset goals
            reset_goals();

            // Reset error variables in PID
            PID_reset(pid);

            // Update game state
            game_state = msg.data[5];

        }

        else {
            // Update game state
            game_state = msg.data[5];
        }
    }
}
