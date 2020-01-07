/** @package main.c
 *  main function for running the program - Controller. Node 1.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "ADC.h"
#include "addresses.h"
#include "CAN.h"
#include "joystick.h"
#include "menu.h"
#include "OLED.h"
#include "UART.h"
#include "slider.h"
#include "SPI.h"
#include "sram_test.h"

#include <util/delay.h>

/** Interrupt vector function for detecting unhandled interrupts.
 */
ISR(__vector_default) {
    printf("Interrupt\n\r");

}

void main() {

    sei();
    UART_init(9600);
    CAN_init();

    //set_bit(UCSR1A, UPE1);
    set_bit(MCUCR, SRE); // Sets the SRE (Static Ram Enable) bit in the MCUCR (MCU Control Register) - enabling external write
    set_bit(SFIOR, XMM2); // Setting XMM2 (External Memory High Mask) bit in the SFIOR (Special Function IO Register) - use 4 bits for external memory address


    /* USB MULTIBOARD INIT */
    _delay_ms(1000);
    joystick_calibrate();
    joystick position = joystick_position();
    Sliders slider = slider_position();

    /* MENU INIT */
    OLED_init();
    // Parent menu is Main Menu
    menu* parent_menu = menu_init();
    // Child menu is Play Game, which means that this is the first current menu from menu_navigate
    menu* child_menu = parent_menu->child;
    // Current menu is nothing
    menu* current_menu = NULL;
    // Direction of joystick
    direction dir = joystick_direction();


    while(1) {
        // The current menu is changed to the one menu navigate decides
        current_menu = menu_navigate(child_menu, dir);

        if (CAN_recent_message().data[1] != 1){
            if (current_menu->title != "GAME OVER") {
                // Print submenu of current menu
                menu_print_submenu(parent_menu, current_menu);
                _delay_ms(500);
                dir = joystick_direction();
                child_menu = current_menu;
                parent_menu = child_menu->parent;
            } else {
                OLED_reset();

                // Printing game over
                child_menu = current_menu;
                parent_menu = child_menu->parent;
            }
        } else {
            // Print game over
            OLED_reset();
            menu_print_game_over();
            _delay_ms(100);

            // Return to main menu
            while (current_menu->title != "PLAY GAME"){
                current_menu = (current_menu->parent);
            }

            child_menu = current_menu;
            parent_menu = child_menu->parent;

            PLAY_GAME_FLAG = 0;
            DIFFICULTY_FLAG = 0;
        }

        // USB MULTIFUNCTION BOARD
        position = joystick_position();
        slider = slider_position();

        CAN_transmit_game_controller(position, slider, PLAY_GAME_FLAG, DIFFICULTY_FLAG);

        _delay_ms(100);
    }
}
