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
void SetupSocketConfig_socketSetup(void)
{
    esp.type = ESPCONN_TCP;
    esp.state = ESPCONN_NONE;
    esp.proto.tcp = (esp_tcp *)&tcp;
    esp.proto.tcp->local_port = (int)espconn_port(); //gets an avaliable port on its own
    espconn_accept(&esp); //setting esp as the espconn
    espconn_regist_connectcb(&esp, SetupSocketConfig_socketConnectCallbackFunction);
    espconn_regist_disconcb(&esp, SetupSocketConfig_socketDisconnectCallbackFunction);
    espconn_regist_recvcb(&esp, SetupSocketConfig_socketDataRecvCallbackFunction);
}
