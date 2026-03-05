#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32_util.h"

#define USER_LED_PIN 5
#define red_PB 6
#define black_PB 7
#define BLUE_BUTTON_PIN 13

int main(void)
{
  	Init_Pin('A', USER_LED_PIN, OUTPUT);  //makes PA5 (user LED) an output
	Init_Pin('A', red_PB, INPUT_PULLUP);   // Makes PA6 (red button) an input with pull up
	Init_Pin('A', black_PB, INPUT_PULLUP);  // Makes PA7 (black button) an input with pull up
	Init_Pin('C', BLUE_BUTTON_PIN, INPUT_PULLUP); //enables pull up on PC13 (Blue Button)

	DigWrite('A', USER_LED_PIN, LOW);
	int frequency = 0;
	/* Loop forever */
	for (;;) {
		if (DigRead('C', BLUE_BUTTON_PIN) == 0) {
			while (DigRead('C', BLUE_BUTTON_PIN) == 0);
			frequency = 160000; // 2 Hz
		}
		else if (DigRead('A', red_PB) == 0) {
			while (DigRead('A', red_PB) == 0);
			frequency = 300000; // 1 Hz
		}
		else if (DigRead('A', black_PB) == 0) {
			while (DigRead('A', black_PB) == 0);
			frequency = 600000; // 0.5 Hz
		}

		if (frequency != 0)
		{
			if (DigRead ('A', USER_LED_PIN) == LOW)
				DigWrite ('A', USER_LED_PIN, HIGH);
			else
				DigWrite ('A', USER_LED_PIN, LOW);

			for (int i = 0; i < frequency; i++);
		}
	}
}
