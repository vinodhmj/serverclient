#include "Socket.hpp"
#include <stdexcept>
namespace ged
{    

std::unique_ptr<Socket> Socket::Create(bool isTCP) {
	if(isTCP)
		{
			std::unique_ptr<Socket> tcp(new TCPSocket);
			return tcp;
		}
	else
		{
			std::unique_ptr<Socket> udp(new UDPSocket);
			return udp;
		}
}

Socket::~Socket()
{
	close(socketHandler);
}
	
bool Socket::init(void) {
    int res= socket(AF_INET, type, 0);
    if (res < 0)
    {
			throw std::runtime_error("Error creating socket - " + std::string(strerror(errno)));
    }
		socketHandler = res;
		return true;
}

int	Socket::Write(const char *request, int requestLen)
{
	int res = write(socketHandler, request, requestLen);
	if (res != requestLen)
    {
			throw std::runtime_error("Error writing to client - " + std::string(strerror(errno)));
    }
	return res;
}

int Socket::Read(char *buffer, int bufferLen)
	{
    int res = read(socketHandler, buffer, bufferLen);
    if (res < 0)
    {
			throw std::runtime_error("Error reading from client - " + std::string(strerror(errno)));
    }

		return res;
}
	
void Socket::setSocketHandler(int newHandler)
{
	close(socketHandler);
	socketHandler = newHandler;
}
	
bool TCPSocket::Bind(const char *ipAddr, short listenPort)
{
	// Fill in the address structure containing self address
	struct sockaddr_in listenAddrIn;
	memset(&listenAddrIn, 0, sizeof(struct sockaddr_in));
	listenAddrIn.sin_family = AF_INET;
	listenAddrIn.sin_addr.s_addr = htonl(INADDR_ANY);
	listenAddrIn.sin_port = htons(listenPort);        // Port to listen
 
	// Bind a socket to the address
	int res = bind(socketHandler, (struct sockaddr*) &listenAddrIn, sizeof(listenAddrIn));
	if (res < 0)
    {
			throw std::runtime_error("Error binding socket to server address -" + std::string(strerror(errno)));
    }
 
	// Set the "LINGER" timeout to zero, to close the listen socket
	// immediately at program termination.
	struct linger linger_opt = { 1, 0 }; // Linger active, timeout 0
	setsockopt(socketHandler, SOL_SOCKET, SO_LINGER, &linger_opt, sizeof(linger_opt));

	   // Now, listen for a connection
	res = listen(socketHandler, 1);    // "1" is the maximal length of the queue
	if (res < 0)
    {
			throw std::runtime_error("Error listening for clients - " + std::string(strerror(errno)));
    }
 
	return true;
}

std::unique_ptr<Socket> TCPSocket::Accept()
{
	std::cout << "Waiting for client..." << std::endl;
	
	  // Accept a connection (the "accept" command waits for a connection with
    // no timeout limit...)
    struct sockaddr_in clientAddr;
    socklen_t clientAddr_len = sizeof(clientAddr);
    int s1 = accept(socketHandler, (struct sockaddr*) &clientAddr, &clientAddr_len);
    if (s1 < 0)
    {
    	close(socketHandler);
			throw std::runtime_error("Error while accepting clients - " + std::string(strerror(errno)));
    }
    std::unique_ptr<Socket> sNew(new TCPSocket);
		sNew->setSocketHandler(s1);
		
		// A connection is accepted. The new socket "s1" is created
    // for data input/output. The clientAddr structure is filled in with
    // the address of connected entity, print it.
    char ip[INET_ADDRSTRLEN];
    inet_ntop(AF_INET, &clientAddr.sin_addr, ip, INET_ADDRSTRLEN);
    std::cout << "Connection from IP "
              << ip
              << " Port "
              << ntohs(clientAddr.sin_port)
			  << std::endl;
 
		close(socketHandler);    // Close the listen socket

		return sNew;
}

bool TCPSocket::Connect(const char *serverAddr, short serverPort)
{
	// Fill in server IP address
	struct sockaddr_in serverAddrIn;
	serverAddrIn.sin_family = AF_INET;
	inet_pton(AF_INET, serverAddr, &serverAddrIn.sin_addr);
	serverAddrIn.sin_port = htons(serverPort);

	// Connect to the remote server
	int res = connect(socketHandler, (struct sockaddr*) &serverAddrIn, sizeof(serverAddrIn));
	if (res < 0)
    {
      throw std::runtime_error("Error connecting to server - " + std::string(strerror(errno)));
    }

	return true;
}


}// namespace
