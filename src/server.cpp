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
    {
        fs->inodes[i].type = -1;
    }

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
  //zwalnianie pamieci przydzielonej na nazwy
  /*
    for(int i =0; i< INODE_COUNT ; ++i)
    {
        if(fs->inodes[i].type != -1)
            free(fs->inodes[i].name);
    }
   */

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


void server::simplefs_mkdir(char* name)
{
        //zwraca struktura z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,TYPE_DIR,CREATE);

    std::cout<< "nazwa "<<dirNodeAndFileName.first<<std::endl;

    if(dirNodeAndFileName.second == -1)
    {
        printf("Błąd przy tworzeniu pliku %s\n");
        return ;
    }
    if(dirNodeAndFileName.second == -2)
    {
        return;
    }
    //zwrócimy parę,zawierającą: nowy nrInode do utworzenia oraz nazwa pliku
    int nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);

    if(nodeNumber == -1)
    {
        printf("Błąd przy tworzeniu pliku \n");
        return ;
    }
	setNewInodeData(nodeNumber, TYPE_DIR, 1, 1, 1,dirNodeAndFileName.first);
	setInodeBit(nodeNumber, true);

	return;
}


