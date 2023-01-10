/**
 *   \file Socket.hpp
 *   \brief RAII Wrapper for Socket used in Server-Client communication
 *
 *
 *
 *   \copyright
 *   \remark
 }*/
// --------------------------------------------------
#ifdef SUPPORTS_PRAGMA_ONCE
#pragma once
#endif

#ifndef SOCKET_GED_H
#define SOCKET_GED_H

#include <iostream>
#include <errno.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netinet/ip.h> 
#include <arpa/inet.h>
#include <memory>
namespace ged {
class Socket
{
public:
  Socket() = default;
	Socket(short _type) : type(_type){}
	Socket(const Socket &other) = default;
  Socket(Socket &&other) noexcept = default;
  Socket& operator=(const Socket &other) = default;
	Socket& operator=(Socket &&other) noexcept = default;

	virtual ~Socket() noexcept;

  static std::unique_ptr<Socket> Create(bool isTCP=true);
		
	virtual bool Connect(const char *ipAddr, short ipPort) = 0;
	virtual bool Bind(const char *ipAddr, short ipPort) = 0;
	virtual std::unique_ptr<Socket> Accept() = 0;
	
	int Read(char *rdBuffer, int rdBufferSize);
	int Write(const char *wrBuffer, int numBytesToSend);

	std::string usage(void) { return "hello";}
	bool init(void);
	void setSocketHandler(int newHandler);

	static const int INVALID_SOCKET_HANDLER = -1;
protected:
	int socketHandler = INVALID_SOCKET_HANDLER;
	short type;
};

class TCPSocket : public Socket
{
public:
	TCPSocket() : Socket(SOCK_STREAM) {}
	TCPSocket(const TCPSocket &other) = default;
	TCPSocket(TCPSocket &&other) noexcept = default;
	TCPSocket& operator=(const TCPSocket &other) = default;
	TCPSocket& operator=(TCPSocket &&other) noexcept = default;

	virtual ~TCPSocket() noexcept = default;
	
	bool Connect(const char *ipAddr, short ipPort);
	bool Bind(const char *ipAddr, short ipPort);
	std::unique_ptr<Socket> Accept();
};
	
class UDPSocket : public Socket
{
public:
	UDPSocket() : Socket(SOCK_DGRAM) {}
  UDPSocket(const UDPSocket &other) = default;
  UDPSocket(UDPSocket &&other) noexcept = default;
	UDPSocket& operator=(const UDPSocket &other) = default;
	UDPSocket& operator=(UDPSocket &&other) noexcept = default;
	virtual ~UDPSocket() noexcept = default;
	
	bool Connect(const char *ipAddr, short ipPort){return false;}
	bool Bind(const char *ipAddr, short ipPort) {return false;}
	std::unique_ptr<Socket> Accept() { std::unique_ptr<Socket> derived(new UDPSocket); return derived;}
};
}  // ged
#endif /* SOCKET_GED_H */
