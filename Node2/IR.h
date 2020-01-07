/** @file IR.h
 *  @brief Header file for the IR
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef IR_H
#define IR_H

#include <avr/io.h>
#include <stdint.h>

#include "bit_operations.h"

#define ADC0 PF0

#define NUM_MEASUREMENTS 4
#define GOAL_LIMIT 50

// Global flag interrupt for detecting goal
extern int BALL_DETECTED_FLAG;

/** Initialize Arduino shield for IR-communication.
 */
void IR_init(void);

/** Function for reading the signal from IR photodiode, detecting a goal.
 *  @return uint16_t ADC - Analog turned digital signal from the photodiode.
 */
uint16_t IR_read_photodiode(void);

/** Reading the digital filtered signal from the photodiode.
 *  @return (sum/NUM_MEASUREMENTS) - The average value/filtered value of the photodiode signal.
 */
uint16_t IR_read_filtered_photodiode(void);

/** Function for resetting goals.
 */
void reset_goals(void);

/** Test function for counting goals.
 */
void test_counting_goals(void);

#endif
