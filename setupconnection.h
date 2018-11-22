/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Thu Nov 22 21:58:42 IST 2018
 * Description: This file serves as the header file to connection.c file. It deals with setting up and connecting to WiFi access points.
*/
#ifndef CONNECTION_H
#define CONNECTION_H
#include "user_config.h"

void SetupConnection_InitWifiConnection(void);
void SetupConnection_WifiEventHandlerCallbackFunction(System_Event_t *);
#endif // CONNECTION_H
