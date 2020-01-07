/** @file CAN.c
 *  @brief c-file for the CAN driver. High level driver sending and receiving CAN messages.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "CAN.h"

message CAN_msg;

/** Function for initializing CAN.
 *  @return int
 */
int CAN_init(void){
    // Reset MCP2515 to configuration mode and test self
    MCP_init();

    //Enable all receive interrupts
    MCP_bit_modify(MCP_CANINTE, 0b11, MCP_RX_INT);

    // INT1 intflag is cleared by writing 1 to INTF1
    set_bit(GIFR, INTF1);

    // Enable interrupt on PD3
    set_bit(GICR, INT1);

    // Set normal mode
	MCP_bit_modify(MCP_CANCTRL, MODE_MASK, MODE_NORMAL);

    uint8_t value;
    value = MCP_read(MCP_CANSTAT);

    // Check if mode is normal
    if ((value & MODE_MASK) != MODE_NORMAL) {
        printf("MCP2515 is not in NORMAL mode!\n\r");
    }

    else {
        printf("MCP2515 is in NORMAL mode!\n\r");
    }

    return 0;
}

/** Function for sending a message with a given id and data using MCP2515 for CAN communication.
 *  @param message msg - message to be sent via CAN
 */
void CAN_send_message(message msg){

    // Write id for transmit buffers enabled to send to SIDH and SIDL registers
    MCP_write(MCP_TXB0SIDH, (msg.id >> 3));
    MCP_write(MCP_TXB0SIDL, (msg.id << 5));

    // Write data length to DLC transmit register
    MCP_write(MCP_TXB0DLC, msg.length);

    // Write each bit of data to transmit register
    for (uint8_t i = 0; i < msg.length; i++){
        MCP_write((MCP_TXB0D + i), msg.data[i]);
    }

    MCP_request_to_send(0);
}

/** Function for receiving a message with a given id and data using MCP2515 for CAN communication.
 *  @return message msg - message received via CAN
 */
message CAN_data_receive(void){
    message msg;
    //Read message id
    msg.id = (MCP_read(MCP_RXB0SIDH) << 3)+ (MCP_read(MCP_RXB0SIDL) >> 5);

    // Read message length
    msg.length = MCP_read(MCP_RXB0DLC);

    // Read each bit of data from receive register
    for (uint8_t i = 0; i < msg.length; i++){
        msg.data[i] = MCP_read(MCP_RXB0D + i);
    }

    // Set interrupt flag to clear to send several times
    MCP_write(MCP_CANINTF, 0x00);

    return msg;
}

/** Function for sending joystick position, buttons, slider positions and play-game flag via CAN to Node 2.
 *  @param joystick position - Position of joystick, struct containing x and y-positions.
 *  @param Sliders slider_position - position of slider right and left.
 *  @param PLAY_GAME_FLAG - Flag set when play game is selected in the main menu.
 *  @param DIFFICULTY_FLAG - Flag set to 0 for EASY, 1 for MEDIUM and 2 for HARD difficulty when playing game.
 */
void CAN_transmit_game_controller(joystick position, Sliders slider_position, int PLAY_GAME_FLAG, int DIFFICULTY_FLAG) {
    message msg;
    msg.length = 7;
    msg.id = 0;

    msg.data[0] = position.x;
    msg.data[1] = position.y;
    msg.data[2] = touch_button_pressed();
    msg.data[3] = slider_position.Left;
    msg.data[4] = slider_position.Right;
    msg.data[5] = PLAY_GAME_FLAG;
    msg.data[6] = DIFFICULTY_FLAG;

    CAN_send_message(msg);
}

/** Function which returns the most recently received CAN message.
 * @return message CAN_msg - the most recently received CAN message.
 */
message CAN_recent_message(void) {
    return CAN_msg;
}

/** Function for testing transmit of CAN messages in loop-back mode.
 */
 void CAN_transmit_loopback_test(void){
    message msg;
    msg.data[0] = 0xff;
    msg.id = 1;
    msg.length = 1;
    int stop = 1;
    while (stop < 10) {
        CAN_send_message(msg);

        message received = CAN_data_receive();

        for (int i = 0; i < received.length; i++){
            printf("%u", received.data[i]);
        }
        stop++;
    }
}

/** Interrupt vector function for CAN for handling CAN interrupts by saving what is received by CAN in global variable CAN_msg.
 *  @param INT1_vect - interrupt vector for CAN.
 */
ISR(INT1_vect){
    CAN_msg = CAN_data_receive();
}
