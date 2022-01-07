/* exam01.c 
 * Simple server
*/

#include <stdio.h>
#include <stdlib.h>
#include "tcplib.h"

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
			tcpwrite(sa, "Hello", 5);
			tcpwrite(sa, crlf, 2);
			tcpclose(sa);
		}
	}
	netlibstop();
	return 0;
}
