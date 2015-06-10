#ifndef COMMUNICATION_H
#define COMMUNICATION_H

#include "shared_memory.h"

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
