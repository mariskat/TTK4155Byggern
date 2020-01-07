/** @file encoder.h
 *  @brief Header-file for reading, resetting and initializing the motor encoder.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef ENCODER_H
#define ENCODER_H

#define F_CPU 16000000

#include "bit_operations.h"

#include <stdint.h>
#include <avr/io.h>
#include <util/delay.h>

/** Function for initializing the encoder.
 */
void encoder_init(void);

/** Function for resetting the encoder.
 */
void encoder_reset(void);

/** Function for reading the encoder.
 *  @return int16_t ((high_byte << 8) | low_byte) - The 16 bit encoder data.
 */
int16_t encoder_read(void);

#endif
