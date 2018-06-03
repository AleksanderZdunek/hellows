#pragma once

#if defined __linux__
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <unistd.h>
#include <arpa/inet.h>
#endif

#include <exception>

enum SocketFlags
{
	DefaultBehavior = 0
};

class Socket
{
public:
    Socket(const char* const ipAddress, unsigned short port);
    ~Socket();

    bool IsOpen() const;
    void Close();

    int GetInternalHandle() const;

    int Send(const void* const msg, unsigned int msgLength);
    int Receive(void* const receiveBuffer, unsigned int bufferLength, int flags=DefaultBehavior);

protected:
    /**
    Creates an inert dummy shell Socket.
    Can be used to create copies of open sockets.
    */
    Socket(int handle, const sockaddr_in& addr);
    int socketHandle;
    sockaddr_in addr;
};

class ClientSocket : public Socket
{
    friend class ServerSocket;
public:
    ClientSocket();
    ~ClientSocket();
private:
    ClientSocket(int handle, const sockaddr_in& addr);
};

class ServerSocket : public Socket
{
public:
    ServerSocket(unsigned short serverPort);
    ClientSocket Accept();
};

class ConstructorFailedException : public std::exception
{
	virtual const char* what() const throw()
	{
		return "Constructor Failed Exception";
	}
};
