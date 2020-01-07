/** @file MCP2515.c
 *  @brief c-file for driver of MCP2515, makes it possible to read and write using SPI, as well as modifying bits, reading MCP status and requesting to send.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "MCP2515.h"

/** Function for initializing MCP and checking if it is in configuration mode.
 * @return uint8_t
 */
uint8_t MCP_init(void){
    //Initialize SPI
    SPI_init();

    //Send reset-command
    MCP_reset();

    MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_CONFIG);

    //Test self
    uint8_t value;
    value = MCP_read(MCP_CANSTAT);
    printf("Value: %i\n\r", value & MODE_MASK);

    if ((value & MODE_MASK)!= MODE_CONFIG) {
        printf("MCP2515 is not in configuration mode after reset!\n\r");
        return 1;
    }
    return 0;
}


/** Function for reading data stored at spesific address from MCP2515.
 * @param uint8_t address - Select address you want data from
 * @return uint8_t character result - The data stored at address selected
 */
uint8_t MCP_read(uint8_t address){
    uint8_t result;

    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Send read instruction
    SPI_read_write(MCP_READ);

    // Send address to shift data stored at address to SO pin
    SPI_read_write(address);

    // Read result
    result = SPI_read_write(0x00);

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);

    return result;
}

/** Function for writing data address of the MCP2515
 * @param char data
 * @param uint8_t address
 */
void MCP_write(uint8_t address, char data){
    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Sending write instruction
    SPI_read_write(MCP_WRITE);

    // Sending address
    SPI_read_write(address);

    // Sending byte of data
    SPI_read_write(data);

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);
}

/** Function for initiating message transmission for one or more of the transmit buffers.
 */
void MCP_request_to_send(uint8_t bit){
    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Send RTS command byte. The last 3 bits of it indicate which transmit buffers are enabled to send.
    switch (bit) {
        case 0:
            SPI_read_write(MCP_RTS_TX0);
        case 1:
            SPI_read_write(MCP_RTS_TX1);
        case 2:
            SPI_read_write(MCP_RTS_TX2);
        default:
            SPI_read_write(MCP_RTS_ALL);
    }

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);
}

/** Function for allowing single instruction access to some of the often used status bits
 * for message reception and transmission.
 * @return uint8_t status - Status for the MCP
 */
uint8_t MCP_read_status(void){
    uint8_t status;

    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Sending read status command byte
    SPI_read_write(MCP_READ_STATUS);

    // Reading status
    status = SPI_read_write(0x00);

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);

    return status;
}

/** Function for setting or clearing individual bits in specific status and control registers.
 * @param uint8_t address - The address of the register you want to modify
 * @param uint8_t mask - Mask determines which bit in register will be allowed to change
 * @param uint8_t data - Data byte determines what value the modified bits in the register will be changed to.
 */
void MCP_bit_modify(uint8_t address, uint8_t mask, uint8_t data){
    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Send modify command
    SPI_read_write(MCP_BITMOD);

    // Send address of register
    SPI_read_write(address);

    // Send mask, which bit in register allowed to change
    SPI_read_write(mask);

    // Send data byte, what value the modified bits will change to.
    SPI_read_write(data);

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);
}

/** Function for resetting the internal registers of MCP2515, and setting configuration mode.
 */
void MCP_reset(void){
    // Select CAN-controller with chip select
    clear_bit(PORTB, CAN_CS);
    clear_bit(PORTB, SS);

    // Send reset instruction
    SPI_read_write(MCP_RESET);

    // Deselect CAN-controller with chip select
    set_bit(PORTB, CAN_CS);
    set_bit(PORTB, SS);
}
