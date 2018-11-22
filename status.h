/* Author: Shrinath Nimare <shrinath.nimare@gmail.com>
 * Time: Fri Nov 23 00:10:23 IST 2018
 * Description: This file contains all the definitions for the statuse. Pretty self explanatory.
*/

#ifndef STATUS_H
#define STATUS_H
#include "user_config.h"

//this is meant to serve as a global status notifier
static enum _globalStatus_
{
    GLOBALSTATUS_INIT,
    GLOBALSTATUS_INITAP,
    GLOBALSTATUS_WORKINGAP
}globalStatus;

//this is meant to serve as a global connection status notifier
static enum _connectionStatus_
{
    CONNECTIONSTATUS_NOTCONNECTED,
    CONNECTIONSTATUS_LISTENING,
    CONNECTIONSTATUS_CONNECTED

}connectionStatus;

//global status functions
void Status_setGlobalStatus(enum _globalStatus_);
uint8 Status_getGlobalStatus(void);

//connection status functions
void Status_setConnectionStatus(enum _connectionStatus_);
uint8 Status_getConnectionStatus(void);


#endif // STATUS_H
