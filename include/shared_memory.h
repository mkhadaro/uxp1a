/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje struktur
wykorzystywanych
w implementacji systemu plikow
**/
#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

#include "constant_definitions.h"

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
    int_l size;//rozmiar pliku
	int_l address;//nr poczatkowego bloku
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
