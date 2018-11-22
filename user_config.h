/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Tue Jun 26 19:05:52 2018 +0530
 * Description: A header file that contains all the required header files
*/
#include "airkiss.h"
#include "at_custom.h"
#include "c_types.h"
#include "c_types.h.orig"
#include "eagle_soc.h"
#include "esp_sdk_ver.h"
#include "espnow.h"
#include "ets_sys.h"
#include "gpio.h"
#include "ip_addr.h"
#include "mem.h"
#include "mesh.h"
#include "os_type.h"
#include "osapi.h"
#include "ping.h"
#include "pwm.h"
#include "queue.h"
#include "simple_pair.h"
#include "smartconfig.h"
#include "sntp.h"
#include "spi_flash.h"
#include "upgrade.h"
#include "user_interface.h"
#include "espconn.h"
#include "wpa2_enterprise.h"

//GLOBAL MACROS

//this is the SSID and Password to the default setup access point
#define AP_SSID "SneakyESPConfig"
#define AP_PASSWORD "987654321"
