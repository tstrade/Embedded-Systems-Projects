//   Lab 1

#include <stdint.h>
#include "stm32f0xx.h"
#include "stm32_util_2.h"
//******************************************
// User #defines
#define USER_LED_PIN 5

//******************************************
// User global variables


int main(void)
{
    /*Setup, one time */
//	Init_Sys_Tick();
	Init_Pin('A', USER_LED_PIN, OUTPUT);  //makes PA5 (user LED) an output
	uint32_t i;
	/* Loop forever */
	for(;;)  /* Loop forever */
	{
		DigWrite('A', USER_LED_PIN, HIGH);
		for(i = 0; i < 60000; i++);
		DigWrite('A', USER_LED_PIN, LOW);
		for(i = 0; i < 60000; i++);
	}
}  // end of the main

