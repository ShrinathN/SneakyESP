/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Fri Nov 23 00:37:47 IST 2018
 * Description: Header file for socketconfig.c Will contain all functions definitions and constants related to initial setup ap socket connections
*/
#ifndef SETUPSOCKETCONFIG_H
#define SETUPSOCKETCONFIG_H
#include "user_config.h"

//data structures used
static struct espconn esp; //global espconn
static esp_tcp tcp; //global esp_tcp


//functions
void SetupSocketConfig_socketSetup(void);
void SetupSocketConfig_socketConnectCallbackFunction(void *);
void SetupSocketConfig_socketDisconnectCallbackFunction(void *);
void SetupSocketConfig_socketDataRecvCallbackFunction(void *, char *, unsigned short);

#endif
