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
    espconn_send(my_esp, (uint8 *)StaticWebPageBuffer, (uint16)os_strlen(StaticWebPageBuffer));
#ifdef DEBUG
    os_printf("Done!\n");
#endif
}

void ICACHE_FLASH_ATTR SetupSocketConfig_SendErrorWebpage(struct espconn * my_esp)
{
#ifdef DEBUG
    os_printf("[INFO]Sending error page to client...");
#endif
    espconn_send(my_esp, (uint8 *)StaticErrorMessageBuffer, (uint16)os_strlen(StaticErrorMessageBuffer));
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
    os_printf("[INFO]Data received\n%s\n", pdata);
#endif
    SetupSocketConfig_ParseData((struct espconn *)arg, pdata, len);
    espconn_disconnect((struct espconn *)arg); //disconnecting from the host
    Status_setConnectionStatus(CONNECTIONSTATUS_NOTCONNECTED); //set status as not connected
}

/* Description: This function parses the input HTTP request given to it, and calls the appropriate response
 * Input: pdata is the pointer to the data
 * len is the length of the data
 * Output: a pointer to a httpRequest data structure, it will contain a pointer to the path, and length of the path string
*/
struct httpRequest * ICACHE_FLASH_ATTR SetupSocketConfig_ParseData(struct espconn * arg,char * pdata, unsigned short len)
{
#ifdef DEBUG
    os_printf("[INFO]Parsing Data...\n");
#endif
    struct httpRequest * req = (struct httpRequest *)os_zalloc(sizeof(struct httpRequest)); //allocating memory for the httpRequest structure
    req->path = os_strstr(pdata,"/"); //pointer to first '/'
    req->path_len = (uint8)((char *)os_strstr(req->path, " ") - (char *)req->path); //pointer to first " ", minus pointer to beginning (that is, the first "/"), basically gives us the length of the request string, including "/"
#ifdef DEBUG
    *(req->path + req->path_len) = 0;
    os_printf("*\t%s\nDone!",req->path);
#endif
    if(os_strcmp("/favicon.ico", req->path) != 0) //if its a favicon request, send the error message
        SetupSocketConfig_SendErrorWebpage(arg);
    else if(os_strcmp("/", req->path) != 0)
        SetupSocketConfig_SendStaticWebpage((struct espconn *)arg); //sending over the static page
}
