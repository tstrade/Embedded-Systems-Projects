//   Lab 1

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32_util_2.h"
//******************************************
// User #defines
#define USER_LED_PIN 5

//******************************************
// User global variables

void MyDelay(int);

int main(void)
{
    /*Setup, one time */
//	Init_Sys_Tick();
	Init_Pin('A', USER_LED_PIN, OUTPUT);  //makes PA5 (user LED) an output
	/* Loop forever */
	for(;;)  /* Loop forever */
	{
		DigWrite('A', USER_LED_PIN, HIGH);
		MyDelay(500);
		DigWrite('A', USER_LED_PIN, LOW);
		MyDelay(500);
	}
}  // end of the main


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
