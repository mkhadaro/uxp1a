#include "../include/client.h"

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

void client::simplefs_open(char* name,int mode)
{

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

}
void client::simplefs_lseek(int fd,int whence,int len)
{

}
