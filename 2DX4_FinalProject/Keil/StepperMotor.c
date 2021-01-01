// Contains code for stepper motor functions

#include "tm4c1294ncpdt.h"
#include <stdint.h>
#include "SysTick.h"
#include "StepperMotor.h"

void PortH_Init(void){
	SYSCTL_RCGCGPIO_R |= SYSCTL_RCGCGPIO_R7;
	while((SYSCTL_PRGPIO_R&SYSCTL_PRGPIO_R7) == 0){};	
	GPIO_PORTH_DIR_R |= 0xF;												
	GPIO_PORTH_AFSEL_R &= ~0xF;		 								
	GPIO_PORTH_DEN_R |= 0xF;																												
	GPIO_PORTH_AMSEL_R &= ~0xF;		 								
	return;
}

void DutyCycleForward(int delay) {
	GPIO_PORTH_DATA_R = 0b00001100;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00000110;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00000011;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00001001;
	SysTick_Wait(96000*delay);
}

void DutyCycleReverse(int delay) {
	GPIO_PORTH_DATA_R = 0b00001001;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00000011;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00000110;
	SysTick_Wait(96000*delay);
	GPIO_PORTH_DATA_R = 0b00001100;
	SysTick_Wait(96000*delay);
}

void rotate(int delay, int dir) {
	if(dir == 1) {
			DutyCycleForward(delay);
	} else if(dir == -1) {
			DutyCycleReverse(delay);
	}
}
