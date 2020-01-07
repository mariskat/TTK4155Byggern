/** @file encoder.c
 *  @brief C-file for reading, resetting and initializing the motor encoder.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */
#include "encoder.h"

/** Function for initializing the encoder.
 */
void encoder_init(void) {
    // SEL
    set_bit(DDRH, PH3);

    // OE
    set_bit(DDRH, PH5);

    // RST
    set_bit(DDRH, PH6);

    // Reset encoder
    encoder_reset();
}

/** Function for resetting the encoder.
 */
void encoder_reset(void) {
    clear_bit(PORTH, PH6);
    _delay_us(200);
    set_bit(PORTH, PH6);
}

/** Function for reading the encoder.
 *  @return int16_t ((high_byte << 8) | low_byte) - The 16 bit encoder data.
 */
int16_t encoder_read(void) {
    // !OE low to enable output from encoder
    clear_bit(PORTH, PH5);

    // SEL low to select high byte
    clear_bit(PORTH, PH3);
    _delay_us(100);

    // Read high byte
    uint8_t high_byte = PINK;

    // SEL high to select low byte
    set_bit(PORTH, PH3);
    _delay_us(100);

    uint8_t low_byte = PINK;

    // !OE high to disable output from encoder
    set_bit(PORTH, PH5);

    return (int16_t) ((high_byte << 8) | low_byte);

}
