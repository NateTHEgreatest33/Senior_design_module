#define PART_TM4C123GH6PM 1;

#include <stdint.h>
#include <stdbool.h>
#include "inc/tm4c123gh6pm.h"
#include "inc/hw_memmap.h"
#include "inc/hw_types.h"
#include "driverlib/sysctl.h"
#include "driverlib/interrupt.h"
#include "driverlib/gpio.h"
#include "inc/hw_gpio.h"
#include "driverlib/timer.h"
#include "driverlib/ssi.h"
#include "driverlib/pin_map.h"
#include "driverlib/pwm.h"

//defined in main.c
void GPIOPortE_Handler(void);

/* - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - 
SubsystemInit(): Initilizes on board subsystems (SSI, Timer, GPIO)
Inputs:
- n/a
Outputs:
- n/a
Notes:
- Subsystems currently added: SSI_0, TImer_1A, GPIO_PORT_F, LoraInterupt Port
Port Mapping:
- SSI0
    - CS:
		- SCLK:
		- MOSI:
		- MISO:
- Timer1A:
		- n/a
- PortF
    - (all)
- LORA
    - E1 (external interrrupt) acts as Rx received


- - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - - */
void SubsystemInit(void)
{
	//timer
	uint32_t intTimer = (SysCtlClockGet() /100);

	/***********************
	* Bus Clock            *
	***********************/
	//Set system clock to 40MHz
	SysCtlClockSet(SYSCTL_SYSDIV_2_5|SYSCTL_USE_PLL|SYSCTL_XTAL_16MHZ|SYSCTL_OSC_MAIN);

	
	
	/***********************
	* Timer1A              *
	***********************/
	//Enable clock for TIMER1 and configure for periodic interrupt
	SysCtlPeripheralEnable(SYSCTL_PERIPH_TIMER1);  //Enable clock for TIMER1
	TimerConfigure(TIMER1_BASE, TIMER_CFG_PERIODIC);  //Set Timer1 to periodic mode

	//Set the interrupt period
	
	TimerLoadSet(TIMER1_BASE, TIMER_A, intTimer -1);
	

	//Enable interrupts and TIMER1 interrupt
	IntEnable(INT_TIMER1A);  //Enable TIMER1 interrupt in the NVIC
	TimerIntEnable(TIMER1_BASE, TIMER_TIMA_TIMEOUT);   //Enable interrupt in the timer module
	IntMasterEnable();  //Enable all interrupts in the NVIC
	
	IntPrioritySet(INT_TIMER1A, 0xE0);
	
	//Start the timer
	TimerEnable(TIMER1_BASE, TIMER_A);
	
	
	
	
	
	/***********************
	* PortF (+ buttons)    *
	***********************/
	
	//Enable PORTF
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOF);  //Enable clock on GPIO PORTF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOF))
	{}
		
	GPIOPinTypeGPIOOutput(GPIO_PORTF_BASE, GPIO_PIN_1|GPIO_PIN_2|GPIO_PIN_3);  //Sets pins 1,2,3 as outputs
	
	//Configure PORTF switches
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = GPIO_LOCK_KEY;  //unlock PF0
	HWREG(GPIO_PORTF_BASE + GPIO_O_CR) |= 0x01;
	HWREG(GPIO_PORTF_BASE + GPIO_O_LOCK) = 0;
	GPIODirModeSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_DIR_MODE_IN);
	GPIOPadConfigSet(GPIO_PORTF_BASE, GPIO_PIN_4|GPIO_PIN_0, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);

	
	/***********************
	* SSI0                 *
	***********************/
		
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOA);  //Enable clock on GPIO PORTA
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOA)){}
	SysCtlPeripheralEnable(SYSCTL_PERIPH_SSI0);
	GPIOPinTypeSSI(GPIO_PORTA_BASE, GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_2);
		
	GPIOPinConfigure(GPIO_PA2_SSI0CLK);
	//GPIOPinConfigure(GPIO_PA3_SSI0FSS);//!! - A3 is manually controlled - !!
	GPIOPinConfigure(GPIO_PA4_SSI0RX);
	GPIOPinConfigure(GPIO_PA5_SSI0TX);
	
	SysCtlDelay(20000000);
	
	//A3 - CS
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);  //Sets pins A3 as outputs
	GPIODirModeSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_DIR_MODE_OUT);	
	GPIOPinTypeGPIOOutput(GPIO_PORTA_BASE, GPIO_PIN_3);  //Sets pins A3 as outputs
	GPIOPadConfigSet(GPIO_PORTA_BASE, GPIO_PIN_3, GPIO_STRENGTH_2MA, GPIO_PIN_TYPE_STD_WPU);
	SysCtlDelay(20000000);
		
		
	//max baudrate is 1000000
	SSIConfigSetExpClk(SSI0_BASE, SysCtlClockGet(), SSI_FRF_MOTO_MODE_0, SSI_MODE_MASTER, 100000, 8);//running at 100Hz

	SSIEnable(SSI0_BASE);
		
	//toggle A3(CS) high to start
	GPIO_PORTA_DATA_R |= 1<<3;


  /***********************
	* LoRa                 *
	***********************/
	//Enable PORTE
	SysCtlPeripheralEnable(SYSCTL_PERIPH_GPIOE);  //Enable clock on GPIO PORTF
	while(!SysCtlPeripheralReady(SYSCTL_PERIPH_GPIOE)){}
		
	GPIODirModeSet(GPIO_PORTE_BASE, GPIO_PIN_1, GPIO_DIR_MODE_IN);
	GPIOIntRegister(GPIO_PORTE_BASE,GPIOPortE_Handler);
	GPIOIntEnable(GPIO_PORTE_BASE, GPIO_INT_PIN_1);



	/***********************
	* External Peripheral  *
	***********************/
	




	/***********************
	* enable int           *
	***********************/
	IntMasterEnable();
	
}

