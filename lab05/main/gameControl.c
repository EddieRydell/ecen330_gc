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

// stats constants
uint32_t num_player_missiles;
uint32_t num_impacts;

// All missiles
missile_t missiles[CONFIG_MAX_TOTAL_MISSILES];

// Alias into missiles array
missile_t *enemy_missiles = missiles+0;
missile_t *player_missiles = missiles+CONFIG_MAX_ENEMY_MISSILES;
missile_t *plane_missile = missiles+CONFIG_MAX_ENEMY_MISSILES+
                           CONFIG_MAX_PLAYER_MISSILES;

// Initialize the game control logic.
// This function initializes all missiles, planes, stats, etc.
void gameControl_init(void)
{
    // Initialize missiles
    for (uint32_t i = 0; i < CONFIG_MAX_ENEMY_MISSILES; i++)
        missile_init_enemy(enemy_missiles+i);
    for (uint32_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++)
        missile_init_idle(player_missiles+i);

    // initialize plane
    missile_init_idle(plane_missile);
    plane_init(plane_missile);

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

    // check for button press. If so, launch a free player missile.
    static bool pressed = false;
    coord_t x, y;
    uint64_t btns;
    btns = ~pin_get_in_reg() & BTN_MASK;
    // if a button is pressed this tick and no buttons were pressed last tick, launch a missile
    if (!pressed && btns) {
        pressed = true; // button pressed
        cursor_get_pos(&x, &y);
        // look for an idle player missile and launch it to the cursor's position
        // if there are no idle player missiles, no missile will be launched
        for (uint32_t i = 0; i < CONFIG_MAX_PLAYER_MISSILES; i++) {
            if (missile_is_idle(player_missiles + i)) {
                missile_init_player(player_missiles + i, x, y);
                break;
            }
        }
    }
    else if (pressed && !btns) {
        pressed = false; // all released
    }

    // Check for moving non-player missile collision with an explosion.
    for (uint32_t i = 0; i < CONFIG_MAX_TOTAL_MISSILES; i++) {
        // if the missile is moving (and it isn't a player missile), figure out if it's colliding with any player missile explosions
        if (missile_is_moving(missiles + i) && (missiles + i)->type != MISSILE_TYPE_PLAYER) {
            // loop through all player missiles and check to see if we're inside their explosion radius
            for (uint32_t j = 0; j < CONFIG_MAX_PLAYER_MISSILES; j++) {
                if (missile_is_colliding(player_missiles + j, (missiles + i)->x_current, (missiles + i)->y_current)) {
                    missile_explode(missiles + i);
                }
            }
        }
    }

    // M3: Count non-player impacted missiles

    // M3: Tick plane & draw stats

    // M3: Check for flying plane collision with an explosion.
}
