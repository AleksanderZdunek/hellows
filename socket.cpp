#include "socket.h"
#include <iostream>
#include <stdexcept>
#include <cstring>

#if defined __windows__
void Socket::WinInit()
{
	std::cout << "Inside WinInit" << std::endl;
	WSADATA wsaData;

	int iResult;

	//Initialize Winsock
	iResult = WSAStartup(MAKEWORD(2, 2), &wsaData);
	if (iResult != 0)
	{
		std::cout << "WSAStartup failed: " << iResult << std::endl;
		throw std::runtime_error("WSAStartup failed");
	}
}

void Socket::WinCleanup()
{
	WSACleanup();
}
#endif

Socket::Socket(const char* const ipAddress, unsigned short port)
{

	#if defined __windows__
		WinInit();
	#endif

    /*Create a reliable stream socket using TCP*/
	//if( (socketHandle = socket(PF_INET, SOCK_STREAM, IPPROTO_TCP)) < 0 )
    if( (socketHandle = socket(PF_INET, SOCK_STREAM, IPPROTO_IP)) < 0 )
	{
	    std::cout << "Server Socket / Socket Error on Create" << std::endl;
		throw ConstructorFailedException();
	}

    /*Construct the remote-address structure*/
    memset(&addr, 0, sizeof(addr));
    addr.sin_family = AF_INET; /*Internet address family*/
    addr.sin_addr.s_addr = inet_addr(ipAddress);
    addr.sin_port = htons(port);
}

Socket::Socket(int handle, const sockaddr_in& addr) : socketHandle(handle), addr(addr)
{
}

Socket::~Socket()
{
	//#if defined __windows__
	//WinCleanup(); //fix me
	//#endif
}

bool Socket::IsOpen() const
{
    return socketHandle != -1;
}

void Socket::Close()
{
    if(IsOpen())
    {
        #if defined __linux__
        close(socketHandle);
		#elif defined __windows__
		closesocket(socketHandle);
        #endif
		socketHandle = -1;
    }
}

int Socket::GetInternalHandle() const
{
    return socketHandle;
}

int Socket::Send(const void* const msg, unsigned int msgLength)
{
	return send(socketHandle, (char*) msg, msgLength, DefaultBehavior);
}

int Socket::Receive(void* receiveBuffer, unsigned int bufferLength, int flags)
{
	return recv(socketHandle, (char*) receiveBuffer, bufferLength, flags);
}

ClientSocket::ClientSocket() : Socket(-1, sockaddr_in())
{
}

ClientSocket::ClientSocket(int handle, const sockaddr_in& addr) : Socket(handle, addr)
{
}

ClientSocket::~ClientSocket()
{
}

ServerSocket::ServerSocket(unsigned short serverPort) : Socket("127.0.0.1", serverPort)
{
    addr.sin_addr.s_addr = htonl(INADDR_ANY);

    int value = 1; //Reuse address
    if (setsockopt(socketHandle, SOL_SOCKET, SO_REUSEADDR, (const char*) &value, sizeof(value)) == -1)
    {
        std::cout << "Server Socket Error. SetSockOpt" << std::endl;
        throw ConstructorFailedException();
    }

    if (!IsOpen())
    {
        std::cout << "Server Socket Error. Socket not open" << std::endl;
        throw ConstructorFailedException();
    }

    //Bind socket to local address
    if( bind(socketHandle, (struct sockaddr*) &addr, sizeof(addr)) < 0 )
    {
        std::cout << "Server Socket Error on Bind" << std::endl;
        throw ConstructorFailedException();
    }

    //Mark the socket so it will listen for incoming connections
    const int maxPending = 5;
    if( listen(socketHandle, maxPending) < 0 )
    {
        std::cout << "Server Socket Error on Listen" << std::endl;
        throw ConstructorFailedException();
    }
}

ClientSocket ServerSocket::Accept()
{
    int clientSocketHandle;
    struct sockaddr_in clientAddr;
    int clientAddrLength = sizeof (clientAddr);

    #if defined __linux__
    clientSocketHandle = accept(socketHandle, (struct sockaddr *) &clientAddr, (socklen_t *)&clientAddrLength);
	#elif defined __windows__
	clientSocketHandle = accept(socketHandle, (struct sockaddr*) &clientAddr, &clientAddrLength);
	#endif
    if ( clientSocketHandle < 0 )
    {
        std::cout << "Server Socket Accept Error" << std::endl;
        throw std::exception();
    }
    return ClientSocket(clientSocketHandle, clientAddr);
}
