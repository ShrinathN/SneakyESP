/* Name: Shrinath Nimare
 * Time: Thu Nov 22 22:42:40 IST 2018
 * Description: This is the C file to the connection.h header file. It deals with setting up and connecting to WiFi access points.
*/
#include "user_config.h"
#include "connection.h"

void ICACHE_FLASH_ATTR Connection_InitWifiConnection()
{
    struct softap_config * softap_configuration = (struct softap_config *)os_zalloc(sizeof(struct softap_config)); //temp struct used to setup the access point
    os_strcpy(softap_configuration->ssid, AP_SSID); //setting the SSID
    softap_configuration->ssid_len = os_strlen(AP_SSID);
    os_strcpy(softap_configuration->password, AP_PASSWORD);
}
