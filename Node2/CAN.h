/** @file CAN.h
 *  @brief Header-file for the CAN driver. High level driver sending and receiving CAN messages.
 *  @author: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef CAN_H
#define CAN_H

#include <stdint.h>
#include <stdio.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <util/delay.h>

#include "MCP2515.h"
#include "SPI.h"

#include "bit_operations.h"

#define MCP_RX_BUFF_OP_MODE_BITS 0b01100000

/** Struct for message, defining id, length and data byte.
 */
typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t data[8];
} message;

/** Function for initializing CAN communication.
 */
int CAN_init(void);

/** Function for sending a message with a given id and data using MCP2515 for CAN communication.
 *  @param message msg - The message to be sent.
 */
void CAN_send_message(message msg);

/** Function for receiving a message with a given id and data using MCP2515 for CAN communication.
 *  @return message message - The message received.
 */
message CAN_data_receive(void);

/** Function for sending GAME OVER via CAN to Node 1.
 *  @param uint8_t GAME_OVER_FLAG - Flag indicating that game is over.
 */
void CAN_transmit_game_info(uint8_t GAME_OVER_FLAG);

/** Function for testing transmit in loop-back mode.
 */
 void CAN_transmit_loopback_test(void);


#endif
