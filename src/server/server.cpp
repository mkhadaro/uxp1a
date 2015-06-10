/**
<--Opis pliku -->
Plik zródłowy zawiera implementacje
wybranych funkcji servera:

destruktor

**/

#include <sys/shm.h>
#include "../../include/communication.h"
#include "../../include/server.h"

server::server()
{
    int pamiec_id = shmget(MEMORY_KEY, sizeof(FileSystem), 0777 | IPC_CREAT | IPC_EXCL);
    /**alokujemy pamiec wspoldzieloną*/
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
    simplefs_mkdir("/");
}

server::~server()
{
   //odłączenie pamięci współdzielonej
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

    int result = -1;
    switch(req.type)
    {
      case OPEN_ACT:
                    result = simplefs_open(req.path,req.mode);
                    break;
      case UNLINK_ACT:
                    result = simplefs_unlink(req.path);
                    break;
      case MKDIR_ACT:
                    result = simplefs_mkdir(req.path);
                    //std::cout<<req.type<<" "<<req.path<<" "<<req.clientFifoId<<" "<<result<<std::endl;
                    break;
      case CREATE_ACT:
                    result = createFile(req.path,TYPE_FILE,req.fd,req.size,req.mode);
                    break;
      case READ_ACT:
                    result = simplefs_read(req.fd,req.size);
                    break;
      case LSEEK_ACT:
                    result = simplefs_lseek(req.fd,req.type,req.size);
                    break;
      case CLOSE:
                    result = close(req.fd);
                    break;
      case WRITE_ACT:
                    int nodeNumber = getNodeNumberByFD(req.fd);//<--sprawdzam czy plik otwarty i zwracam inode- powiazany
                    if((nodeNumber < 0) || (checkMode(nodeNumber,req.mode)))//sprawdzam uprawnienia
                        result = -1;
                    else
                        result = writeToFile(nodeNumber, req.size);
                    break;
    }
    // TODO do debugowania
    printf("%s\n", req.path);
    serverResponse res;
    res.result = result;
    strcpy(res.clientFifoId, req.clientFifoId);
    int clientfd = open(req.clientFifoId, O_WRONLY);
    write(clientfd, &res, sizeof(res));
    close(clientfd);
  }
}