#include "Socket.hpp"
#include <exception>

static void usage();

int main(int argc, char *argv[])
{
	try {
    if (argc > 1 && *(argv[1]) == '-')
    {
        usage();
        return EXIT_FAILURE;
    }

		int res = -1;
    int listenPort = 1234;
    if (argc > 1)
        listenPort = atoi(argv[1]);

		// Create a TCP socket
		auto sock = ged::Socket::Create(true);
    sock->init();
		sock->Bind("", listenPort);
		auto handler = sock->Accept();
		const char request[] = "Hello!\r\n";
    int requestLen = (int)strlen(request);
		res = handler->Write(request, requestLen);
		const int bufferLen = 1023;
    char buffer[bufferLen + 1];
    res = handler->Read(buffer, bufferLen);

		buffer[res] = 0;
		std::cout << "Received " << res << " bytes:" << std::endl << buffer << std::endl;

    return EXIT_SUCCESS;

        } catch (std::exception& ex) {
		std::cerr << ex.what() << std::endl;
        return EXIT_FAILURE;
	}
} // end of main

static void usage() {
    std::cout << "A simple Internet server application.\n"
              << "It listens to the port written in command line (default 1234),\n"
              << "accepts a connection, and sends the \"Hello!\" message to a client.\n"
              << "Then it receives the answer from a client and terminates.\n\n"
              << "Usage:\n"
              << "     server [port_to_listen]\n"
              << "Default is the port 1234.\n";
}
