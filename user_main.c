#include "user_config.h"
#include "setupconnection.h"
#include "status.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)

void ICACHE_FLASH_ATTR user_init(void)
{
    Status_setGlobalStatus(GLOBALSTATUS_INIT); //setting the global status as init
    Status_setConnectionStatus(CONNECTIONSTATUS_NOTCONNECTED); //setting connection as not connected
    SetupConnection_InitWifiConnection(); //will start the initial setup access point
}
