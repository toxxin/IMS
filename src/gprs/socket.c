/*
 * socket.c
 *
 *  Created on: 05.02.2014
 *      Author: anton
 */

#include "ims.h"

int createSocket(Socket *socket, GPRSSocketProto proto, char *ip, uint16_t port)
{
	int ret = E_OK;
	char tmpdata[50];
	char cmd[60];

	_DEBUG("Init socket...");

	switch (proto)
	{
	case UDP:
		assert(ip);
		assert(port);
		uint8_t index = 1;
		uint16_t local_port = 80;

		socket->id = index;
		socket->proto = proto;
		socket->local_port = local_port;
		socket->port = port;
		strcpy(socket->ip, ip);
		snprintf(&cmd[0], 60, "AT+WIPCREATE=%d,%d,%d,\"%s\",%d\r", proto, index, local_port, ip, port);
		break;
	case TCPcli:
	case TCPserv:
	default:
		_DEBUG("Unknown proto");
		break;
	}

	ret = getModuleData(cmd, &tmpdata[0]);

	Delay_ms(5000);

	/* wait URC +WIPREADY! */
	// int = waitURC(char *URC, uint16_t msec);

	return ret;
}


int closeSocket(Socket *socket)
{
	int ret = E_OK;
	char tmpdata[40];
	char cmd[30];

	_DEBUG("Close socket...");

	snprintf(&cmd[0], 30, "AT+WIPCLOSE=%d,%d\r", socket->proto, socket->id);

	ret = getModuleData(cmd, &tmpdata[0]);

	return ret;
}


int readSocket(Socket *socket, uint8_t *buf, size_t size, uint16_t msec)
{
	int ret = E_OK;
	char tmpdata[40];
	char cmd[30];


}


void writeSocket(Socket *socket, uint8_t *buf, size_t size)
{
	uint8_t i;

	for (i = 0; i < size; ++i)
		USART_BUF_TX(buf[i]);
}
