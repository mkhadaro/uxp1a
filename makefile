CC = g++
CFLAGS  = -g -Wall

all: main.cpp main.o server.o server_actions.o server_api.o server_communication.o client.o client_api_fs.o interface.o output.o tests.o
	$(CC) $(CFLAGS) main.o server.o server_actions.o server_api.o server_communication.o client.o client_api_fs.o interface.o output.o tests.o -o program
main.o: main.cpp
	$(CC) $(CFLAGS) main.cpp -c -o main.o
server.o: src/server/server.cpp
	$(CC) $(CFLAGS) src/server/server.cpp -c -o server.o
server_actions.o: src/server/server_actions.cpp
	$(CC) $(CFLAGS) src/server/server_actions.cpp -c -o server_actions.o
server_api.o: src/server/server_actions.cpp
	$(CC) $(CFLAGS) src/server/server_api.cpp -c -o server_api.o
server_communication.o: src/server/server_communication.cpp
	$(CC) $(CFLAGS) src/server/server_communication.cpp -c -o server_communication.o
client.o: src/client/client.cpp
	$(CC) $(CFLAGS) src/client/client.cpp -c -o client.o
client_api_fs.o: src/client/client_api_fs.cpp
	$(CC) $(CFLAGS) src/client/client_api_fs.cpp -c -o client_api_fs.o
interface.o: src/client/interface.cpp
	$(CC) $(CFLAGS) src/client/interface.cpp -c -o interface.o
output.o: src/output.cpp
	$(CC) $(CFALGS) src/output.cpp -c -o output.o
tests.o: src/tests.cpp
	$(CC) $(CFLAGS) src/tests.cpp -c -o tests.o
server_communication.o: src/server/server_communication.cpp
	$(CC) $(CFLAGS) src/server/server_communication.cpp -c -o server_communication.o
clean:
	rm -f *.o

