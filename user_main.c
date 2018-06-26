#include "user_config.h"
#define AP_SSID ""
#define AP_PASSWORD ""
#define SET_BAUD(UART_NUMBER, BAUD_RATE) uart_div_modify(UART_NUMBER, UART_CLK_FREQ / BAUD_RATE)
void user_init(void)
{
}
