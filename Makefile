.PHONY: all clean

CC = g++

%.o: %.cpp
	$(CC) -Wall -O0 -g -c -o $@ $<
	
Socket.o: Socket.cpp Socket.hpp
	$(CC) -Wall -O0 -g -c -o $@ $<
	
Server: Server.o Socket.o
	$(CC) -Wall -O0 -g -o $@ $^
	
Client: Client.o Socket.o
	$(CC) -Wall -O0 -g -o $@ $^

#test: test.o Socket.o						#
#$(CC) -Wall -O0 -g -o $@ $^

all: Server Client test
	
clean:
	rm -f Client Server *.o
