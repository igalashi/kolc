#
#
#
CC = gcc
CFLAGS = -Wall -g -O
CLIBS =

CXX = g++
CXXFLAGS = -Wall -g -O
CXXLIBS = -L. -lkol

INCLUDES =

EXES = exam01 exam02 exam03

all: tcplib.o $(EXES)

#libkol.a: koltcp.o kolsocket.o koluri.o kolthread.o
#	ar r $@ $^

exam01: exam01.c tcplib.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

exam02: exam02.c tcplib.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

exam03: exam03.c tcplib.o
	$(CC) $(CFLAGS) -o $@ $^ $(CLIBS)

clean:
	rm -f *.o *.a
	rm -f $(EXES)

.cpp.o:
	$(CXX) $(CXXFLAGS) -c $< -o $@

.c.o:
	$(CC) $(CFLAGS) -c $< -o $@
