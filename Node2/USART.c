/** @file USART.c
 *  @brief c-file for the USART driver - to initialize, recieve and transfer data.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */
#include "USART.h"

/** Function for initializing USART.
 *  @param unsigned int ubrr - UBRR register, User Baud Rate Register
 */
void USART_init (unsigned int bd) {
    // Setting baud rate
    int ubrr = 0.15*((long)FOSC/((long)(16*bd))-1);

    UBRR0H = (unsigned char) (ubrr >> 8);
    UBRR0L = (unsigned char) (ubrr);

    // Enabling transmitter/receiver
    set_bit(UCSR0B, RXEN0);
    set_bit(UCSR0B, TXEN0);

    fdevopen(USART_trans, USART_recv);
}

/** Function for recieving data from USART.
 *  @return UDR0 - Empty Handler, if register RXC0 (RX complete) is not set.
 */
unsigned char USART_recv (void) {
    // Wait for data to be received
    loop_until_bit_is_set(UCSR0A, RXC0);

    return UDRE0;
}

/** Function for transferring data from USART.
 *  @param unsigned char letter - Letter to be transferred
 */
void USART_trans (unsigned char letter) {
    // Wait for empty transmit buffer
    loop_until_bit_is_set(UCSR0A, UDRE0);
    UDR0 = letter;
}
