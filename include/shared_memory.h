/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje struktur
wykorzystywanych
w implementacji systemu plikow
**/
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "constant_definitions.h"

 //klucz do odwolan do pamieci wspoldz.
#define MEMORY_KEY 1062

// Stale dotyczace systemu plikow
#define INODE_COUNT 512
#define BLOCK_COUNT 1031
#define DATA_BLOCK_COUNT 1024
#define DESCRIPTION_TABLE_SIZE 128
#define BLOCK_SIZE 4096

#define INODE_NAME_SIZE 16

#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2


#define ACTUAL_POSITION 1
#define END_POSITION    2
#define START_POSITION  3

//mode
#define READ 1
#define WRITE 2

//rodzaj pliku w odniesieniu do hierarchii
#define CHILD 1
#define PARENT_DIR 2

#define NAME_SIZE 24

#define EMPTY_ADDRESS 0xFFFFFFFFFFFFFFFF

//stale dotyczace typow operacji
#define DELETE 1
#define GET_VALUE 1
#define CREATE 2


#define int_l long long int

/** struktura opisujaca inode
    zawiera - nazwe
*/
typedef struct inode
{
    char name[INODE_NAME_SIZE];
	int type;
	short int r;
	short int w;
	short int x;
	double size;
	double address;
    int pointers[8];/***zawiera podrzędne nr inod-ow plików lub katalogów**/
} INode;

/**
struktura opisujaca deskryptor pliku
zawiera - deskryptor pliku,nr inode-a,tryb w ktorym zostal otwarty,aktualna pozycja w pliku
**/
typedef struct
{
    int fileDescriptor;
    int nrInode;
    int mode;
    int filePosition;
}FileDescription;

/**
struktura opisujaca system plikow
*/
typedef struct
{
	char superBlock[BLOCK_SIZE];
	char inodeBitmap[BLOCK_SIZE];
	char blockBitmap[BLOCK_SIZE];
	INode inodes[INODE_COUNT];
	char dataBlocks[DATA_BLOCK_COUNT*BLOCK_SIZE];
	FileDescription descriprionTable[DESCRIPTION_TABLE_SIZE];
} FileSystem;

#endif // SHARED_MEMORY_H
