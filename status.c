/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Fri Nov 23 00:10:23 IST 2018
 * Description: This file is here to implement the functions in status.h
*/
#include "status.h"
#include "user_config.h"

//sets global status
void ICACHE_FLASH_ATTR Status_setGlobalStatus(enum _globalStatus_ temp)
{
#ifdef DEBUG
    os_printf("[INFO]Setting globalStatus = %d\n", (uint8)temp);
#endif
    globalStatus = temp;
}

//return global status
uint8 ICACHE_FLASH_ATTR Status_getGlobalStatus(void)
{
    return (uint8)globalStatus;
}

//sets connection status
void ICACHE_FLASH_ATTR Status_setConnectionStatus(enum _connectionStatus_ temp)
{
#ifdef DEBUG
    os_printf("[INFO]Setting connectionStatus = %d\n", (uint8)temp);
#endif
    connectionStatus = temp;
}

//gets connection status
uint8 ICACHE_FLASH_ATTR Status_getConnectionStatus(void)
{
    return (uint8)connectionStatus;
}
