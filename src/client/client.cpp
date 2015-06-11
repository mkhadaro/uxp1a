#include "../../include/client.h"

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
