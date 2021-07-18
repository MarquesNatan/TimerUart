/*============================================================================*/
#include "serial.h"
/*============================================================================*/
#include <stdint.h>
/*============================================================================*/
void Serial_1_Config(serial_config_t* serialConfig)
{
    uint8_t brg8LOW;
    uint8_t brg8HIGH;
    uint8_t brg16HIGH;
       
    uint8_t error_8LOW;
    uint8_t error_8HIGH;
    uint8_t error_16HIGH;
    
    uint8_t error_min;
    
    // SPBRGH:SPBRG
    brg8LOW   = (_XTAL_FREQ/(serialConfig->serial_desired_baud * 64));
    brg8HIGH  = (_XTAL_FREQ/(serialConfig->serial_desired_baud * 16));
    brg16HIGH = (_XTAL_FREQ/(serialConfig->serial_desired_baud *  4));
    
    // ERROR CALCULATION
    error_8LOW   = (_XTAL_FREQ/(brg8LOW  * 64)) - serialConfig->serial_desired_baud;
    error_8HIGH  = (_XTAL_FREQ/(brg8HIGH * 16)) - serialConfig->serial_desired_baud;
    error_16HIGH = (_XTAL_FREQ/(brg16HIGH * 4)) - serialConfig->serial_desired_baud;
    
    // Select small error
    error_min = error_8LOW;
    
    BAUDCONbits.BRG16 = 0x00;
    TXSTAbits.BRGH = 0x00;
    
    SPBRG = (brg8LOW - 1);
    
    if(error_8HIGH < error_min)
    {
        error_min = error_8HIGH;
        
        BAUDCONbits.BRG16 = 0x00;
        TXSTAbits.BRGH = 0x01;
    
        SPBRG = (brg8HIGH - 1);
    }
    if(error_16HIGH < error_min)
    {
        BAUDCONbits.BRG16 = 0x01;
        TXSTAbits.BRGH = 0x01;
    
        SPBRG = (brg16HIGH - 1);
    }
    
    TXSTAbits.SYNC = serialConfig->serial_op_mode;
    
    RCSTAbits.SPEN = 0x01;

    if(serialConfig->serial_data_length == SERIAL_DATA_LENGTH_9)
    {
        TXSTAbits.TX9 = 0x01; 
        RCSTAbits.RC9 = 0x01; 
    }else
    {
        TXSTAbits.TX9 = 0x00;
        RCSTAbits.RC9 = 0x00; 
    }
    
    TXSTAbits.TXEN = 0x01;
    RCSTAbits.CREN = 0x01;
    
    PIE1bits.RCIE = 0x01;    
}
/*============================================================================*/
void Serial_Transmit(char data)
{
    TXREG = data;
    while(!TRMT);
}
/*============================================================================*/
uint8_t Serial_Receive(void)
{
    return RCREG;
}
/*============================================================================*/