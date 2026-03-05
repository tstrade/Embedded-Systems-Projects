#include <stdint.h>
#include "stm32f0xx.h"
void DigWrite(char port, int pin, int state)
{
	switch (port) {
	case 'a':
	case 'A':
		if (state == 0)
			GPIOA->ODR = (GPIOA->ODR & ~(1 << pin));
		else
			GPIOA->ODR = (GPIOA->ODR | (1 << pin));
		break;
	case 'b':
	case 'B':
		if (state == 0)
			GPIOB->ODR = (GPIOB->ODR & ~(1 << pin));
		else
			GPIOB->ODR = (GPIOB->ODR | (1 << pin));
		break;
	case 'c':
	case 'C':
		if (state == 0)
			GPIOC->ODR = (GPIOC->ODR & ~(1 << pin));
		else
			GPIOC->ODR = (GPIOC->ODR | (1 << pin));
		break;
	case 'd':
	case 'D':
		if (state == 0)
			GPIOD->ODR = (GPIOD->ODR & ~(1 << pin));
		else
			GPIOD->ODR = (GPIOD->ODR | (1 << pin));
		break;
	case 'f':
	case 'F':
		if (state == 0)
			GPIOF->ODR = (GPIOF->ODR & ~(1 << pin));
		else
			GPIOF->ODR = (GPIOF->ODR | (1 << pin));
		break;
	}
	return;
}
void Init_Pin(char port, int pin, int direction) {
	int dirtemp;

	//direction = 0 for dig input, 1 for output, 2 for alternate, and 3 for floating, 4 is in w/pullup
	if ((direction >= 0) & (direction <= 4)) {
		dirtemp = direction % 4; //forces input w/pullup to be input first
		switch (port) {
		case 'a':
		case 'A':
			RCC->AHBENR |= 0x00020000;  //enable clock for Port A
			GPIOA->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOA->PUPDR |= (1 << (2 * pin)); //pull up enable
			break;
		case 'b':
		case 'B':
			RCC->AHBENR |= 0x00040000;  //enable clock for Port B
			GPIOB->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOB->PUPDR |= (1 << (2 * pin)); //pull up enable
			break;
		case 'c':
		case 'C':
			RCC->AHBENR |= 0x00080000;  //enable clock for Port C
			GPIOC->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOC->PUPDR |= (1 << (2 * pin)); //pull up enable
			break;
		case 'd':
		case 'D':
			RCC->AHBENR |= 0x00100000;  //enable clock for Port D
			GPIOD->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOD->PUPDR |= (1 << (2 * pin)); //pull up enable
			break;
		}
	}

}

int DigRead(char port, int pin) //Gen.Purpose input
{
	if (port == 'a' || port == 'A') {
		if (GPIOA->IDR & (1 << pin))
			return (1);
		else
			return (0);
	}
	if (port == 'b' || port == 'B') {
		if (GPIOB->IDR & (1 << pin))
			return (1);
		else
			return (0);
	}
	if (port == 'c' || port == 'C') {
		if (GPIOC->IDR & (1 << pin))
			return (1);
		else
			return (0);
	}
	if (port == 'd' || port == 'D') {
		if (GPIOD->IDR & (1 << pin))
			return (1);
		else
			return (0);
	}
	return (-1);
}
