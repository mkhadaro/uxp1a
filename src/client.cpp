#include "../include/client.h"
#include "../include/server.h"
#include <iostream>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>

client::client()
{
	char cpid[8];
	strcpy(clientFifoId, CLIENT_FIFO);
	sprintf(cpid, "%ld", (long)getpid());
	strcat(clientFifoId, cpid);
	mkfifo(clientFifoId, 0666);
	fs = attachSegmentOfSharedMemory();
	printf("client fifo id %s\n", clientFifoId);
}

client::~client()
{
	detachSegmentOfSharedMemory(fs);
	unlink(clientFifoId);
}

serverResponse client::sendRequest(int type, char *path, int fd, int_l size, int mode)
{
	clientRequest req;
	strcpy(req.clientFifoId, clientFifoId);

	req.type = type;
	strcpy(req.path, path);
	req.fd = fd;
	req.size = size;
	req.mode = mode;

	int serverFd = open(SERVER_FIFO, O_WRONLY);
	write(serverFd, &req, sizeof(req));
	close(serverFd);

	serverResponse res;
	int clientFd = open(clientFifoId, O_RDONLY);
	read(clientFd, &res, sizeof(res));
	close(clientFd);

	printf("%s\n", res.clientFifoId);

	if(strcpy(res.clientFifoId, clientFifoId) != 0 || res.type != type)
		res.result = -5;

	return res;
}

void client::simplefs_open(char* path,int mode)
{
    serverResponse response = sendRequest(OPEN_ACT, path, 0, 0, mode);
    if(response.result < 0)
        fileDescription = -1;
    else
        fileDescription = response.fd;
}

void client::simplefs_unlink(char* name)
{
    serverResponse response = sendRequest(UNLINK_ACT, name, 0, 0, 0);
    if(response.result < 0)
        printf("Blad pzry usuwaniu pliku");
}

void client::simplefs_mkdir(char* path)
{
	serverResponse response = sendRequest(MKDIR_ACT, path, 0, 0, 0);
    if(response.result == -1)
        printf("Blad przytworzeniu katalogu");
}

void client::simplefs_create(char* name,int r, int w, int x)
{
    serverResponse response = sendRequest(CREATE_ACT, name, r, w, x);
    if(response.result < 0)
        printf("Blad przytworzeniu pliku");
}

void client::simplefs_read(int fd,char* buf,int len)
{
    serverResponse response = sendRequest(READ_ACT, "", fd, len, READ);
    if(response.result < 0)
        return;
    for(int i = 0;i < len; ++i)
        buf[i] = fs->dataBlocks[response.result + i ];

    sendRequest(CLOSE, "", fd, 0, 0);
}

void client::simplefs_write(int fd,char* buf,int len)
{
    serverResponse response = sendRequest(WRITE_ACT, "", fd, len, WRITE);
    if(response.result < 0)
        return;
    for(int i = 0;i < strlen(buf); ++i)
        fs->dataBlocks[response.result + i ] = buf[i];

    sendRequest(CLOSE, "", fd, 0, 0);
}

void client::simplefs_lseek(int fd,int whence,int offset)
{
    serverResponse response = sendRequest(LSEEK_ACT, "", fd, offset, whence);
}

void client::close_file(int fd)
{
    serverResponse response = sendRequest(CLOSE, "", fd, 0, 0);
}

void client::simplefs_list(char* path)
{
	for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(fs->inodes[i].type != -1)
        {
            std::cout<<fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(&fs->inodes[i]);
            std::cout<<std::endl;
        }
    }
    std::cout<<std::endl;
}

void client::showFiles(INode *inode)
{
    for(int j =0; j < sizeof(inode->pointers)/sizeof(int); ++j)
    {
        std::cout<<inode->pointers[j]<<" ";
        //if(inode->pointers[j] != 0)
            //std::cout<<"name "<<server.fs->inodes[inode->pointers[j]].name<<" ";
        if(fs->inodes[inode->pointers[j]].type == TYPE_HELPER)
        {
                std::cout<<"\n\t"<<"\t";
                showFiles(&fs->inodes[inode->pointers[j]]);
        }
    }
}

FileSystem* client::attachSegmentOfSharedMemory()
{
    //pobieramy id pamieci wspoldzielonej
    int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0);
    if(pamiec_id == -1)
    {
        printf("Blad przy uzyskaniu id pamieci, MEMORY_KEY:%d\n", MEMORY_KEY);
        return 0;
    }
    //podlaczamy sie do segmentu pamieci wspoldzielonej
    FileSystem* shared_memory;
    shared_memory = (FileSystem* ) shmat (pamiec_id, 0, 0);
      //podlaczamy wspoldzielona pamiec do przestrzeni adresowej naszego procesu
    if(shared_memory == (void *) -1)
    {
        printf("Brak dostępu do pamięci współdzielonej\n");
        return 0;
    }
    return shared_memory;
}

void client::detachSegmentOfSharedMemory(FileSystem* shared_memory)
{
    //odlaczamy segment wspoldzelonej pamięci
    if(shmdt(shared_memory) == -1)
    {
        printf("Nieudane odłączenie pamięci współdzielonej\n");
        return;
    }
}
