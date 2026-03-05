#include <DEVICES.h>
#include <stdint.h>
#include "stm32f0xx.h"

extern volatile long Sys_Ticks;
extern volatile long millisec;
extern volatile long CountDown_ms;

void Init_Sys_Tick(void) {
	SysTick->LOAD = 8000;
	SysTick->VAL = 0;
	SysTick->CTRL = 0x07;
}
void SysTick_Handler(void) {
	Sys_Ticks++;
	millisec++;
	if (CountDown_ms)
		CountDown_ms--;
}
void Delay_100us(int N) {
	int i = 0, j = 0;

	for (j = 0; j < N; j++) {
		for (i = 0; i < 65; i++)
			;
	}
}
void Delay_1ms(int N) {
	int i = 0, j = 0;

	for (j = 0; j < N; j++) {
		for (i = 0; i < 10; i++)
			Delay_100us(1);
	}
}
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

		if ((direction >= 0) & (direction <= 4)) {
		dirtemp = direction % 4;
		switch (port) {
		case 'a':
		case 'A':
			RCC->AHBENR |= 0x00020000;
			GPIOA->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOA->PUPDR |= (1 << (2 * pin));
			break;
		case 'b':
		case 'B':
			RCC->AHBENR |= 0x00040000;
			GPIOB->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOB->PUPDR |= (1 << (2 * pin));
			break;
		case 'c':
		case 'C':
			RCC->AHBENR |= 0x00080000;
			GPIOC->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOC->PUPDR |= (1 << (2 * pin));
			break;
		case 'd':
		case 'D':
			RCC->AHBENR |= 0x00100000;
			GPIOD->MODER |= (dirtemp << (2 * pin));
			if (direction == 4)
				GPIOD->PUPDR |= (1 << (2 * pin));
			break;
		}
	}
}

int DigRead(char port, int pin)
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

void Init_I2C2(int speed)
{ //
	RCC->APB1ENR |= 0x00400000;
	Init_Pin('B', 13, ALT);
	Init_Pin('B', 14, ALT);
	GPIOB->AFR[1] |= 0x05500000;
	GPIOB->OTYPER |= 0x00006000;

	if (speed == 400000)
		I2C2->TIMINGR = (uint32_t) 0x00310309;
	else
		I2C2->TIMINGR = (uint32_t) 0x10420F13;
	I2C2->CR1 = I2C_CR1_PE;
}

void I2C2WriteRegister(unsigned char Recv_addr, char Reg_addr, char Data) {

	I2C2->CR2 = I2C_CR2_AUTOEND | (2 << 16) | Recv_addr << 1;
	I2C2->CR2 |= I2C_CR2_START;

	while ((I2C2->ISR & I2C_ISR_TXIS) == 0)
		;
	I2C2->TXDR = Reg_addr;
	while ((I2C2->ISR & I2C_ISR_TXIS) == 0)
		;
	I2C2->TXDR = Data;
}

