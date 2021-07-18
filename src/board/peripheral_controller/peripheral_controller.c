/*============================================================================*/
#include "peripheral_controller.h"
#include "../../board/pinout/pinout.h"
#include "../../pic18f4520/gpio/gpio.h"
/*============================================================================*/
void Peripheral_Controller(void* args)
{
    // Configura os pinos como saída digital
    PIN_CONFIGURE_DIGITAL(PIN_OUTPUT, LED_HEARTBEAT1_PORT, LED_HEARTBEAT1_MASK);
    PIN_CONFIGURE_DIGITAL(PIN_OUTPUT, LED_HEARTBEAT2_PORT, LED_HEARTBEAT2_MASK);
    PIN_CONFIGURE_DIGITAL(PIN_OUTPUT, LED_HEARTBEAT3_PORT, LED_HEARTBEAT3_MASK);
    
    // O estado dos leds começam invertidos
    PIN_DIGITAL_WRITE(PIN_LOW,  LED_HEARTBEAT1_PORT, LED_HEARTBEAT1_MASK);
    PIN_DIGITAL_WRITE(PIN_HIGH, LED_HEARTBEAT2_PORT, LED_HEARTBEAT2_MASK);
    PIN_DIGITAL_WRITE(PIN_HIGH, LED_HEARTBEAT3_PORT, LED_HEARTBEAT3_MASK);
}
/*============================================================================*/