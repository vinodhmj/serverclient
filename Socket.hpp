/**
 *   \file Socket.hpp
 *   \brief RAII Wrapper for Socket used in Server-Client communication
 *
 *   This header file contains the declaration of both the abstract base class
 *   Socket and its children TCPsocket and UDPSocket types.
 *   The UML follows a Factory pattern with a static Create function at the base class and Factory entry interface.
 *
 *   \copyright none
 *   \remark 
 }*/
// --------------------------------------------------
#ifdef SUPPORTS_PRAGMA_ONCE
#pragma once
#endif

#ifndef SOCKET_GED_H
#define SOCKET_GED_H

#include <memory>
#include <sys/socket.h>

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

		bool init(void);
		void setSocketHandler(int newHandler);

		static const int INVALID_SOCKET_HANDLER = -1;
	protected:
		int socketHandler = INVALID_SOCKET_HANDLER;
		short type;
	}; // end of class Socket

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
	}; // end of class TCPSocket
	
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
		std::unique_ptr<Socket> Accept() { return nullptr; }
	}; // End of class UDPSocket
} // ged
#endif /* SOCKET_GED_H */
