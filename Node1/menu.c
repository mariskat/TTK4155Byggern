/** @file menu.c
 *  @brief C-file for the menu on the OLED - to move around in the menu.
 *  @authors: Anastasia Lindbäck and Marie Skatvedt
 */

#include "menu.h"

// Flag (0-1) set to 1 when a new game is indicated, and 0 when a game is ended.
int PLAY_GAME_FLAG = 0;

// Flag (0-2) indicating difficulty of game.
int DIFFICULTY_FLAG = 0;

/** Function for creating new submenu and initialising it.
 *  @param char* menu_title - Title of the menu being created
 *  @param menu* parent_menu - Pointer to struct menu that should be current menus' parent
 *  @param menu* child_menu - Pointer to struct menu that should be current menus' child
 *  @param menu* left_sibling_menu - Pointer to struct menu that should be current menus' left sibling
 *  @param menu* right_sibling_menu - Pointer to struct menu that should be current menus' right sibling
 *  @return new_menu - New menu struct with specified child, parent and siblings
 */
menu* menu_new(char* menu_title, menu* parent_menu, menu* child_menu, menu* left_sibling_menu, menu* right_sibling_menu){
    menu* new_menu = malloc(sizeof(menu));

    new_menu->title = menu_title;
    new_menu->parent = parent_menu;
    new_menu->child = child_menu;
    new_menu->left_sibling = left_sibling_menu;
    new_menu->right_sibling = right_sibling_menu;

    return new_menu;
}

/** Function for printing a submenu.
 *  @param menu* parent_menu - pointer to struct that is current menus' parent.
 *  @param menu* current_menu - pointer to curren menu struct.
 */
void menu_print_submenu(menu* parent_menu, menu* current_menu){
    OLED_reset();
    OLED_home();

    if (parent_menu->title == "PLAY GAME") {
        OLED_print("  PLAYING GAME");

    } else {
        OLED_print("   ");
        OLED_print(parent_menu->title);
    }

    OLED_go_to_line(current_line + 2);

    menu* child_menu = parent_menu->child;

    while (child_menu != NULL){
        if (child_menu == current_menu) {
            OLED_print_highlight("o ", 5);
            OLED_print_highlight(child_menu->title, 5);
            OLED_go_to_line(current_line + 1);
        }

        else {
            OLED_print("  ");
            OLED_print(child_menu->title);
            OLED_go_to_line(current_line + 1);
        }
        child_menu = child_menu->right_sibling;
    }
}

/** Function for printing GAME OVER when game is ended. Toggling data.
 */
void menu_print_game_over(void){
    for (int i = 0; i < 10; i++) {
        OLED_position(4, 25);
        OLED_print("GAME OVER");
        _delay_ms(700);
        OLED_clear_line(4);
        _delay_ms(1000);
    }
}



/** Function for navigating the menu by moving between siblings and parent/child and setting flags when indicated by joystick button press.
 *  @param menu* child_menu - Pointer to struct menu that corresponds to current menus' child
 *  @param direction dir - direction enum corresponding to joystick movement.
 *  @return current_menu - Menu struct that is chosen by joystick movement
 */
menu* menu_navigate(menu* child_menu, direction dir){
    menu* current_menu = child_menu;

    if (dir == NEUTRAL) {
        current_menu = child_menu;
    }

    else if (dir == UP) {
        if (current_menu->left_sibling != NULL) {
            current_menu = (current_menu->left_sibling);
        }
    }

    else if (dir == DOWN) {
        if (current_menu->right_sibling != NULL) {
            current_menu = (current_menu->right_sibling);
        }
    }

    // If direction is left and the game is not playing
    else if ((dir == LEFT) && (PLAY_GAME_FLAG != 1)) {
        menu* parent = current_menu->parent;
        if ((parent != NULL) && (parent->title != "MAIN MENU")){
            current_menu = (current_menu->parent);
        }
    }

    else {
        return current_menu;
    }

    // Navigate through abstraction layers by pressing joystick button
    if (!(joystick_button_not_pressed())) {
        if (current_menu->title == "PLAY GAME"){
            current_menu = current_menu->child;
            PLAY_GAME_FLAG = 1;

        } else if (current_menu->title == "END GAME"){
            current_menu = (current_menu->child);

        } else if (current_menu->title == "GAME OVER"){
            menu_print_game_over();
            _delay_ms(5000);
            // Return to main menu
            while (current_menu->title != "PLAY GAME"){
                current_menu = (current_menu->parent);
            }
            PLAY_GAME_FLAG = 0;
            DIFFICULTY_FLAG = 0;

        } else if (current_menu->title == "EASY"){
            // Return to main menu
            while (current_menu->title != "GAME SETTINGS"){
                current_menu = (current_menu->parent);
            }
            DIFFICULTY_FLAG = 0;

        } else if (current_menu->title == "MEDIUM"){
            // Return to main menu
            while (current_menu->title != "GAME SETTINGS"){
                current_menu = (current_menu->parent);
            }
            DIFFICULTY_FLAG = 1;
        } else if (current_menu->title == "HARD"){
            // Return to main menu
            while (current_menu->title != "GAME SETTINGS"){
                current_menu = (current_menu->parent);
            }
            DIFFICULTY_FLAG = 2;
        } else if (current_menu->child != NULL){

            current_menu = current_menu->child;
        }
    }
    return current_menu;
}

/** Function for creating main menu and submenus and setting their member variables.
 *  @return main_menu - Menu struct corresponding to main menu.
 */
menu* menu_init() {
    /* MAIN MENU */
    // Creating main menu, initializing its parent, child and siblings to NULL
    menu* main_menu = menu_new("MAIN MENU", NULL, NULL, NULL, NULL);

    /* SUBMENUS */
    // Submenu of main menu: setting left siblings and parent, right siblings and children must be set after initialization
    menu* play_game = menu_new("PLAY GAME", main_menu, NULL, NULL, NULL);
    menu* game_settings = menu_new("GAME SETTINGS", main_menu, NULL, play_game, NULL);
    menu* highscores = menu_new("HIGHSCORES", main_menu, NULL, game_settings, NULL);

    // GAME SETTINGS
    // Submenus of game settings
    menu* play_mode = menu_new("DIFFICULTY", game_settings, NULL, NULL, NULL);
    menu* players = menu_new("PLAYER MODE", game_settings, NULL, play_mode, NULL);

    // Submenus of players
    menu* single_player = menu_new("SINGLE PLAYER", players, NULL, NULL, NULL);
    menu* multi_player = menu_new("MULTIPLAYER", players, NULL, single_player, NULL);

    // Submenu for difficulty
    menu* easy = menu_new("EASY", play_mode, NULL, NULL, NULL);
    menu* medium = menu_new("MEDIUM", play_mode, NULL, easy, NULL);
    menu* hard = menu_new("HARD", play_mode, NULL, medium, NULL);

    // PLAY GAME
    // Submenu of play game
    menu* end_game = menu_new("END GAME", play_game, NULL, NULL, NULL);

    // Subsubmenu of play game
    menu* game_over = menu_new("GAME OVER", end_game, NULL, NULL, NULL);

    // HIGH SCORE
    menu* high_score_submenu = menu_new(" ", highscores, NULL, NULL, NULL);

    /* SETTING THE CHILD VARIABLES*/
    // Setting child of main menu
    main_menu->child = play_game;

    // Setting child of play game
    play_game->child = end_game;

    // Setting child of end game
    end_game->child = game_over;

    // Setting child of game settings
    game_settings->child = play_mode;

    // Setting child of players
    players->child = single_player;

    // Setting child of play_mode
    play_mode->child = easy;

    // Setting child of high score
    highscores->child = high_score_submenu;

    /* SETTING RIGHT SIBLINGS*/
    // Main menu right siblings
    play_game->right_sibling = game_settings;
    game_settings->right_sibling = highscores;
    highscores->right_sibling = NULL;

    // Settings submenu
    play_mode->right_sibling = players;
    players->right_sibling = NULL;

    // Settings subsubmenu
    single_player->right_sibling = multi_player;
    multi_player->right_sibling = NULL;

    easy->right_sibling = medium;
    medium->right_sibling = hard;
    hard->right_sibling = NULL;

    // Play game submenu
    end_game->right_sibling = NULL;

    // Play game subsubmenu
    game_over->right_sibling = NULL;

    // High score submenu
    high_score_submenu->right_sibling = NULL;

    return main_menu;
}
