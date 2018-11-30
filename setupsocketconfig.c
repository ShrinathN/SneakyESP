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

/* Description: This function sends the static error message/webpage
 * Input: pointer to the espconn struct
 * pdata is the pointer to the data
 * len is the length of the data
 * Output: none
*/
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
	struct httpRequest * req = (struct httpRequest *)SetupSocketConfig_ParseRequest(pdata, len);
	if(req->method == HTTPREQUEST_METHOD_GET) //if request is GET, here's some predefined responses
	{
		if(os_strcmp(req->path, "/") == 0)
			SetupSocketConfig_SendStaticWebpage((struct espconn *)arg); //sending over the static page
		else //anything else and send the error page
			SetupSocketConfig_SendErrorWebpage((struct espconn *)arg);
	}
	else if(req->method == HTTPREQUEST_METHOD_POST)
	{
#ifdef DEBUG
		uint8 * temp_i = (uint8 *)os_zalloc(sizeof(uint8));
		*temp_i = 0;
		while(*temp_i < req->variables_num)
		{
			os_printf("%s=%s\n",req->data.httpRequestMethodArgsPost->post_variable[*temp_i], req->data.httpRequestMethodArgsPost->post_value[*temp_i]);
			(*temp_i)++;
		}
#endif
	}
	espconn_disconnect((struct espconn *)arg); //disconnecting from the host
	Status_setConnectionStatus(CONNECTIONSTATUS_NOTCONNECTED); //set status as not connected
}

/* Description: This function parses the input HTTP request given to it, and calls the appropriate response
 * Input: pdata is the pointer to the data
 * len is the length of the data
 * Output: a pointer to a httpRequest data structure, it will contain a pointer to the path, and length of the path string
*/
struct httpRequest * ICACHE_FLASH_ATTR SetupSocketConfig_ParseRequest(char * pdata, unsigned short len)
{
#ifdef DEBUG
	os_printf("[INFO]Parsing Data...\n");
#endif
	struct httpRequest * req = (struct httpRequest *)os_zalloc(sizeof(struct httpRequest)); //allocating memory for the httpRequest structure
	req->path = os_strstr(pdata, "/"); //pointer to first '/'
	req->path_len = (uint8)((char *)os_strstr(req->path, " ") - (char *)req->path); //pointer to first " ", minus pointer to beginning (that is, the first "/"), basically gives us the length of the request string, including "/"
#ifdef DEBUG
	os_printf("\t[INFO]Length %d\n", req->path_len);
#endif
	char * temp = (char *)os_zalloc(5); //5 bytes worth of space, this memory will be used to hold the method string
	os_memcpy(temp, pdata, 5); //copying the first 4 bytes of the HTTP request to temp
	*(temp + 4) = 0; //null terminator
	if(os_strcmp(temp, "GET ") == 0) //its a GET request
		req->method = HTTPREQUEST_METHOD_GET;
	else if(os_strcmp(temp, "POST") == 0) //its a POST request
		req->method = HTTPREQUEST_METHOD_POST; //set request method as POST
	os_free(temp); //freeing the temp variable memory
	if(req->method == HTTPREQUEST_METHOD_POST) //since the request is a POST, te variables need to be parsed
	{
		req->data.httpRequestMethodArgsPost = (struct _httpRequestMethodArgsPost_ *)os_zalloc(sizeof(struct _httpRequestMethodArgsPost_));
		SetupSocketConfig_ParsePOST(pdata, len, req);
	}
#ifdef DEBUG
	*(req->path + req->path_len) = 0;
	os_printf("*\t%s\n%d\nDone!", req->path, os_strlen(req->path));
#endif
	return (struct httpRequest*)req;
}

/* Description: This function parses the POST request into variables and corresponding value strings
 * Input: _requestString_ is the pointer to char array containing the HTTP request
 * _length_ is the length of the above string
 * _request_ is a pointer to the struct httpRequest which is going to store all the information (indirectly ofc)
 * Output: nuffin
*/
void SetupSocketConfig_ParsePOST(char * _requestString_, unsigned short _length_, struct httpRequest * _request_)
{
#ifdef DEBUG
	os_printf("\t[INFO]POST method being parsed!\n");
#endif
	//declaring and allocating stuff
	_request_->variables_num = 1;
	char * temp_post_start = (char *)(os_strstr(_requestString_, "\r\n\r\n") + 4); //starting of the POST variables
	char * temp = temp_post_start;
	uint8 * length_post_str = (uint8 *)os_zalloc(sizeof(uint8));
	*length_post_str = (uint8)(_length_ - (temp - _requestString_));
	temp[*length_post_str] = 0; //setting last character as null terminator
	//counting number of variables and values
	while(*temp != 0) //travelling through the array
	{
		if(*temp == '&') //counting the number of '&' symbols
			_request_->variables_num++;
		temp++;
	}
//	_request_->variables_num++; //inc by one, now this equals no. of variables in request. Redundary, since _request_->variables_num is being set as 1
	_request_->data.httpRequestMethodArgsPost = os_zalloc(sizeof(struct _httpRequestMethodArgsPost_));
#ifdef DEBUG
	os_printf("\t[INFO]Variables found = %d\n", _request_->variables_num);
#endif
	_request_->data.httpRequestMethodArgsPost->post_variable = (char **)os_malloc(sizeof(char *) * _request_->variables_num); //allocating pointers to number of variables and their values
	_request_->data.httpRequestMethodArgsPost->post_value = (char **)os_malloc(sizeof(char *) * _request_->variables_num);
	temp = temp_post_start;
	//allocating memory for counters
	uint8 * temp_counter = (uint8 *)os_zalloc(sizeof(uint8)); //will keep count of characters since last point of interest
	uint8 * temp_postcounter = (uint8 *)os_zalloc(sizeof(uint8)); //will keep count of variables
//	*temp_postcounter = 0; //not really needed because we're using os_zalloc
	//TODO: This might mess up malformed POST requests, find a better way
	while(*temp_postcounter < _request_->variables_num) //while all variables and their values are not found
	{
		*temp_counter = 0;
		//finding the variable name
		while(*temp != '=') //will keep parsing until a '=' is found
		{
			(*temp_counter)++;
			temp++;
		}
		//copying variable string to memory
		_request_->data.httpRequestMethodArgsPost->post_variable[*temp_postcounter] = (char *)os_malloc(*temp_counter + 1);//allocating memory for variable string
		os_memcpy(_request_->data.httpRequestMethodArgsPost->post_variable[*temp_postcounter], (temp - *temp_counter)/*temp is current pointer location, *temp_counter number of bytes between temp and '='*/, *temp_counter);//copying string data to memory
		_request_->data.httpRequestMethodArgsPost->post_variable[*temp_postcounter][*temp_counter] = 0;//setting null terminator at last
		temp++; //incrementing character, so that the next string does not start at '='
		//this means the variable name has been found, and copied to the memory
#ifdef DEBUG
		os_printf("\t[INFO]req->post_variable[%d]=%s\n",*temp_postcounter, _request_->data.httpRequestMethodArgsPost->post_variable[*temp_postcounter]);
#endif
		//now find out the value of the variable
		*temp_counter = 0; //reseting character counter
		while(*temp != '&' && *temp != '\0') //will continue until an '&' or null terminator is found
		{
			(*temp_counter)++;
			temp++;
		}
		//copying value string to memory
		_request_->data.httpRequestMethodArgsPost->post_value[*temp_postcounter] = (char *)os_malloc(*temp_counter + 1);//allocating memory for value string
		os_memcpy(_request_->data.httpRequestMethodArgsPost->post_value[*temp_postcounter], (temp - *temp_counter), *temp_counter);//copying string data to memory
		_request_->data.httpRequestMethodArgsPost->post_value[*temp_postcounter][*temp_counter] = 0;//setting null terminator at last
		temp++; //incrementing character, so that next string does not start at '&'
#ifdef DEBUG
		os_printf("\t[INFO]req->post_value[%d]=%s\n",*temp_postcounter, _request_->data.httpRequestMethodArgsPost->post_value[*temp_postcounter]);
#endif
		(*temp_postcounter)++; //incrementing number of variable-value pairs
	}
	//freeing up memory
	os_free(temp);
	os_free(temp_counter);
	os_free(temp_postcounter);
	os_free(temp_post_start);
	os_free(length_post_str);
}

/* Description: This function parses the GET request into variables and corresponding value strings
 * Input: _requestString_ is the pointer to char array containing the HTTP request
 * _length_ is the length of the above string
 * _request_ is a pointer to the struct httpRequest which is going to store all the information (indirectly ofc)
 * Output: nuffin
*/
void SetupSocketConfig_ParseGET(char * _requestString_, unsigned short _length_, struct httpRequest * _request_)
{
#ifdef DEBUG
	os_printf("\t[INFO]GET method being parsed!\n");
#endif
	_request_->variables_num = 1;
	char * startOfGetVars = (char *)(os_strstr(_requestString_, "?") + 1); //this is the starting of GET variables after the request, +1 because ? is excluded
	char * temp = startOfGetVars; //temp variable used for parsing
	//counting the number of variables
	while(*temp != 0) //travelling through the array
	{
		if(*temp == '&') //counting the number of '&' symbols
			_request_->variables_num++;
		temp++;
	}
//	_request_->variables_num++; //inc by one, now this equals no. of variables in request. Redundary, since _request_->variables_num is being set as 1
}
