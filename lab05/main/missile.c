#include "missile.h"
#include "config.h"

#include "esp_random.h"

#include <stdlib.h>
#include <math.h>

typedef enum {INIT, MOVING, EXPLODING_GROWING, EXPLODING_SHRINKING, IMPACTED, IDLE} missileState;
#define NUM_FIRING_LOCATIONS 3
#define CURSOR_EDGE_CORRECTION 3 // value to add to player missile x_dest so it doesn't fire from offscreen

// helper function to initialize default fields in missile
// (assumes that x_origin, y_origin, x_dest, and y_dest have already been set)
void missile_generic_init_helper(missile_t* missile) {
    missile->length = 0;
    missile->explode_me = false;
    missile->radius = 0;
    missile->x_current = missile->x_origin;
    missile->y_current = missile->y_origin;
    missile->total_length = hypotf((float)(missile->y_dest - missile->y_origin), (float)(missile->x_dest - missile->x_origin));
}

// Initialize the missile as an idle missile. If initialized to the idle
// state, a missile doesn't appear nor does it move.
void missile_init_idle(missile_t* missile) {
    missile->currentState = IDLE;
}

// Initialize the missile as a player missile. This function takes an (x, y)
// destination of the missile (as specified by the user). The origin is the
// closest "firing location" to the destination (there are three firing
// locations evenly spaced along the bottom of the screen).
void missile_init_player(missile_t* missile, coord_t x_dest, coord_t y_dest) {
    missile->type = MISSILE_TYPE_PLAYER;
    missile->currentState = INIT;

    int32_t segment_width = LCD_W / NUM_FIRING_LOCATIONS;
    int32_t offset = segment_width / 2;

    // make sure the missile won't fire from offscreen,
    // then have it quantize its x_origin to one of the firing locations
    if (x_dest <= 0) x_dest = 1;
    else if (x_dest >= LCD_W - CURSOR_EDGE_CORRECTION) x_dest = LCD_W - CURSOR_EDGE_CORRECTION;
    missile->x_origin = (coord_t)roundf((float)(x_dest - offset) / (float)segment_width) * segment_width + offset;
    missile->y_origin = LCD_H;
    missile->y_dest = y_dest;
    missile->x_dest = x_dest;

    missile_generic_init_helper(missile);
}

// Initialize the missile as an enemy missile. This will randomly choose the
// origin and destination of the missile. The origin is somewhere near the
// top of the screen, and the destination is the very bottom of the screen.
void missile_init_enemy(missile_t* missile) {
    missile->type = MISSILE_TYPE_ENEMY;
    missile->currentState = INIT;

    missile->y_origin = 0;
    missile->x_origin = (coord_t)(esp_random() % LCD_W);
    missile->y_dest = LCD_H;
    missile->x_dest = (coord_t)(esp_random() % LCD_W);

    missile_generic_init_helper(missile);
}

// Initialize the missile as a plane missile. This function takes the (x, y)
// location of the plane as an argument and uses it as the missile origin.
// The destination is randomly chosen along the bottom of the screen.
void missile_init_plane(missile_t* missile, coord_t x_orig, coord_t y_orig) {
    missile->type = MISSILE_TYPE_PLANE;
    missile->currentState = INIT;

    missile->y_origin = y_orig;
    missile->x_origin = x_orig;
    missile->y_dest = LCD_H;
    missile->x_dest = (coord_t)(esp_random() % LCD_W);

    missile_generic_init_helper(missile);
}

// Used to indicate that a moving missile should be detonated. This occurs
// when an enemy or a plane missile is located within an explosion zone.
void missile_explode(missile_t* missile) {
    missile->explode_me = true;
}

// Tick the state machine for a single missile.
void missile_tick(missile_t* missile) {
    // determine the color of the missile before drawing
    uint16_t color;
    switch (missile->type) {
        case MISSILE_TYPE_PLAYER:
            color = CONFIG_COLOR_PLAYER_MISSILE;
            break;
        case MISSILE_TYPE_ENEMY:
            color = CONFIG_COLOR_ENEMY_MISSILE;
            break;
        case MISSILE_TYPE_PLANE:
            color = CONFIG_COLOR_PLANE_MISSILE;
            break;
        default:
            color = CONFIG_COLOR_ENEMY_MISSILE;
            break;
    }

    // state transition logic
    switch (missile->currentState) {
        case INIT:
            missile->currentState = MOVING;
            break;
        case MOVING:
            // transition out of this state if:
            // 1. the missile needs to explode (EXPLODING_GROWING)
            // 2. the missile has traversed its entire path (IMPACTED or EXPLODING GROWING, depending on type)
            if (missile->explode_me) {
                missile->currentState = EXPLODING_GROWING;
            }
            else if (missile->length >= missile->total_length) {
                if (missile->type == MISSILE_TYPE_PLAYER) {
                    missile->currentState = EXPLODING_GROWING;
                }
                else missile->currentState = IMPACTED;
            }
            break;
        case EXPLODING_GROWING:
            if (missile->radius >= CONFIG_EXPLOSION_MAX_RADIUS) {
                missile->currentState = EXPLODING_SHRINKING;
            }
            break;
        case EXPLODING_SHRINKING:
            if (missile->radius <= 0) {
                missile->currentState = IDLE;
            }
            break;
        case IMPACTED:
            missile->currentState = IDLE;
            break;
        case IDLE:
            break;
    }

    // action logic
    switch (missile->currentState) {
        case INIT:
            break;
        case MOVING: { // create a new scope here so we can declare a variable without the compiler getting mad
            // update the missile's length to increase by a predetermined amount for each tick it's moving
            if (missile->type == MISSILE_TYPE_PLAYER) {
                missile->length += CONFIG_PLAYER_MISSILE_DISTANCE_PER_TICK;
            }
            else if (missile->type == MISSILE_TYPE_ENEMY) {
                missile->length += CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK;
            }
            else if (missile->type == MISSILE_TYPE_PLANE) {
                missile->length += CONFIG_ENEMY_MISSILE_DISTANCE_PER_TICK;
            }
            float fraction = missile->length / missile->total_length;
            missile->x_current =
                    missile->x_origin + (coord_t) (fraction * (float) (missile->x_dest - missile->x_origin));
            missile->y_current =
                    missile->y_origin + (coord_t) (fraction * (float) (missile->y_dest - missile->y_origin));
            lcdDrawLine(&dev, missile->x_origin, missile->y_origin, missile->x_current, missile->y_current, color);
            break;
        }
        case EXPLODING_GROWING:
            missile->radius += CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK;
            lcdFillCircle(&dev, missile->x_current, missile->y_current, (int32_t)missile->radius, color);
            break;
        case EXPLODING_SHRINKING:
            missile->radius -= CONFIG_EXPLOSION_RADIUS_CHANGE_PER_TICK;
            lcdFillCircle(&dev, missile->x_current, missile->y_current, (int32_t)missile->radius, color);
            break;
        case IMPACTED:
        case IDLE:
            break;
    }
}

// Return the current missile position through the pointers *x,*y.
void missile_get_pos(missile_t* missile, coord_t* x, coord_t* y) {
    *x = missile->x_current;
    *y = missile->y_current;
}

// Return the missile type.
missile_type_t missile_get_type(missile_t* missile) {
    return missile->type;
}

// Return whether the given missile is moving.
bool missile_is_moving(missile_t* missile) {
    return missile->currentState == MOVING;
}

// Return whether the given missile is exploding. If this missile
// is exploding, it can explode another intersecting missile.
bool missile_is_exploding(missile_t* missile) {
    return missile->currentState == EXPLODING_GROWING || missile->currentState == EXPLODING_SHRINKING;
}

// Return whether the given missile is idle.
bool missile_is_idle(missile_t* missile) {
    return missile->currentState == IDLE;
}

// Return whether the given missile is impacted.
bool missile_is_impacted(missile_t* missile) {
    return missile->currentState == IMPACTED;
}

// Return whether an object (e.g., missile or plane) at the specified
// (x,y) position is colliding with the given missile. For a collision
// to occur, the missile needs to be exploding and the specified
// position needs to be within the explosion radius.
bool missile_is_colliding(missile_t* missile, coord_t x, coord_t y) {
    float x_distance = (float)(missile->x_current - x);
    float y_distance = (float)(missile->y_current - y);
    float distance = hypotf(x_distance, y_distance);
    if (missile_is_exploding(missile) && distance < missile->radius) {
        return true;
    }
    return false;
}
