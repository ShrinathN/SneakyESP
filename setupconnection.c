/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Thu Nov 22 22:42:40 IST 2018
 * Description: This is the C file to the connection.h header file. It deals with setting up and connecting to WiFi access points.
*/
#include "user_config.h"
#include "setupconnection.h"
#include "setupsocketconfig.h"

/* Description: This function sets up the initial setup wifi access point. It will also set the event handler callback function for the setup process.
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupConnection_InitWifiConnection()
{
    struct softap_config * softap_init_configuration = (struct softap_config *)os_zalloc(sizeof(struct softap_config)); //temp struct used to setup the access point
    os_strcpy((char *)softap_init_configuration->ssid, AP_SSID); //setting the SSID
    softap_init_configuration->ssid_len = (uint8)os_strlen(AP_SSID); //setting SSID length
    os_strcpy((char *)softap_init_configuration->password, AP_PASSWORD); //setting SSID password
    wifi_softap_set_config_current(softap_init_configuration); //setting the config
    wifi_set_event_handler_cb(SetupConnection_WifiEventHandlerCallbackFunction);
    os_free(softap_init_configuration); //freeing up space
}

/* Description: This is the event handler callback function for the initial setup process, it calls the socket initialization functions
 * Input: none
 * Output: none
*/
void ICACHE_FLASH_ATTR SetupConnection_WifiEventHandlerCallbackFunction(System_Event_t * event)
{
    if(event->event == EVENT_SOFTAPMODE_STACONNECTED) //executed when a station has connected to the access point
    {
        SetupSocketConfig_socketSetup(); //will setup the http server
    }
}
