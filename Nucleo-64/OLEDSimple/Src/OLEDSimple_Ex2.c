#include <stdint.h>
#include "stm32f0xx.h"
#include "STM32_util.h"
#include "DEVICES.h"


long i;
volatile long Sys_Ticks, millisec, CountDown_ms;


enum PORT_CONNECTION
{
	USER_LED_PIN = 5,
	RED_PB,
	BLACK_PB,
	YELLOW_PB,
	WHITE_PB,
	BLUE_PB,
	BLUE_BUTTON_PIN = 13
} __attribute__ ((packed));


void Draw_MySegment (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);
void Draw_Rect_OLED (uint8_t, uint8_t, uint8_t, uint8_t, uint8_t);


int main(void) {
	Init_Sys_Tick ();
	Init_OLED (0);
	Clear_OLED ();

	Init_Pin ('A', RED_PB, INPUT_PULLUP);
	Init_Pin ('A', WHITE_PB, INPUT_PULLUP);

	while (1)
	{
		if (DigRead ('A', RED_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', RED_PB) == 0);
			Draw_Rect_OLED (2, 10, 6, 80, 7);
		}

		else if (DigRead ('A', WHITE_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', WHITE_PB) == 0);
			Clear_OLED ();
		}
	}
}

void
Draw_Rect_OLED (uint8_t A1, uint8_t B1, uint8_t A2, uint8_t B2, uint8_t W)
{
	uint8_t top_W, bottom_W;

	/* Ending page is +1 due to the way Draw_MySegment loops through pages */
	/* 	i.e., drawing from A1 to A1+1 only actual draws in page A1		   */

	/* Top */
	top_W = (uint8_t)0xFF >> (8 - W); // Use top W bits of the page
	Draw_MySegment (A1, A1+1, B1, B2, top_W);

	/* Bottom */
	bottom_W = (uint8_t)0xFF << (8 - W); // Use the bottom W bits of the page
	Draw_MySegment (A2, A2+1, B1, B2, bottom_W);

	/* End at column B1+W to make sure segment is W pixels wide */
	/* Left */
	Draw_MySegment (A1, A2+1, B1, B1+W, 0xFF);

	/* Start at column B2-W to make sure segment is W pixels wide */
	/* Right */
	Draw_MySegment (A1, A2+1, B2-W, B2, 0xFF);
}


void
Draw_MySegment (uint8_t row_i, uint8_t row_f, uint8_t col_i, uint8_t col_f, uint8_t thickness)
{
	uint8_t i, j, width = col_f - col_i;

	/* For pages row_i to row_f-1, draw (col_f-col_i) segments */
	/* 	that are `thickness` pixels thick					   */
	for (j = row_i; j < row_f; j++)
	{
		Set_Cursor_Pos (col_i, j);
		for (i = 0; i < width; i++)
		{
			Draw_segm_OLED (thickness);
		}
	}
}

