/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Thu Nov 22 22:42:40 IST 2018
 * Description: This is the C file to the connection.h header file. It deals with setting up and connecting to WiFi access points.
*/
#include "user_config.h"
#include "status.h"
#include "setupconnection.h"
#include "setupsocketconfig.h"

/* Description: This function sets up the initial setup wifi access point. It will also set the event handler callback function for the setup process.
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupConnection_InitWifiConnection()
{
#ifdef DEBUG
    os_printf("[INFO]Initializing init wifi connection...");
#endif
    struct softap_config * softap_init_configuration = (struct softap_config *)os_zalloc(sizeof(struct softap_config)); //temp struct used to setup the access point
    softap_init_configuration->max_connection = 1; //max connections, 1 for security reasons
    softap_init_configuration->authmode = AUTH_WPA2_PSK; //WPA2_PSK as the authentication method
    os_strcpy((char *)softap_init_configuration->ssid, AP_SSID); //setting the SSID
    softap_init_configuration->ssid_len = (uint8)os_strlen(AP_SSID); //setting SSID length
    os_strcpy((char *)softap_init_configuration->password, AP_PASSWORD); //setting SSID password
    wifi_softap_set_config_current(softap_init_configuration); //setting the config
    wifi_set_event_handler_cb(SetupConnection_WifiEventHandlerCallbackFunction);
    os_free(softap_init_configuration); //freeing up space
#ifdef DEBUG
    os_printf("Done!\n");
#endif
}

/* Description: This is the event handler callback function for the initial setup process, it calls the socket initialization functions
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupConnection_WifiEventHandlerCallbackFunction(System_Event_t * event)
{
    if(event->event == EVENT_SOFTAPMODE_STACONNECTED) //executed when a station has connected to the access point
    {
#ifdef DEBUG
    os_printf("[INFO]Running WiFi event handler, station connected!\n");
#endif
        SetupSocketConfig_SocketSetup(); //will setup the http server
        Status_setConnectionStatus(CONNECTIONSTATUS_LISTENING);
    }
}
