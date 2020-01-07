/** @file SPI.h
 *  @brief Header-file for the SPI communication driver.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef SPI_H
#define SPI_H

#include <stdint.h>
#include <avr/io.h>

#include "bit_operations.h"

#define DDR_SPI DDRB
#define PIN_SS PB4
#define PIN_MOSI PB5
#define PIN_MISO PB6
#define PIN_SCK PB7

/**Function for initializing communication over SPI.
 */
void SPI_init(void);

/**Function for transmitting and receiving data over SPI.
 * @param char data - Data to send.
 * @return uint8_t SPDR - The contents of the SPDR.
 */
uint8_t SPI_read_write(uint8_t data);

/**Function for testing SPI driver.
 * @param char data - Data to send.
 */
void SPI_test(char data);

#endif
