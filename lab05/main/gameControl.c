#include <stdio.h>
#include <stdlib.h> // rand

#include "config.h"
#include "lcd.h"
#include "cursor.h"
#include "pin.h"
#include "btn.h"
#include "missile.h"
#include "plane.h"
#include "gameControl.h"

// M3: Define stats constants

// All missiles
missile_t missiles[CONFIG_MAX_TOTAL_MISSILES];

// Alias into missiles array
missile_t *enemy_missiles = missiles+0;
missile_t *player_missiles = missiles+CONFIG_MAX_ENEMY_MISSILES;
missile_t *plane_missile = missiles+CONFIG_MAX_ENEMY_MISSILES+
                           CONFIG_MAX_PLAYER_MISSILES;

// M3: Declare stats variables


// Initialize the game control logic.
// This function initializes all missiles, planes, stats, etc.
void gameControl_init(void)
{
    // Initialize missiles
    for (uint32_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++)
        missile_init_enemy(enemy_missiles+i);
    for (uint32_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++)
        missile_init_idle(player_missiles+i);
    missile_init_idle(plane_missile);

    // M3: Initialize plane

    // M3: Initialize stats
}

// Update the game control logic.
// This function calls the missile & plane tick functions, reinitializes
// idle enemy missiles, handles button presses, fires player missiles,
// detects collisions, and updates statistics.
void gameControl_tick(void)
{
    // Tick missiles in one batch
    for (uint32_t i = 0; i < CONFIG_MAX_TOTAL_MISSILES; i++)
        missile_tick(missiles+i);

    // Reinitialize idle enemy missiles
    for (uint32_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++)
        if (missile_is_idle(enemy_missiles+i))
            missile_init_enemy(enemy_missiles+i);

    // M1: Reinitialize idle player missiles, !!! remove after Milestone 1 !!!
    /*for (uint32_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++)
        if (missile_is_idle(player_missiles+i))
            missile_init_player(player_missiles+i, rand()%LCD_W, rand()%LCD_H);*/ //todo remove completely

    // M2: Check for button press. If so, launch a free player missile.
    static bool pressed = false;
    coord_t x, y;
    uint64_t btns;
    btns = ~pin_get_in_reg() & BTN_MASK;
    if (!pressed && btns) {
        pressed = true; // button pressed
        cursor_get_pos(&x, &y);
        // TODO: Check to see if a player
        // missile is idle and launch it
        // to the target (x,y) position.
    } else if (pressed && !btns) {
        pressed = false; // all released
    }

    // M2: Check for moving non-player missile collision with an explosion.

    // M3: Count non-player impacted missiles

    // M3: Tick plane & draw stats

    // M3: Check for flying plane collision with an explosion.
}
