/* exam02.c
 * Simple Client
*/

#include <stdio.h>
#include <stdlib.h>
#include "tcplib.h"

#define MYBUFSIZE 1023

int main(int argc, char* argv[])
{
	char buf[(MYBUFSIZE + 1)];
	int port;
	SOCKET s;
	int n;

	if(argc != 3)
	{
		fprintf(stderr, "Usage: %s host port\n", argv[0]);
		return (-1);
	}
	port = atoi(argv[2]);

	netlibstart();
	s = tcpsocket();
	tcpopen(s, argv[1], port);
	while((n = tcpread(s, buf, MYBUFSIZE)) > 0)
	{
		buf[n] = 0;
		printf("%s", buf);
	}
	tcpclose(s);
	netlibstop();
	return 0;
}
