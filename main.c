#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "driverlib/i2c.h"
#include "driverlib/pin_map.h"
#include "driverlib/ssi.h"
#include "LoraAPI.h"

//global data
volatile bool Global_error = true;
volatile bool Global_warning = true;
volatile uint8_t warningCounter = 0;
uint8_t storageArray[100] = {0};



//declaration
void SubsystemInit(void);

//local variables
uint8_t messageSize = 0;
bool newMessage = false;



int i=0;







//test values:
uint8_t arrayTest[5] = {0x11, 0x22, 0x33, 0x44, 0x55};
uint8_t testArraySize = 5;



void GPIOPortE_Handler(void){
	IntMasterDisable();
	messageSize = LoraGetMessage();
	if(messageSize != 0x00){
		newMessage = true;
	}else{
		Global_warning = false;
		warningCounter++;
	}
	IntMasterEnable();
}


int main(void)
{
		//initilize subsystems & Lora
    SubsystemInit();
		Global_error = LoraInitRx();
		Global_error = LoRaMode(RXCONTINUOUS);
		//initilize LED to green (status is good, initilization complete)
		GPIO_PORTF_DATA_R |= 0x08;


			while(1){
				if(newMessage == true){
					//do stuff (will be stored in storageArray, and use messageSize to determine size)
					newMessage = false;
				}

						
		}

}//main





