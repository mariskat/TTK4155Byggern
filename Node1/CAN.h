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
#include "joystick.h"
#include "slider.h"

#include "bit_operations.h"

/** Struct for message, defining id, length and data byte.
 */
typedef struct {
    uint8_t id;
    uint8_t length;
    uint8_t data[8];
} message;

/** Function for initializing CAN.
 *  @return int
 */
int CAN_init(void);

/** Function for sending a message with a given id and data using MCP2515 for CAN communication.
 *  @param message msg - message to be sent via CAN
 */
void CAN_send_message(message msg);

/** Function for receiving a message with a given id and data using MCP2515 for CAN communication.
 *  @return message msg - message received via CAN
 */
message CAN_data_receive(void);

/** Function for sending joystick position, buttons, slider positions and play-game flag via CAN to Node 2.
 *  @param joystick position - Position of joystick, struct containing x and y-positions.
 *  @param Sliders slider_position - position of slider right and left.
 *  @param PLAY_GAME_FLAG - Flag set when play game is selected in the main menu.
 *  @param DIFFICULTY_FLAG - Flag set to 0 for EASY, 1 for MEDIUM and 2 for HARD difficulty when playing game.
 */
void CAN_transmit_game_controller(joystick position, Sliders slider_position, int PLAY_GAME_FLAG, int DIFFICULTY_FLAG);

/** Function which returns the most recently received CAN message.
 * @return message CAN_msg - the most recently received CAN message.
 */
message CAN_recent_message(void);

/** Function for testing transmit of CAN messages in loop-back mode.
 */
 void CAN_transmit_loopback_test(void);

#endif
