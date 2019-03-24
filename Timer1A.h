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



extern volatile bool Global_error;
extern volatile bool Global_warning;
extern volatile uint8_t warningCounter;
