CC = g++
CFLAGS  = -g -Wall

all: main.cpp main.o server.o server_actions.o server_api.o client.o interface.o output.o
	$(CC) $(CFLAGS) main.o server.o server_actions.o server_api.o client.o interface.o output.o -o program
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o
server.o: src/server.cpp
	$(CC) $(CFLAGS) src/server.cpp -c -o server.o
server_actions.o: src/server_actions.cpp
	$(CC) $(CFLAGS) src/server_actions.cpp -c -o server_actions.o
server_api.o: src/server_actions.cpp
	$(CC) $(CFLAGS) src/server_api.cpp -c -o server_api.o
client.o: src/client.cpp
	$(CC) $(CFLAGS) src/client.cpp -c -o client.o
interface.o: src/interface.cpp
	$(CC) $(CFLAGS) src/interface.cpp -c -o interface.o
output.o: src/output.cpp
	$(CC) $(CFALGS) src/output.cpp -c -o output.o
clean:
	rm -f *.o

