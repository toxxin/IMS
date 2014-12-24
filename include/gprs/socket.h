/*
 * socket.h
 *
 *  Created on: 05.02.2014
 *      Author: anton
 */

#ifndef SOCKET_H_
#define SOCKET_H_


typedef enum
{
	UDP			= 1,
	TCPcli 		= 2,
	TCPserv		= 3,
	FTP			= 4
} GPRSSocketProto;


/** @defgroup GPRS Operation mode @{ */
#define GPRS_CONTINUOUS_MODE		1
#define GPRS_TRANSPARENT_MODE		2
/** @} */


typedef struct _Socket
{
	uint8_t id;
	GPRSSocketProto proto;
	uint16_t local_port;
	char ip[32];
	uint16_t port;
} Socket;

//TODO:: Consider change initialisation such as linux way!
int createSocket(Socket *socket, GPRSSocketProto proto, char *ip, uint16_t port);

int closeSocket(Socket *socket);

#endif /* SOCKET_H_ */
