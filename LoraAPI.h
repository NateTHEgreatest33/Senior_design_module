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


//defines
#define CONFIG_REG_ADDR 0x01
#define LORA_FIFO_ADDR 0x00
enum modes{SLEEP, STBY, FSTX, TX, FSRX, RXCONTINUOUS, RXSINGLE, CAD}; 
extern uint8_t storageArray[100];


//functions

uint32_t loRaRead(uint8_t RegisterAddress_8bit);

void loRaWrite(uint8_t RegisterAddress_8bit, volatile uint8_t data8Bit);


//should be avoided as no device (besides MAYBE, HUB, should be initilized to TX mode) & bc it does not verify G0 as output (Rx does)
bool LoraInitTx(void);

bool LoraInitRx(void);

bool LoRaMode(uint8_t mode);

bool LoraSendMessage(uint8_t Message[], uint8_t number_of_bytes);

uint8_t LoraGetMessage(void);

