/** @file oled.c
 *  @brief OLED H-file for displaying on OLED-screen of multiboard.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef OLED_H
#define OLED_H

#define OLED_LINES 8
#define OLED_COLS 128

#define FONT_OFFSET 32

#define FONT_SIZE 8


#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <util/delay.h>
#include <avr/pgmspace.h>

#include "addresses.h"
#include "fonts.h"

// Global unsigned ints for keeping track of current line and column
uint8_t current_line;
uint8_t current_column;

/** Initialization routine, setting up OLED display
 */
void OLED_init(void);

/**Function for writing data to OLED.
 * @param unsigned char character - Character to be displayed on OLED. Defined in fonts.h.
 */
void OLED_write(unsigned char character);

/**Function for writing highlighted data to OLED.
 * @param unsigned char character - Character to be displayed on OLED. Defined in fonts.h.
 */
void OLED_highlight(unsigned char character);

/**Function for printing data to OLED.
 * @param char* data - Data to be printed on the display.
 */
void OLED_print(char* data, ...);

/**Function for printing highlighted data to OLED.
 * @param char* data - Data to be printed on the display.
 */
void OLED_print_highlight(char* data, ...);

/**Function for controlling the OLEDs registers.
 * @param uint8_t cmnd - Address for register in OLED.
 */
void OLED_command(uint8_t cmnd);

/**Function for switching lines (0-7) on OLED.
 * @param uint8_t line - Which line in OLED-matrix to go to.
 */
void OLED_go_to_line(uint8_t line);

/**Function for switching columns (0-127) on OLED.
 * @param uint8_t column - Which column in OLED-matrix to go to.
 */
void OLED_go_to_column(uint8_t column);

/**Function for switching position (line, column) on OLED.
 * @param uint8_t line - Which line in OLED-matrix to go to.
 * @param uint8_t column - Which column in OLED-matrix to go to.
 */
void OLED_position(uint8_t line, uint8_t column);

/**Function for clearing chosen line (0-7) on OLED.
 * @param int line - Which line to clear.
 */
void OLED_clear_line(uint8_t line);

/**Function for resetting OLED, clearing all lines on OLED (clear display)
 */
void OLED_reset(void);

/**Function for returning OLED initial position in display matrix.
 */
void OLED_home(void);

#endif
