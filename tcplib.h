/* tcplib.h */

#ifndef TCPLIB_H_INCLUDED
#define TCPLIB_H_INCLUDED

#ifdef WIN32
#include <winsock2.h>
#include <ws2tcpip.h>
#if (_WIN32_WINNT >= 0x0501)
#define HAS_GETADDRINFO
#endif	/* (_WIN32_WINNT > 0x0501) */
#else
#include <unistd.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
typedef int SOCKET;
#define INVALID_SOCKET (-1)
#define HAS_GETADDRINFO
#endif	/* WIN32 */

/* Function prototypes */
int netlibstart(void);
int netlibstop(void);

SOCKET tcpsocket(void);
int tcpopen(SOCKET sd, const char* host, int port);
int tcplisten(SOCKET sd, int port, int backlog);
int tcpclose(SOCKET sd);
int tcpread(SOCKET sd, char* buf, int nbytes);
int tcpwrite(SOCKET sd, const char* buf, int nbytes);

#endif	/* TCPLIB_H_INCLUDED */
