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
        fs->inodes[i].type = FREE_INODE;
        fs->inodes[i].pointers = 0;
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



void server::createInode(char* name,int type,int nrInode)
{
    INode inode;
    strcpy(inode.name , name);
    inode.type = type;
    inode.r = 1;
    inode.size = 0;
    inode.w = 1;
    inode.pointers = 0;
    inode.mapsDirFile.clear();

    fs->inodes[nrInode] = inode;
}

void server::simplefs_mkdir(char* name)
{
    filesName id_inode_file = checkName(name,TYPE_DIR,CREATE);

    int dirNode = id_inode_file.second;
    if(dirNode == -1)
    {
        printf("No such file %s of directory\n",name);
        return;
    }
    std::cout<<"zawarosc listy gornej "<<fs->inodes[id_inode_file.second].pointers<<std::endl;

    id_inode_file = updateLinksMapAndCreateFile(id_inode_file,TYPE_DIR);
    printf("%d inodeNumber \n",id_inode_file.second);


    std::cout<<"zawarosc listy "<<fs->inodes[id_inode_file.second].pointers<<std::endl;


    setNewInodeData(id_inode_file.second,TYPE_DIR,1,1,1,id_inode_file.first);
    setInodeBit(id_inode_file.second, true);
}


