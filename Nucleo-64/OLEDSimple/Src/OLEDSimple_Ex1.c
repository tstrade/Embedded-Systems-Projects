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


int main(void) {
	Init_Sys_Tick ();
	Init_OLED (0);
	Clear_OLED ();

	Init_Pin ('A', RED_PB, INPUT_PULLUP);
	Init_Pin ('A', BLACK_PB, INPUT_PULLUP);
	Init_Pin ('A', YELLOW_PB, INPUT_PULLUP);
	Init_Pin ('A', WHITE_PB, INPUT_PULLUP);
	Init_Pin ('A', BLUE_PB, INPUT_PULLUP);

	Init_Pin ('C', BLUE_BUTTON_PIN, INPUT_PULLUP);

	const uint8_t d = 0xFF;

	while (1)
	{
		if (DigRead ('C', BLUE_BUTTON_PIN) == 0)
		{
			/* Press and release */
			while (DigRead ('C', BLUE_BUTTON_PIN) == 0);
			Clear_OLED ();
		}

		else if (DigRead ('A', RED_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', RED_PB) == 0);
			Draw_MySegment (14, 110, 1, 2, d);
		}

		else if (DigRead ('A', BLACK_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', BLACK_PB) == 0);
			Draw_MySegment (14, 110, 6, 7, d);
		}

		else if (DigRead ('A', YELLOW_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', YELLOW_PB) == 0);
			Draw_MySegment (14, 22, 1, 6, d);
		}

		else if (DigRead ('A', WHITE_PB) == 0)
		{
			/* Press and release */
			while (DigRead ('A', WHITE_PB) == 0);
			Draw_MySegment (102, 110, 1, 6, d);
		}

		else if (DigRead ('A', BLUE_PB) == 0)
		{
			/* While pressed... */
			while (DigRead ('A', BLUE_PB) == 0)
			{
				Draw_MySegment (14, 110, 1, 2, d);
				Draw_MySegment (14, 110, 6, 7, d);
				Draw_MySegment (14, 22, 1, 6, d);
				Draw_MySegment (102, 110, 1, 6, d);

				/* 2 kHz */
				Delay_1ms (250);
				Clear_OLED ();
				Delay_1ms (250);
			}
		}
	}
}


void
Draw_MySegment (uint8_t col_i, uint8_t col_f, uint8_t row_i, uint8_t row_f, uint8_t thickness)
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

