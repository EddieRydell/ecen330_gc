
#include <stdlib.h> // abs

#include "my_lcd.h"
#include "joy.h"
#include "cursor.h"

#define SEN_DEFAULT (1.25f*LCD_W) // 1.25x screen width pixels per second
#define THRESH_DEFAULT 150 // raw ADC values
#define CLIP(x,lo,hi) ((x) < (lo) ? (lo) : ((x) > (hi) ? (hi) : (x)))

static uint32_t uperiod; // Update period in milliseconds.
static float sfactor; // Joystick sensitivity factor.
static uint32_t thresh; // Joystick displacement threshold.
static float xpos, ypos; // Current cursor position as a float.


// Initialize the cursor. Must be called before use.
// per: Specify the period in milliseconds that the cursor
// position is updated with a call to cursor_tick().
// Return zero if successful, or non-zero otherwise.
int32_t cursor_init(uint32_t per)
{
	if (per == 0 || joy_init()) return -1;
	uperiod = per; // Save period parameter
	// Set defaults
	cursor_set_sensitivity(SEN_DEFAULT);
	cursor_set_threshold(THRESH_DEFAULT);
	// Initialize the cursor position to the center of the screen.
	xpos = LCD_W/2;
	ypos = LCD_H/2;
	return 0;
}

// Update the cursor position based on the joystick displacement.
// This function is not safe to call from an ISR context.
// Therefore, it must be called from a software task context.
void cursor_tick(void)
{
	int32_t dcx, dcy;

	// Get joystick position relative to the center in raw ADC values.
	joy_get_displacement(&dcx, &dcy);
	if (abs(dcx) < thresh && abs(dcy) < thresh) return;

	// Based on the joystick position relative to center,
	// calculate a new position for the cursor.
	xpos += dcx*sfactor;
	ypos += dcy*sfactor;

	// Clip new position to screen.
	xpos = CLIP(xpos, 0, LCD_W-1);
	ypos = CLIP(ypos, 0, LCD_H-1);
}

// Set the sensitivity of the cursor to joystick movement.
// Sensitivity is in units of screen pixels/sec at full joystick displacement.
// The default is 1.25x screen width pixels per second.
// sens: joystick movement sensitivity in screen pixels/sec.
void cursor_set_sensitivity(float sens)
{
	sfactor = ((sens < 1.0f) ? 1.0f : sens) / JOY_MAX_DISP * uperiod / 1000;
}

// Set the threshold of joystick displacement needed before moving the cursor.
// Threshold is specified as a factor (0 to 1) of maximum displacement.
// If this value is too low, the cursor will drift when the joystick is untouched.
// The default is a displacement factor of 0.075 from center position.
// thr: threshold factor (0 to 1)
void cursor_set_threshold(float thr)
{
	thresh = thr;
}

// Get the cursor position in screen coordinates.
// Coordinate values range from 0 to lcd maximum width and height minus 1.
// *x: pointer to x coordinate.
// *y: pointer to y coordinate.
void cursor_get_pos(int32_t *x, int32_t *y)
{
	*x = xpos+0.5f;
	*y = ypos+0.5f;
}
