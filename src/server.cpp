#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/ipc.h>
#include <sys/shm.h>
#include "../include/server.h"


server::server()
{
    int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0777 | IPC_CREAT | IPC_EXCL);
    //alokujemy pamiec wspoldzielona
    if(pamiec_id == -1)
    {
        printf("Błąd przy alokacji pamieci współdzielonej\n");
        return ;
    }

    fs = attachSegmentOfSharedMemory();

    for(int i =0; i< INODE_COUNT ; ++i)
        fs->inodes[i].type = -1;

    for(int i =0; i< DESCRIPTION_TABLE_SIZE ; ++i)
        fs->descriprionTable[i].nrInode = -1;

    mkfifo(SERVER_FIFO, 0666);
}

server::~server()
{
   detachSegmentOfSharedMemory(fs);
   //pobieramy id pamieci wspoldzielonej
  int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0);
  if(pamiec_id == -1)
  {
    printf("Błąd przy uzyskaniu segmentu pamięci współdzielonej\n");
    return ;
  }
  //zwalnianie pamięci współdzielonej
  shmctl(pamiec_id, IPC_RMID, NULL);
}

void server::work()
{
  char buf[128];
  int n = 0;
  while(true)
  {
    clientRequest req;
    int fd = open(SERVER_FIFO, O_RDONLY);
    n = read(fd, &req, sizeof(req));
    close(fd);

    if(n == 0)
      break;

    serverResponse res;
    switch(req.type)
    {
      case OPEN_ACT:
                    res.result = simplefs_open(req.path,req.mode);
                    break;
      case UNLINK_ACT:
                    res.result = simplefs_unlink(req.path);
                    break;
      case MKDIR_ACT:
                    res.result = simplefs_mkdir(req.path);
                    break;
      case CREATE_ACT:
                    res.result = createFile(req.path,TYPE_FILE,req.fd,req.size,req.mode);
                    break;
      case READ_ACT:

                    break;
      case LSEEK_ACT:

                    break;
      case CLOSE:
                    res.result = close(req.fd);
                    break;
      case WRITE_ACT:
                    int nodeNumber = getNodeNumberByFD(req.fd);
                    if((nodeNumber < 0) || (checkMode(nodeNumber,req.mode)))
                        res.result = -1;
                    else
                        res.result = writeToFile(nodeNumber, req.size);
                    break;
    }
    // TODO do debugowania
    printf("%s\n", req.path);
    strcpy(res.clientFifoId, req.clientFifoId);
    int clientfd = open(req.clientFifoId, O_WRONLY);
    write(clientfd, &res, sizeof(res));
    close(clientfd);
  }
}

FileSystem* server::attachSegmentOfSharedMemory()
{
    //pobieramy id pamieci wspoldzielonej
    int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0);
    if(pamiec_id == -1)
    {
        printf("Blad przy uzyskaniu id pamieci\n");
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

void server::detachSegmentOfSharedMemory(FileSystem* shared_memory)
{
    //odlaczamy segment wspoldzelonej pamięci
    if(shmdt(shared_memory) == -1)
    {
        printf("Nieudane odłączenie pamięci współdzielonej\n");
        return;
    }
}
