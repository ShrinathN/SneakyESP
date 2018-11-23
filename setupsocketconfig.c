/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Fri Nov 23 00:41:04 IST 2018
 * Description: Header file for socketconfig.c Will contain all functions definitions and constants related to initial setup ap socket connections
*/
#include "setupsocketconfig.h"
#include "user_config.h"
#include "status.h"

/* Description: This function will setup the callback functions
 * and start listening using the global espconn esp
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SocketSetup(void)
{
#ifdef DEBUG
    os_printf("[INFO]Setting up sockets...");
#endif
    esp.type = ESPCONN_TCP;
    esp.state = ESPCONN_NONE;
    esp.proto.tcp = (esp_tcp *)&tcp;
    esp.proto.tcp->local_port = 80; //starts listening on port 80
    espconn_accept(&esp); //setting esp as the espconn
    espconn_regist_connectcb(&esp, SetupSocketConfig_SocketConnectCallbackFunction);
//    espconn_regist_disconcb(&esp, SetupSocketConfig_SocketDisconnectCallbackFunction);
    espconn_regist_recvcb(&esp, SetupSocketConfig_SocketDataRecvCallbackFunction);
#ifdef DEBUG
    os_printf("Done!\n");
#endif
}

/* Description: This is the function called when the espconn struct esp has connected to a host
 * It calls a function to send the hard coded web page over
 * Input: pointer arg, points to the espconn structure being dealt with here
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SocketConnectCallbackFunction(void * arg)
{
#ifdef DEBUG
    os_printf("[INFO]Client connected on port!\n");
#endif
    Status_setConnectionStatus(CONNECTIONSTATUS_CONNECTED); //set status as connected
    SetupSocketConfig_SendStaticWebpage((struct espconn *)arg); //sending over the static page
    espconn_disconnect((struct espconn *)arg); //disconnecting from the host
    Status_setConnectionStatus(CONNECTIONSTATUS_NOTCONNECTED); //set status as not connected
#ifdef DEBUG
    os_printf("[INFO]Disconnected from client!\n");
#endif
}

/* Description: This callback function sends over the hard coded web page, along with the HTTP header
 * Input: pointer to the struct espconn structure
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SendStaticWebpage(struct espconn * my_esp)
{
#ifdef DEBUG
    os_printf("[INFO]Sending data to client...");
#endif
    espconn_send(my_esp, StaticWebPageBuffer, (uint16)os_strlen((char *)StaticWebPageBuffer));
#ifdef DEBUG
    os_printf("Done!\n");
#endif
}

/* Description: This callback function is called when data is received from the host
 * Input: pointer arg, points to the espconn structure being dealt with here
 * pdata is the pointer to the data
 * len is the length of the data
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupSocketConfig_SocketDataRecvCallbackFunction(void * arg, char * pdata, unsigned short len)
{
#ifdef DEBUG
    os_printf("[INFO]Data sent\n%s\n", pdata);
#endif
}
