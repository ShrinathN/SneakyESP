#include "user_config.h"
#include "connection.h"
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)

void user_init(void)
{
    Connection_InitConnection();
}
