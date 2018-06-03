TARGET = hellows
SOURCE = main.cpp

VER = -std=c++11
CFLAGS = $(VER) -Wall -O3

$(TARGET): $(SOURCE) socket.o
	g++ $(CFLAGS) -o $(TARGET) $(SOURCE) socket.o

socket.o: socket.cpp socket.h
	g++ $(CFLAGS) -o $@ -c $<

clean:
	rm -f $(TARGET) *.o
