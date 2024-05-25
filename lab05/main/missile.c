#include "missile.h"
#include "config.h"

#include "esp_random.h"

#include <stdlib.h>
#include <math.h>

typedef enum {INIT, MOVING, EXPLODING_GROWING, EXPLODING_SHRINKING, IMPACTED, IDLE} missileState;

void missile_init_idle(missile_t* missile) {
    missile->currentState = IDLE;
}

void missile_init_player(missile_t* missile, coord_t x_dest, coord_t y_dest) {
    missile->type = MISSILE_TYPE_PLAYER;
    missile->currentState = INIT;

    missile->y_origin = LCD_H;
    missile->x_origin = x_dest;
    missile->y_dest = y_dest;
    missile->x_dest = x_dest;
    missile->x_current = missile->x_origin;
    missile->y_current = missile->y_origin;

    missile->total_length = sqrtf(
            (float)((missile->y_dest - missile->y_origin) * (missile->y_dest - missile->y_origin)) +
            (float)((missile->x_dest - missile->x_origin) * (missile->x_dest - missile->x_origin)));

    missile->length = sqrtf(
            (float)((missile->y_current - missile->y_origin) * (missile->y_current - missile->y_origin)) +
            (float)((missile->x_current - missile->x_origin) * (missile->x_current - missile->x_origin)));

    missile->explode_me = false;
    missile->radius = 0;
}

void missile_init_enemy(missile_t* missile) {
    missile->type = MISSILE_TYPE_ENEMY;
    missile->currentState = INIT;

    missile->y_origin = 0;
    missile->x_origin = (coord_t)(esp_random() % LCD_W);
    missile->y_dest = LCD_H;
    missile->x_dest = (coord_t)(esp_random() % LCD_W);
    missile->x_current = missile->x_origin;
    missile->y_current = 0;

    missile->total_length = sqrtf(
            (float)((missile->y_dest - missile->y_origin) * (missile->y_dest - missile->y_origin)) +
            (float)((missile->x_dest - missile->x_origin) * (missile->x_dest - missile->x_origin)));

    missile->length = sqrtf(
            (float)((missile->y_current - missile->y_origin) * (missile->y_current - missile->y_origin)) +
            (float)((missile->x_current - missile->x_origin) * (missile->x_current - missile->x_origin)));

    missile->explode_me = false;
    missile->radius = 0;
}

void missile_init_plane(missile_t* missile, coord_t x_orig, coord_t y_orig) {
    missile->type = MISSILE_TYPE_PLANE;
    missile->currentState = INIT;

    missile->y_origin = y_orig;
    missile->x_origin = x_orig;
    missile->y_dest = LCD_H;
    missile->x_dest = (coord_t)(esp_random() % LCD_W);
    missile->x_current = missile->x_origin;
    missile->y_current = missile->y_origin;

    missile->total_length = sqrtf(
            (float)((missile->y_dest - missile->y_origin) * (missile->y_dest - missile->y_origin)) +
            (float)((missile->x_dest - missile->x_origin) * (missile->x_dest - missile->x_origin)));

    missile->length = sqrtf(
            (float)((missile->y_current - missile->y_origin) * (missile->y_current - missile->y_origin)) +
            (float)((missile->x_current - missile->x_origin) * (missile->x_current - missile->x_origin)));

    missile->explode_me = false;
    missile->radius = 0;
}


void missile_explode(missile_t* missile) {
    missile->explode_me = true;
}

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
    }

    // state transition logic
    switch (missile->currentState) {
        case INIT:
            missile->currentState = MOVING;
            break;
        case MOVING:
            if (missile->explode_me) {
                missile->currentState = EXPLODING_GROWING;
            }
            else if (missile->length >= missile->total_length) {
                missile->currentState = IMPACTED;
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
        case MOVING: {
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
            break;
        case IDLE:
            break;
    }
}


void missile_get_pos(missile_t* missile, coord_t* x, coord_t* y) {
    *x = missile->x_current;
    *y = missile->y_current;
}

missile_type_t missile_get_type(missile_t* missile) {
    return missile->type;
}

bool missile_is_moving(missile_t* missile) {
    return missile->currentState == MOVING;
}

bool missile_is_exploding(missile_t* missile) {
    return missile->currentState == EXPLODING_GROWING || missile->currentState == EXPLODING_SHRINKING;
}

bool missile_is_idle(missile_t* missile) {
    return missile->currentState == IDLE;
}

bool missile_is_impacted(missile_t* missile) {
    return missile->currentState == IMPACTED;
}

bool missile_is_colliding(missile_t* missile, coord_t x, coord_t y) {
    return 0; // todo
}
