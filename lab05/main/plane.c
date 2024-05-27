#include "plane.h"

#include "lcd.h"
#include "config.h"
#include "esp_random.h"

static enum {INIT, FLYING, IDLE} plane_state;
static bool plane_missile_fired;
static bool plane_exploding;
static coord_t plane_x;
static coord_t plane_y;
static missile_t* plane_missile;
static uint32_t missile_launch_time;
static uint32_t plane_lifetime_ticks;

#define DEFAULT_PLANE_HEIGHT (LCD_H / 4)
#define PLANE_TICKS_TO_CROSS_SCREEN (LCD_W / CONFIG_PLANE_DISTANCE_PER_TICK)

// Initialize the plane state machine. Pass a pointer to the missile
// that will be (re)launched by the plane. It will only have one missile.
void plane_init(missile_t* missile) {
    plane_missile_fired = false;
    plane_state = INIT;
    plane_exploding = false;
    plane_x = LCD_W;
    plane_y = DEFAULT_PLANE_HEIGHT;
    plane_missile = missile;
    missile_launch_time = esp_random() % (uint32_t)PLANE_TICKS_TO_CROSS_SCREEN;
    plane_lifetime_ticks = 0;
}


// Trigger the plane to explode.
void plane_explode(void) {
    plane_exploding = true;
}

// State machine tick function.
void plane_tick(void) {
    static uint32_t idle_ticks = 0;
    // plane state transition logic
    switch (plane_state) {
        case INIT:
            plane_state = FLYING;
            plane_init(plane_missile);
            break;
        case FLYING:
            if (plane_exploding || plane_x <= 0) {
                plane_state = IDLE;
            }
            break;
        case IDLE:
            if (idle_ticks == CONFIG_PLANE_IDLE_TIME_TICKS) {
                plane_state = FLYING;
            }
            break;
    }

    // plane state action logic
    switch (plane_state) {
        case INIT:
            break;
        case FLYING:
            plane_lifetime_ticks++;
            if (plane_lifetime_ticks == missile_launch_time) {
                missile_init_plane(plane_missile, plane_x, plane_y);
            }
            plane_x -= (coord_t)CONFIG_PLANE_DISTANCE_PER_TICK;
            break;
        case IDLE:
            idle_ticks++;
            break;
    }
}


// Return the current plane position through the pointers *x,*y.
void plane_get_pos(coord_t* x, coord_t* y) {
    *x = plane_x;
    *y = plane_y;
}

// Return whether the plane is flying.
bool plane_is_flying(void) {
    return plane_state == FLYING;
}