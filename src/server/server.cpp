/**
<--Opis pliku -->
Plik zródłowy zawiera implementacje
wybranych funkcji servera:

konstruktor
destruktor
funkcje manipulujące na pamieci wspoldzielonej

**/

#include <sys/shm.h>
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
    {
        fs->inodes[i].type = -1;
    }

    for(int i =0; i< DESCRIPTION_TABLE_SIZE ; ++i)
        fs->descriprionTable[i].nrInode = -1;

    for(int i =0; i< DATA_BLOCK_COUNT*BLOCK_SIZE ; ++i)
    {
        fs->dataBlocks[i] = 0;
    }

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
