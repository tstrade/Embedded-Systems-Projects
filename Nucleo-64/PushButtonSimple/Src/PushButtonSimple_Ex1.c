//   Toggles user led on blue button press

#include <stdint.h>
#include "stm32f0xx.h"
#include "ELT_stm32_util_Lab2.h"

// User #defines
#define USER_LED_PIN 5
#define BLUE_BUTTON_PIN 13

void MyDelay(int);

int main(void)
{
  	Init_Pin('A', 5, OUTPUT);
	Init_Pin('C', 13, INPUT);

	DigWrite('A', USER_LED_PIN, HIGH);

	/* Loop forever */
	for (;;) {
		if (DigRead('C', BLUE_BUTTON_PIN) == 0) {
			while (DigRead('C', BLUE_BUTTON_PIN) == 0);

			for (int i = 0; i < 3; i++)
			{
				DigWrite('A', USER_LED_PIN, LOW);
				MyDelay(500);
				DigWrite('A', USER_LED_PIN, HIGH);
				MyDelay(500);
			}
		}

	}


}

void MyDelay(int d)
{
	switch(d)
	{
	case 100:
		d = 60000;
		break;
	case 500:
		d = 300000;
		break;
	case 1000:
		d = 600000;
		break;
	default:
		return;
	}

	int i;
	for (i = 0; i < d; i++);
}
