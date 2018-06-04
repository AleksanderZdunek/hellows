#include <iostream>
#include "socket.h"
#include <cstdio> //sscanf()
#include <string>

const char html[] = "<!DOCTYPE html><html><head><title>Hello World</title></head><body><h1>Hello World Wide Web!</h1><p>served by hellows web server<br />(c) Aleksander Zdunek</p></body></html>";
const char msg[] = "HTTP/1.1 200 OK\nContent-Length: 173\nContent-Type: text/html\r\n<!DOCTYPE html><html><head><title>Hello World</title></head><body><h1>Hello World Wide Web!</h1><p>served by hellows web server<br />(c) Aleksander Zdunek</p></body></html>\r\n";
//const char response[] = "HTTP/1.1 200 OK";
//const char content_type[] = "Content-Type: text/html";
//const char content-length[] = "Content-Length: 175"; 

int serverPort = 80;

int main(int argc, char* argv[])
{
    //std::cout << "sizeof(html): " << sizeof(html) << std::endl;
	std::cout << "Hello World Web Server" << std::endl;
  
	if(1 < argc) sscanf(argv[1], "%d", &serverPort);

  	std::cout << "listening on port " << serverPort << std::endl; 
	ServerSocket connectionListener(serverPort);

	unsigned int count(0);
	for(;;)
	{
		ClientSocket remote = connectionListener.Accept();
		
		std::cout << "Meep! " << count++ << "\n";

		char databuffer[512]{0};
		remote.Receive(databuffer, sizeof(databuffer));
		std::cout << remote.Send(msg, sizeof(msg)-1) << "\n"; //-1 because don't send null terminator
		remote.Close();

		std::cout << "----\n" << databuffer << "----\n";

		std::cout << "Beep!" << std::endl;
	}
	std::cout << "Goodbye\n";

	return 0;
}
