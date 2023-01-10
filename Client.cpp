#include <iostream>
#include <string.h>

#include "Socket.hpp"

static void usage();

int main(int argc, const char *argv[])
{
	try
		{
			if (argc > 1 && *(argv[1]) == '-')
				{
					usage();
					return EXIT_FAILURE;
				}

			// init local variables
			int res = -1;
			const char* serverAddr = "127.0.0.1";
			short serverPort = 1234;
			
			// look for input arguments by the user 
			if (argc > 1)
        serverAddr = argv[1];

			if (argc >= 3)
        serverPort = (short) atoi(argv[2]);

			// Create a TCP socket
			auto sock = ged::Socket::Create(true);
			sock->init();
      
			// Print a resolved address of server (the first IP of the host)
			std::cout << "server address = "
								<< serverAddr
								<< ", port "
								<< static_cast<int>(serverPort)
								<< std::endl;

			// Connect to the server
			sock->Connect(serverAddr, serverPort);
			std::cout << "Connected. Reading a server message" << std::endl;

			// Read from server
			const int bufferLen = 1023;
			char buffer[bufferLen + 1];
			res = sock->Read(buffer, bufferLen);

			buffer[res] = 0;
			std::cout << "Received:" << std::endl << buffer << std::endl;

			// Write to server
			const char reply[] = "Thanks! Bye-bye...\r\n";
			int replyLen = (int)strlen(reply);
			res = sock->Write(reply, replyLen);

			return EXIT_SUCCESS;

		} catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
		return EXIT_FAILURE;
	}
}

static void usage()
{
	std::cout << "A simple Internet client application.\n"
						<< "Usage:\n"
						<< "         client [IP_address_of_server [port_of_server]]\n"
						<< "     where IP_address_of_server is the IP number of server\n"
						<< "     port_of_server is a port number, default is 1234.\n"
						<< "The client connects to a server which address is given in a\n"
						<< "command line, receives a message from a server, sends the message\n"
						<< "\"Thanks! Bye-bye...\", and terminates.\n";
}
