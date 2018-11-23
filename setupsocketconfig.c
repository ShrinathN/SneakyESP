/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Fri Nov 23 00:41:04 IST 2018
 * Description: Header file for socketconfig.c Will contain all functions definitions and constants related to initial setup ap socket connections
*/
#include "setupsocketconfig.h"

/* Description: This function will setup the callback functions
 * and start listening using the global espconn esp
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SocketSetup(void)
{
    esp.type = ESPCONN_TCP;
    esp.state = ESPCONN_NONE;
    esp.proto.tcp = (esp_tcp *)&tcp;
    esp.proto.tcp->local_port = 80; //starts listening on port 80
    espconn_accept(&esp); //setting esp as the espconn
    espconn_regist_connectcb(&esp, SetupSocketConfig_SocketConnectCallbackFunction);
//    espconn_regist_disconcb(&esp, SetupSocketConfig_SocketDisconnectCallbackFunction);
//    espconn_regist_recvcb(&esp, SetupSocketConfig_SocketDataRecvCallbackFunction);
}

/* Description: This is the function called when the espconn struct esp has connected to a host
 * It calls a function to send the hard coded web page over
 * Input: pointer arg, points to the espconn structure being dealt with here
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SocketConnectCallbackFunction(void * arg)
{
    SetupSocketConfig_SendStaticWebpage((struct espconn *)arg);
}

void ICACHE_FLASH_ATTR SetupSocketConfig_SendStaticWebpage(struct espconn * my_esp)
{
    espconn_send(my_esp, StaticWebPageBuffer, (uint16)os_strlen((char *)StaticWebPageBuffer));
}
