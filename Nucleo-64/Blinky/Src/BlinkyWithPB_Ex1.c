#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32_util.h"

#define USER_LED_PIN 5
#define red_PB 6
#define white_PB 7

int main(void)
{
  	Init_Pin('A', 5, OUTPUT);  //makes PA5 (user LED) an output
	Init_Pin('A', red_PB, INPUT_PULLUP);  //enables pull up on PC13 (Blue Button)
	Init_Pin('A', white_PB, INPUT_PULLUP);

	DigWrite('A', USER_LED_PIN, LOW);
	/* Loop forever */
	for (;;) {
		if (DigRead('A', red_PB) == 0) {
			while (DigRead('A', red_PB) == 0);
			DigWrite('A', USER_LED_PIN, HIGH);
		}

		if (DigRead('A', white_PB) == 0) {
			while (DigRead('A', white_PB) == 0);
			DigWrite('A', USER_LED_PIN, LOW);
		}

	}
}


