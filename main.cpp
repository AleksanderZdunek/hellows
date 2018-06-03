#include <iostream>
#include "socket.h"

const char html[] = "<!DOCTYPE html><html><head><title>Hello World</title></head><body><h1>Hello World Wide Web!</h1><p>served by hellows web server<br />(c) Aleksander Zdunek</p></body></html>";

int main(int argc, char* argv[])
{
  std::cout << "Hello World Web Server" << std::endl;

  unsigned int count(0);

  ServerSocket connectionListener(80);

  for(;;)
  {
      ClientSocket remote = connectionListener.Accept();

      std::cout << "Meep! " << count++ << "\n";

      char databuffer[512]{0};
      remote.Receive(databuffer, sizeof(databuffer));
      remote.Send(html, sizeof(html));
      remote.Close();

      std::cout << "----\n" << databuffer << "----\n";

      std::cout << "Beep!" << std::endl;
  }
  std::cout << "Goodbye\n";

  return 0;
}
