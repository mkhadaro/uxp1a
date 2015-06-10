#include "../include/client.h"
#include <iostream>
#include <string>


#include <string.h>

client::client()
{
	char cpid[8];
	strcpy(clientFifoId, CLIENT_FIFO);
	sprintf(cpid, "%ld", (long)getpid());
	strcat(clientFifoId, cpid);
	mkfifo(clientFifoId, 0666);
}

client::~client()
{

}

serverResponse client::sendRequest(int type, char path[128], int_l inodeNumber, int_l size, int mode)
{
	clientRequest req;
	strcpy(req.clientFifoId, clientFifoId);
	req.type = type;
	strcpy(req.path, path);
	req.inodeNumber = inodeNumber;
	req.size = size;
	req.mode = mode;

	int serverFd = open(SERVER_FIFO, O_WRONLY);
	write(serverFd, &req, sizeof(req));
	close(serverFd);

	serverResponse res;
	int clientFd = open(clientFifoId, O_RDONLY);
	read(clientFd, &res, sizeof(res));
	close(clientFd);

	// TODO do debugowania
	printf("%s\n", res.clientFifoId);

	if(strcpy(res.clientFifoId, clientFifoId) != 0 || res.type != type)
		res.result = -5;

	return res;
}

int client::userInterface()
{
    std::string polecenie;
    std::cin>>polecenie;

    if (polecenie == "help")
    {
        std::cout << "mkdir" << std::endl;
        std::cout << "open" << std::endl;
        std::cout << "unlink" << std::endl;
        std::cout << "creat" << std::endl;
        std::cout << "read" << std::endl;
        std::cout << "write" << std::endl;
        std::cout << "lseek" << std::endl;
        std::cout << "koniec - k" << std::endl;
    }
    else if (polecenie == "mkdir")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "open")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "unlink")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "creat")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "read")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "write")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "lseek")
    {
        std::cout<< "poprawne polecenie" << std::endl;
    }
    else if (polecenie == "k") return 1;
    else std::cout<<"NIE MA TAKIEGO POLECENIA.\n" << std::endl;
    return 0;
}

void client::simplefs_open(char* name,int mode)
{
    //
}
void client::simplefs_unlink(char* name)
{

}
void client::simplefs_mkdir(char* name)
{

}
void client::simplefs_create(char* name,int mode)
{

}
void client::simplefs_read(int fd,char* buf,int len)
{

}
void client::simplefs_write(int fd,char* buf,int len)
{
    //TODO
    //wyslanie wiadomosci do serwera


    int fd = simplefs_open(name, WRITE);
    int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0);
    shared_memory = attachSegmentOfSharedMemory();

    int written_bytes = write(fd, buffer, len);
    if (written_bytes == -1)
    {
        printf("Nie zapisano");
    }
    close(fd);
    return written_bytes;



}
void client::simplefs_lseek(int fd,int whence,int offset)
{

}
