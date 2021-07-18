#include "pic18f4520/fuse/fuse.h"
#include <xc.h>
#include <stdint.h>
/*============================================================================*/
#include "pic18f4520/interrupt/interrupt.h"
#include "pic18f4520/timer/timer.h"
#include "pic18f4520/serial/serial.h"
#include "board/peripheral_controller/peripheral_controller.h"
#include "board/board_definitions/board_definitions.h"
#include "pic18f4520/gpio/gpio.h"
#include "board/pinout/pinout.h"
/*============================================================================*/
timer_config_t timerConfig = {
    .timer_length = TIMER_LENGTH_16, 
    .timer_clk_src = TIMER_CLKO_SRC,
    .timer_transition = TIMER_TRANSITION_LOW_HIGH,
    .timer_prescaler_assign = TIMER_PRESCALER_IS_ASSIGNED,
    .timer_prescaler_value = TIMER_PRESCALER_8
};
/*============================================================================*/
serial_config_t serialConfig = 
{
    .serial_sync_com = SERIAL_ASSYNC_MODE,
    .serial_data_length = SERIAL_DATA_LENGTH_8,
    .serial_op_mode = SERIAL_MASTER_MODE,
    .serial_desired_baud = SPEED_SERIAL
};
/*============================================================================*/
extern global_timer_t global_timer_value;
/*============================================================================*/
uint64_t last_time_blink_led_0 = 0;
uint64_t last_time_blink_led_1 = 0;
uint8_t serial_byte_received = 0;
/*============================================================================*/
void tickHook_func(global_timer_t *timer_value)
{
    if((*timer_value - last_time_blink_led_0) >= LED1_BLINK_S)
    {
        DIGITAL_PIN_TOGGLE(LED_HEARTBEAT1_PORT, LED_HEARTBEAT1_MASK);
        last_time_blink_led_0 = *timer_value;
    }
    if((*timer_value - last_time_blink_led_1) >= LED2_BLINK_S)
    {
        DIGITAL_PIN_TOGGLE(LED_HEARTBEAT2_PORT, LED_HEARTBEAT2_MASK);
        last_time_blink_led_1 = *timer_value;
    }
}
/*============================================================================*/    
void __interrupt() TASK_Manager(void)
{
    // Verifica se a fonte de interrupcao foi do timer 0
    if (INTCONbits.TMR0IF == 0x01)
    {
        global_timer_value++;
        tickHook_Execute(&global_timer_value); // Excuta uma a função especifica
        TMR0 = 0x85EE; 
        INTCONbits.T0IF = 0x00;
    }
    
    // Verifica se a fonte de interrupcao foi da serial
    if(PIR1bits.RCIF)
    {    
        serial_byte_received  = Serial_Receive();    
        if(serial_byte_received == 0x41) // se  o byte recebido foi A (0x41)
        {
            PIN_DIGITAL_WRITE(PIN_HIGH, LED_HEARTBEAT3_PORT, LED_HEARTBEAT3_MASK);
        }else if(serial_byte_received == 0x44) // se o byte recebido foi um B (0x44)
        {
            PIN_DIGITAL_WRITE(PIN_LOW, LED_HEARTBEAT3_PORT, LED_HEARTBEAT3_MASK);
        }
        PIR1bits.RCIF = 0x00; 
    }
}
/*============================================================================*/    
void main(void)
{
    Peripheral_Controller(NULL);
    Interrupt_GlobalEnable();   
    Serial_1_Config(&serialConfig);     
    Timer0_Config(&timerConfig);
    Timer0_SetTickHook(tickHook_func);
    
    while(1)
    {
    }
    return;
}
/*============================================================================*/