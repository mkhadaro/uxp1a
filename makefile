CC = g++
CFLAGS  = -g -Wall

all: main.cpp main.o server.o server_actions.o client.o
	$(CC) $(CFLAGS) main.o server.o server_actions.o client.o -o program
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o
server.o: src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp -c -o server.o
server_actions.o: src/server_actions.cpp
	$(CC) $(CFLAGS) src/server_actions.cpp -c -o server_actions.o
client.o: src/client.cpp
	$(CC) $(CFLAGS) src/client.cpp -c -o client.o
clean:
	rm -f *.o

