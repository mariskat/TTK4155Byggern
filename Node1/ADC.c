/** @file ADC.c
 *  @brief C-file for the Analog to Digital Converter. Switches the channels by writing wanted channels to ADC external memory and returning the channels output.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "ADC.h"

/** Function for returning output of selected channel of the ADC.
 *  @param channel - Hexadecimal representation of binary number corresponding to channels on ADC.
 *  @return address[0] - Voltage output of selected ADC-channel
 */
uint8_t selected_channel_output(int channel){
    // Write to the channel wanted on the ADC
    volatile char *address = adc_addr;
    address[0] = channel;

    // Delaying time t_c, waiting for writetoggle to be registered
    _delay_us(500);

    // Returning 8 bit digital signal from ADC, converted from channel specified in input
    return address[0];
}
