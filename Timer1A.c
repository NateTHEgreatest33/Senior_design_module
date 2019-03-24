#include "Timer1A.h"
//0x8 = green, 0x4 = blue, 0x2 red

void Timer1A_Handler(void)
{
	//error/warning handling
	if(Global_error == false || warningCounter >= 200){
		GPIO_PORTF_DATA_R = 0x2;//red
		while(1){}
	}
	if(Global_warning == false){
		Global_warning = true;
		warningCounter++;
		GPIO_PORTF_DATA_R |= (0x8|0x2);//yellow
	}
	
	
	// Clear the timer interrupt
	TimerIntClear(TIMER1_BASE, TIMER_TIMA_TIMEOUT);
	
}
