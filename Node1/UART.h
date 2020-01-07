/** @file UART.h
 *  @brief Header-file for the UART driver - to initialize, recieve and transfer data.
 *  @author: Anastasia Lindbäck and Marie Skatvedt
 */
#ifndef UART_H
#define UART_H

#include <stdint.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include <stdio.h>

#define FOSC 4915200 

/** Function for initializing UART.
 *  @param unsigned int ubrr - UBRR register, User Baud Rate Register
 */
void UART_init (unsigned int ubrr);

/** Function for recieving data from UART.
 *  @return UDR0 - Empty Handler, if register RXC0 (RX complete) is not set.
 */
unsigned char UART_recv (void);

/** Function for transferring data from UART.
 *  @param unsigned char letter - Letter to be transferred.
 */
void UART_trans (unsigned char letter);

#endif
