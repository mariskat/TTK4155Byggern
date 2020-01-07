/** @file PID.c
 *  @brief C-file for the PID-controller, controlling the motor movement.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "PID.h"

int PID_FLAG = 0;

#define ERROR_SLACK 15
#define EDGE_SLACK 30
#define MAX_RESOLUTION 255


/** Function for initializing the PID-controller.
 * @param PID* pid - PID controller
 */
void PID_init(PID* pid) {

    // Initializing error variables to zero from start
    PID_reset(pid);

    cli();

    // Enabling timer
	// Using normal mode
	set_bit(TCCR3B, CS31);

	// Enable timer interrupt
	set_bit(TIMSK3, TOIE3);

	sei();

	_delay_ms(500);

}

/** Function for resetting the PID-controller.
 * @param PID* pid - PID controller
 */
void PID_reset(PID* pid) {
    pid->last_error = 0;
    pid->sum_errors = 0;

    PID_set_parameters(pid, EASY);
}

/** Function for calculating the error and introducing integral and derivative-effects to return the control variable.
 * @param uint8_t reference_value - The current position of the slider.
 * @param uint8_t process_value - The current position of the motor, read from encoder.
 * @param PID* pid - PID controller
 * @return int16_t control_variable - Control variable to control the motor.
 */
int16_t PID_calculate_control(uint8_t reference_value, uint8_t process_value, PID* pid) {

    int16_t p_term, i_term, d_term;
    int16_t error;
    int16_t control_variable;

    if (reference_value < EDGE_SLACK) {
        reference_value = EDGE_SLACK;
    }
    else if (reference_value > (MAX_RESOLUTION - EDGE_SLACK)) {
        reference_value = MAX_RESOLUTION - EDGE_SLACK;
    }

    error = reference_value - process_value;

    // Calculate P term
    p_term = pid->K_p * error;

    // Calculcate I term
    if (abs(error) > ERROR_SLACK) {
        pid->sum_errors += error;
        i_term = pid->K_i * pid->sum_errors;
    }
    else {
        i_term = 0;
    }

    // Calculate D term
    d_term = pid->K_d * (error - pid->last_error);

    // Calculate control variable
    control_variable = (p_term + i_term + d_term)/SCALING_FACTOR;

    if (control_variable > MAX_CONTROL_VALUE) {
        control_variable = MAX_CONTROL_VALUE;
    }

    else if (control_variable < -MAX_CONTROL_VALUE) {
        control_variable = -MAX_CONTROL_VALUE;

    }

    return control_variable;

}

/** Function for moving motor according to the reference and process position, controlled by PID-controller.
 * @param PID* pid - PID controller.
 * @param message msg - Message from CAN, including the slider position.
 */
void PID_controller(PID* pid, message msg) {
    if (PID_FLAG == 1) {
        // Get reference and process values
        uint8_t reference_value = msg.data[3]; // Left slider (0 - 255)
        //printf("REFERENCE VALUE: %u \n\r", reference_value);

        uint8_t process_value = motor_position();
        //printf("PROCESS VALUE: %u \n\r", process_value);

        // Calculate the control variable
        int16_t control_value = PID_calculate_control(reference_value, process_value, pid);
        //printf("CONTROL VALUE: %i \n\r", control_value);

        // Apply control on system
        motor_move(control_value);

        PID_FLAG = 0;

    }
}

/** Function for setting the tuning parameters of the PID.
 * @param PID* pid - PID controller.
 * @param difficulty mode - enum difficulty
 */
void PID_set_parameters(PID* pid, difficulty mode) {
    switch(mode) {
        case EASY:
            pid->K_p = 1 * SCALING_FACTOR;
            pid->K_i = 0.03 * SCALING_FACTOR;
            pid->K_d = 0.02 * SCALING_FACTOR;
            break;

        case MEDIUM:
            pid->K_p = 1 * SCALING_FACTOR;
            pid->K_i = 0.08 * SCALING_FACTOR;
            pid->K_d = 0.09 * SCALING_FACTOR;
            break;

        case HARD:
            pid->K_p = 2.5 * SCALING_FACTOR;
            pid->K_i = 2 * SCALING_FACTOR;
            pid->K_d = 0.1 * SCALING_FACTOR;
            break;

        default:
            pid->K_p = 1 * SCALING_FACTOR;
            pid->K_i = 0.03 * SCALING_FACTOR;
            pid->K_d = 0.02 * SCALING_FACTOR;
            break;
    }
}

/** Interrupt service routine which is executed at period set by TIMER3, setting the flag for updating control variable for PID.
 */
ISR(TIMER3_OVF_vect) {
    PID_FLAG = 1;
}
