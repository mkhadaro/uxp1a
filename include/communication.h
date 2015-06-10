#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "shared_memory.h"

#define SERVER_FIFO "tmp/server"
#define CLIENT_FIFO "tmp/client_"

#define OPEN_ACT 1
#define UNLINK_ACT 2
#define MKDIR_ACT 3
#define CREATE_ACT 4
#define READ_ACT 5
#define WRITE_ACT 6
#define LSEEK_ACT 7
#define CLOSE 8

typedef struct
{
	char clientFifoId[24];
	int type;
	char path[128]; //dla create
	int fd;         //r
	int_l size;     //w
	int mode;       //x
} clientRequest;

typedef struct
{
	char clientFifoId[24];
	int type;
	int result;//-1 w przypadku bledu lub przy readzie ,write - nr bloku do ktorego piszemy lub z ktorego czytamy
	int fd;//deskryptor pliku po odpowiedzi
} serverResponse;

#endif
