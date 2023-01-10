/**
 * This source file contains the implementation for the abstract base class
 Socket and its children TCPSocket and UDPSocket.

 TODO: Implement concrete function for UDP type connection
 */

#include <iostream>
#include <stdexcept>
#include <string.h>
#include <errno.h>
#include <stdlib.h>
#include <unistd.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <arpa/inet.h>

#include "Socket.hpp"

namespace ged
{
/**
 *  \brief Factory Pattern Create for Sockets
 *
 *  use the inArg to select the type of connection
 *
 *  \param isTCP default TCP type
 *  \return unique pointer of Socket base class
 */
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
	// Release the resource after use or when going out of scope
	close(socketHandler);
}

/**
 *  \brief initialize the Socket by acquiring the resource
 *
 *  
 *
 *  \param param
 *  \return status bool
 */	
bool Socket::init(void) {
    int res= socket(AF_INET, type, 0);
    if (res < 0)
    {
			throw std::runtime_error("Error creating socket - " + std::string(strerror(errno)));
    }
		socketHandler = res;
		return true;
}

/**
 *  \brief Wrapper for socket write
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */	
int	Socket::Write(const char *request, int requestLen)
{
	int res = write(socketHandler, request, requestLen);
	if (res != requestLen)
    {
			throw std::runtime_error("Error writing to client - " + std::string(strerror(errno)));
    }
	return res;
}

/**
 *  \brief Wrapper for socket read
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */	
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

/**
 *  \brief Wrapper for socket bind
 *
 * Used only in Server call
 *
 *  \param ipAddr is only used on need basis, ususally ignored
 *  \return return type
 */
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

/**
 *  \brief Wrapper for socket accept
 *
 *  Detailed description
 *
 *  \param param
 *  \return return type
 */	
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

    return sNew;
}

/**
 *  \brief Wrapper for socket connect
 *
 *  Only used from Client side
 *
 */
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
