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

enum _method_
{
    HTTPREQUEST_METHOD_GET = 0x0,
    HTTPREQUEST_METHOD_POST
};

struct _httpRequestMethodArgsPost_
{
	char **post_variable;
	char **post_value;
};

struct _httpRequestMethodArgsGet_
{
	char **get_variable;
	char **get_value;
};

struct httpRequest
{
    enum _method_ method;
    char * path;
    uint8 path_len;
    uint8 variables_num;
	union
	{
		struct _httpRequestMethodArgsPost_ * httpRequestMethodArgsPost;
		struct _httpRequestMethodArgsGet_ * httpRequestMethodArgsGet;
	}data;
};

static char StaticErrorMessageBuffer[] =
        "HTTP/1.1 404 Not Found\n"
        "Content-Type: text/html\n"
        "Connection: close\n\n"
        ;

static char StaticWebPageBuffer[] =
		"HTTP/1.1 200 OK\n"
		"Content-Type: text/html\n"
		"Connection: close\n\n"
		"<!DOCTYPE html>"
		"<html>"
		"<head><title>SneakyESP Setup</title>"
		"<style>html{height: 100%;width: 100%;}"
		"body {background-image: linear-gradient(blue,lightblue, white);}</style>"
		"</head>"
		"<body>"
		"<center>"
		"<h1>SneakyESP Setup Page</h1><br>"
		"<form action=\"/config\" method=\"POST\">"
		"SSID<input type=\"text\" name=\"SSID\"/><br>"
		"Password<input type=\"password\" name=\"PASSWORD\"/><br>"
		"<input type=\"submit\" value=\"Submit\"/>"
		"</form>"
		"</center>"
		"</body>"
		"</html>"
		;

//functions
void SetupSocketConfig_SocketSetup(void);
void SetupSocketConfig_SocketConnectCallbackFunction(void *);
//void SetupSocketConfig_SocketDisconnectCallbackFunction(void *);
void SetupSocketConfig_SocketDataRecvCallbackFunction(void *, char *, unsigned short);
void SetupSocketConfig_SendStaticWebpage(struct espconn *);
void SetupSocketConfig_SendErrorWebpage(struct espconn *);
struct httpRequest * SetupSocketConfig_ParseRequest(char *, unsigned short);
void SetupSocketConfig_ParsePOST(char *, unsigned short, struct httpRequest *);
void SetupSocketConfig_ParseGET(char *, unsigned short, struct httpRequest *);
#endif
