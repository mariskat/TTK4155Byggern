/** @file UART.c
 *  @brief c-file for the UART driver - to initialize, recieve and transfer data.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "UART.h"


/** Function for initializing UART.
 *  @param unsigned int ubrr - UBRR register, User Baud Rate Register
 */
void UART_init (unsigned int ubrr) {
    int bd = 0.15*((long)FOSC/((long)(16*ubrr))-1);

    UBRR0H = (unsigned char) (bd>>8);
    UBRR0L = (unsigned char) bd;

    UCSR0B = (1<<RXEN0) | (1<<TXEN0);
    UCSR0C = (1<<USBS0) | (1<<URSEL0) | (0<<UCSZ10) | (3<<UCSZ00);

    fdevopen(UART_trans, UART_recv);
}

/** Function for recieving data from UART.
 *  @return UDR0 - Empty Handler, if register RXC0 (RX complete) is not set.
 */
unsigned char UART_recv (void) {
     if(!(UCSR0A & (1<<RXC0))){
        return 0;
    }
    return UDR0;
}

/** Function for transferring data from UART.
 *  @param unsigned char letter - Letter to be transferred.
 */
void UART_trans (unsigned char letter) {
    while (!(UCSR0A & (1 << UDRE0))){};

    UDR0 = letter;
}
