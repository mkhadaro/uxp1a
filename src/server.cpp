#include <stdio.h>
#include <sys/ipc.h>
#include <sys/shm.h>

#include "../include/server.h"

#include <iostream>

server::server()
{
    int pamiec_id = shmget(KLUCZ_PAMIECI, sizeof(FileSystem), 0777 | IPC_CREAT | IPC_EXCL);
    //alokujemy pamiec wspoldzielona
    if(pamiec_id == -1)
    {
        printf("Błąd przy alokacji pamieci współdzielonej\n");
        return ;
    }

}

server::~server()
{
   //pobieramy id pamieci wspoldzielonej
  int pamiec_id = shmget(KLUCZ_PAMIECI, sizeof(FileSystem), 0);
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
    int pamiec_id = shmget(KLUCZ_PAMIECI, sizeof(FileSystem), 0);
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

void server::initDataInSharedMemory()
{

    FileSystem* shared_memory = attachSegmentOfSharedMemory();
    /**************************************************************************************************/
    //tu piszemy dane do naszej pamieci wspoldzielonej

	INode* root_inode = &(shared_memory->inodes[0]);

	root_inode->name[0] = 'r';
	root_inode->name[1] = 'o';
	root_inode->name[2] = 'o';
	root_inode->name[3] = 't';
	root_inode->name[4] = '\0';
	root_inode->type = TYPE_DIR;
	root_inode->pointers[0] = 1;
	root_inode->r = 1;
	root_inode->w = 1;

    printf("inode %s",root_inode->name);

	INode* inode = &(shared_memory->inodes[1]);
	inode->name[0] = 'a';
	inode->name[1] = 'b';
	inode->name[2] = 'c';
	inode->name[3] = '\0';
	inode->type = TYPE_DIR;
	inode->r = 1;
	inode->w = 1;
	inode->pointers[0] = 2;

	inode = &(shared_memory->inodes[2]);
	inode->name[0] = 'd';
	inode->name[1] = 'e';
	inode->name[2] = 'f';
	inode->name[3] = '\0';
	inode->type = TYPE_DIR;
	inode->r = 1;
	inode->w = 1;
	inode->pointers[2] = 7;

	inode = &(shared_memory->inodes[7]);
	inode->name[0] = 'g';
	inode->name[1] = 'h';
	inode->name[2] = 'i';
	inode->name[3] = '\0';
	inode->type = TYPE_FILE;
	inode->r = 1;
	inode->w = 1;
	inode->pointers[0] = BLOCK_INDEX_OFFSET + 0;

    /**************************************************************************************************/
    detachSegmentOfSharedMemory(shared_memory);

     shared_memory = attachSegmentOfSharedMemory();

     inode = &(shared_memory->inodes[7]);
     printf("\nroot_name: %s\n", inode->name);

     detachSegmentOfSharedMemory(shared_memory);
    //inicjowanie potoku fifo serwera
}


void server::simplefs_open(char* name,int mode)
{

}
void server::simplefs_unlink(char* name)
{

}
void server::simplefs_mkdir(char* name)
{

}
void server::simplefs_create(char* name,int mode)
{

}
void server::simplefs_read(int fd,char* buf,int len)
{

}
void server::simplefs_write(int fd,char* buf,int len)
{

}
void server::simplefs_lseek(int fd,int whence,int len)
{

}
