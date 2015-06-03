CC = g++
CFLAGS  = -g -Wall

program: main.cpp
	$(CC) $(CFLAGS) -o program main.cpp src/server.cpp src/client.cpp
clean:
	rm -f *.o

