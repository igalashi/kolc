/* exam03.c 
 * Simple Http Mirror server
*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "tcplib.h"

#define MY_BUF_SIZE 8191
char mybuf[(MY_BUF_SIZE + 1)];
const char head[] =
	"HTTP/1.0 200 OK\r\n"
	"Content-Type: text/plain\r\n"
	"\r\n";

int net_getchar(SOCKET s)
{
	char b[2];
	if(recv(s, b, 1, 0) != 1)
		return EOF;
	return ((int)b[0]) & 255;
}

int doservice(SOCKET t)
{
	int rcvchars;
	int c;
	int lfcount;

	/* Fill received characters until <CR><LF><CR><LF> found */

	rcvchars = 0;
	mybuf[0] = 0;
	lfcount = 0;
	while((c = net_getchar(t)) != EOF)
	{
		if(rcvchars < (MY_BUF_SIZE - 1))
			mybuf[rcvchars++] = (char)c;

		if(c == '\n')
			++lfcount;
		else if(c != '\r')
			lfcount = 0;

		if(lfcount >= 2)
			break;
	}
	mybuf[rcvchars] = 0;
	tcpwrite(t, head, strlen(head));
	return tcpwrite(t, mybuf, rcvchars);
}

int main(int argc, char* argv[])
{
	static char crlf[] = {0x0a, 0x0d};
	SOCKET ss;
	SOCKET sa;
	int port;

	if(argc != 2)
	{
		fprintf(stderr, "Usage: %s port\n", argv[0]);
		return (-1);
	}
	port = atoi(argv[1]);

	netlibstart();
	ss = tcpsocket();
	tcplisten(ss, port, 5);
	while(1)
	{
		if((sa = accept(ss, 0, 0)) != INVALID_SOCKET)
		{
			doservice(sa);
			tcpclose(sa);
		}
	}
	netlibstop();
	return 0;
}
