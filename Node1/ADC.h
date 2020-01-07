/** @file ADC.h
 *  @brief Header-file for the Analog to Digital Converter. Returning analog voltage output of joystick, slider and buttons from digital signals from USB-multiboard.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef ADC_H
#define ADC_H

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <avr/io.h>
#include <util/delay.h>

#include "addresses.h"

#define F_CPU 4915200

// Set and clear registers by bit
#define set_bit( reg, bit ) (reg |= (1 << bit))
#define clear_bit( reg, bit ) (reg &= ~(1 << bit))

/** Function for returning output of selected channel of the ADC.
 *  @param channel - Hexadecimal representation of binary number corresponding to channels on ADC.
 *  @return address[0] - Voltage output of selected ADC-channel
 */
uint8_t selected_channel_output(int channel);


#endif
