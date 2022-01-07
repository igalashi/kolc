/* tcplib.c
 *
*/

#include "tcplib.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

SOCKET
tcpsocket(void)
{
	return socket(PF_INET, SOCK_STREAM, 0);
}

int
tcpopen(SOCKET sd, const char* host, int port)
{
	struct sockaddr_in srvaddr;
#ifdef HAS_GETADDRINFO
	struct sockaddr_in* resaddr;
	struct addrinfo hints;
	struct addrinfo* res;
	int err;
#else
	struct hostent *hp;
	unsigned long inaddr;
#endif	/* HAS_GETADDRINFO */

	memset((char*)&srvaddr, 0, sizeof(srvaddr));
	srvaddr.sin_family = AF_INET;
	srvaddr.sin_port = htons((u_short)port);
#ifdef HAS_GETADDRINFO
	res = 0;
	memset((char*)&hints, 0, sizeof(hints));
	hints.ai_family = PF_INET;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_protocol = 0;
	if((err = getaddrinfo(host, 0, &hints, &res)) != 0)
		return (-1);

	resaddr = (struct sockaddr_in*)res->ai_addr;
	srvaddr.sin_addr = resaddr->sin_addr;
	freeaddrinfo(res);
#else
	if((inaddr = inet_addr(host)) != INADDR_NONE)
		memcpy((char*)&srvaddr.sin_addr, (char*)&inaddr, sizeof(inaddr));
	else
	{
		if((hp = gethostbyname(host)) == NULL)
			return (-1);
		if(hp->h_addrtype != AF_INET)
			return (-1);
		memcpy((char*)&srvaddr.sin_addr, (char*)hp->h_addr, hp->h_length);
	}
#endif	/* HAS_GETADDRINFO */
	return connect(sd, (struct sockaddr*)&srvaddr, sizeof(srvaddr));
}

int
tcplisten(SOCKET sd, int port, int backlog)
{
	struct sockaddr_in addr;

	memset((char*)&addr, 0, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_addr.s_addr = htonl(INADDR_ANY);
	addr.sin_port = htons((u_short)port);

	if(bind(sd, (const struct sockaddr*)&addr, sizeof(addr)) == -1)
		return (-1);

	return listen(sd, backlog);
}

int
tcpclose(SOCKET sd)
{
#ifdef WIN32
	return closesocket(sd);
#else
	return close(sd);
#endif	/* WIN32 */
}

int
tcpread(SOCKET sd, char* buf, int nbytes)
{
	int nleft, nrecv;

	nleft = nbytes;
	while(nleft > 0)
	{
		nrecv = recv(sd, buf, nleft, 0);
		if(nrecv < 0)
			return nrecv;
		else if(nrecv == 0)
			break;
		nleft -= nrecv;
		buf += nrecv;
	}
	return (nbytes - nleft);
}

int
tcpwrite(SOCKET sd, const char* buf, int nbytes)
{
	int nleft, nsent;

	nleft = nbytes;
	while(nleft > 0)
	{
		nsent = send(sd, buf, nleft, 0);
		if(nsent <= 0)
			return nsent;
		nleft -= nsent;
		buf += nsent;
	}
	return (nbytes - nleft);
}

int
netlibstart(void)
{
#ifdef WIN32
	WORD wVersion;
	WSADATA wsaData;
	wVersion = MAKEWORD(2,2);
	if(WSAStartup( wVersion, &wsaData ))
		return (-1);
#endif	/* WIN32 */
	return 0;
}

int
netlibstop(void)
{
#ifdef WIN32
	if(WSACleanup())
		return (-1);
#endif	/* WIN32 */
	return 0;
}
