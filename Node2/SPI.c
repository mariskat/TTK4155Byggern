/** @file SPI.c.
 *  @brief c-file for the SPI communication driver.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "SPI.h"

/**Function for initializing communication over SPI.
 */
void SPI_init(void) {

    // Set MOSI
    set_bit(DDR_SPI, PIN_MOSI);

    // Set SCK output
    set_bit(DDR_SPI, PIN_SCK);

    // Set slave select
    set_bit(DDR_SPI, PIN_SS);

    set_bit(DDR_SPI, PB0);

    // Enable Master
    set_bit(SPCR, MSTR);

    // Set clock rate fck/16
    set_bit(SPCR, SPR0);

    // Enable SPI
    set_bit(SPCR, SPE);
}

/**Function for transmitting and receiving data over SPI.
 * @param char data - Data to send.
 * @return uint8_t SPDR - The contents of the SPDR.
 */
uint8_t SPI_read_write(uint8_t data) {
    // Start transmission
    SPDR = data;

    // Wait for transmission to complete
    loop_until_bit_is_set(SPSR, SPIF);

    return SPDR;
}

/**Function for testing SPI driver.
 * @param char data - Data to send
 */
void SPI_test(char data){
    SPI_init();

    while(1) {
        SPI_read_write(data);

    }
}
