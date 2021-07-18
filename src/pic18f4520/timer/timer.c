/*============================================================================*/
#include "timer.h"
/*============================================================================*/
static void (*tickHook_func_prt)(global_timer_t*);
/*============================================================================*/
void Timer0_Config( timer_config_t* timerConfig )
{
    T0CONbits.TMR0ON = 0x00;
    
    T0CONbits.T08BIT = timerConfig->timer_length;
    
    T0CONbits.T0CS = timerConfig->timer_clk_src; 
       
    if(!(T0CONbits.PSA =  timerConfig->timer_prescaler_assign))
    {
        T0CONbits.T0PS = timerConfig->timer_prescaler_value;
    }
    
    INTCONbits.TMR0IE = 0x01;
    INTCON2bits.TMR0IP = 0x01;
    
    TMR0 = 0x85EE;
    
    T0CONbits.TMR0ON = 0x01;
}
/*============================================================================*/
void Timer0_SetTickHook(void (*tickFunc)(global_timer_t*))
{
    tickHook_func_prt = tickFunc;
}
/*============================================================================*/
void tickHook_Execute(global_timer_t* global_timer_value)
{
    tickHook_func_prt(global_timer_value);
}
/*============================================================================*/
global_timer_t Timer0_GetGlobalTime( void )
{
    return global_timer_value;
}