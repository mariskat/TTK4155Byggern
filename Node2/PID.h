/** @file PID.h
 *  @brief Header-file for the PID-controller, controlling the motor movement.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef PID_H
#define PID_H

#include "CAN.h"
#include "motor.h"

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>

#define SCALING_FACTOR 128

// Define max control variable
#define MAX_CONTROL_VALUE 1023

extern int PID_FLAG;

typedef enum {EASY = 0, MEDIUM, HARD} difficulty;

typedef struct {
    // Tuning variables, multiplied by SCALING_FACTOR
    int16_t K_p;
    int16_t K_i;
    int16_t K_d;

    int16_t last_error;
    int32_t sum_errors;
} PID;


/** Function for initializing the PID-controller.
 * @param PID* pid - PID controller
 */
void PID_init(PID* pid);

/** Function for resetting the PID-controller.
 * @param PID* pid - PID controller
 */
void PID_reset(PID* pid);

/** Function for calculating the error and introducing integral and derivative-effects to return the control variable.
 * @param uint8_t reference_value - The current position of the slider.
 * @param uint8_t process_value - The current position of the motor, read from encoder.
 * @param PID* pid - PID controller
 * @return int16_t control_variable - Control variable to control the motor.
 */
int16_t PID_calculate_control(uint8_t reference_value, uint8_t process_value, PID* pid);

/** Function for moving motor according to the reference and process position, controlled by PID-controller.
 * @param PID* pid - PID controller.
 * @param message msg - Message from CAN, including the slider position.
 */
void PID_controller(PID* pid, message msg);

/** Function for setting the tuning parameters of the PID.
 * @param PID* pid - PID controller.
 * @param difficulty mode - enum difficulty
 */
void PID_set_parameters(PID* pid, difficulty mode);

#endif
