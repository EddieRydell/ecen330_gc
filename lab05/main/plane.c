#include "plane.h"

#include "lcd.h"
#include "config.h"

#include "esp_random.h"
#include "esp_log.h"

static enum {INIT, FLYING, IDLE} plane_state;
static bool plane_exploding;
static coord_t plane_x;
static coord_t plane_y;
static missile_t* plane_missile;
static uint32_t missile_launch_time;
static uint32_t plane_lifetime_ticks;

#define DEFAULT_PLANE_HEIGHT (LCD_H / 4)
#define PLANE_TICKS_TO_CROSS_SCREEN (LCD_W / CONFIG_PLANE_DISTANCE_PER_TICK)

static const char* TAG = "plane";

// helper function to draw the plane to the screen
void draw_self() {
    lcdFillTri(&dev,
               plane_x, DEFAULT_PLANE_HEIGHT + CONFIG_PLANE_HEIGHT / 2,
               plane_x, DEFAULT_PLANE_HEIGHT - CONFIG_PLANE_HEIGHT / 2,
               plane_x - CONFIG_PLANE_WIDTH, DEFAULT_PLANE_HEIGHT,
               CONFIG_COLOR_PLANE);
}

// Initialize the plane state machine. Pass a pointer to the missile
// that will be (re)launched by the plane. It will only have one missile.
void plane_init(missile_t* missile) {
    ESP_LOGI(TAG, "Plane initializing");
    plane_state = INIT;
    plane_exploding = false;
    plane_x = LCD_W + CONFIG_PLANE_WIDTH;
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
            plane_init(plane_missile);
            plane_state = FLYING;
            break;
        case FLYING:
            if (plane_exploding || plane_x <= 0) {
                plane_state = IDLE;
            }
            break;
        case IDLE:
            // once the timer runs out, go back to init
            if (idle_ticks >= CONFIG_PLANE_IDLE_TIME_TICKS) {
                idle_ticks = 0;
                plane_state = INIT;
            }
            break;
    }

    // plane state action logic
    switch (plane_state) {
        case INIT:
            break;
        case FLYING:
            draw_self();
            plane_lifetime_ticks++;
            // launch a missile from the plane if we hit the randomly predetermined launch time
            if (plane_lifetime_ticks == missile_launch_time) {
                missile_init_plane(plane_missile, plane_x, plane_y);
            }
            plane_x -= (coord_t)CONFIG_PLANE_DISTANCE_PER_TICK * 2;
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