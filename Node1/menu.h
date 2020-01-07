/** @file menu.c
 *  @brief H-file for the menu on the OLED - to move around in the menu.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#ifndef MENU_H
#define MENU_H


#include <stdint.h>
#include <stdio.h>
#include <stdarg.h>
#include <stdlib.h>
#include <util/delay.h>

#include "joystick.h"
#include "OLED.h"

// Global flag for playing the game
extern int PLAY_GAME_FLAG;

// Global flag for setting difficulty of the game
extern int DIFFICULTY_FLAG;

// Menu struct containing menu struct pointers to other submenus
typedef struct {
    char* title;

    struct menu* parent;
    struct menu* child;
    struct menu* left_sibling;
    struct menu* right_sibling;
} menu;


/** Function for creating new submenu and initialising it.
 *  @param char* menu_title - Title of the menu being created
 *  @param menu* parent_menu - Pointer to struct menu that should be current menus' parent
 *  @param menu* child_menu - Pointer to struct menu that should be current menus' child
 *  @param menu* left_sibling_menu - Pointer to struct menu that should be current menus' left sibling
 *  @param menu* right_sibling_menu - Pointer to struct menu that should be current menus' right sibling
 *  @return new_menu - New menu struct with specified child, parent and siblings
 */
menu* menu_new(char* menu_title, menu* parent_menu, menu* child_menu, menu* left_sibling_menu, menu* right_sibling_menu);

/** Function for printing a submenu.
 *  @param menu* parent_menu - pointer to struct that is current menus' parent.
 *  @param menu* current_menu - pointer to curren menu struct.
 */
void menu_print_submenu(menu* parent_menu, menu* current_menu);

/** Function for printing GAME OVER when game is ended. Toggling data.
 */
void menu_print_game_over(void);

/** Function for navigating the menu by moving between siblings and parent/child and setting flags when indicated by joystick button press.
 *  @param menu* child_menu - Pointer to struct menu that corresponds to current menus' child
 *  @param direction dir - direction enum corresponding to joystick movement.
 *  @return current_menu - Menu struct that is chosen by joystick movement
 */
menu* menu_navigate(menu* child_menu, direction dir);

/** Function for creating main menu and submenus and setting their member variables.
 *  @return main_menu - Menu struct corresponding to main menu.
 */
menu* menu_init();

#endif
