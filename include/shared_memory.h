#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

//Opis pliku: Plik naglowkowy zawierajacy definicje stalych wykorzystywanych
//w implementacji systemu plikow
#include <iostream>
#include <map>
 //klucz do odwolan do pamieci wspoldz.
#define MEMORY_KEY     1045//998

// Stale dotyczace systemu plikow
#define INODE_COUNT 512
#define BLOCK_COUNT 1024
#define DATA_BLOCK_COUNT 1017
#define BLOCK_SIZE 4096

#define INODE_NAME_SIZE 16

#define ROOT_INODE (INODE_COUNT + BLOCK_COUNT);
#define POINTER_COUNT 8
#define BLOCK_INDEX_OFFSET 32768

// Typy inodeow
#define FREE_INODE -1
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2

typedef struct inode
{
    char name[INODE_NAME_SIZE];
	int type;
	short int r;
	short int w;
	short int x;
	int pointers;
	double size;
	double address;
    /***mapuje nazwy dowiązane do niego pliki/katalogi na inode**/
    std::multimap<int,char*> mapsDirFile;
    //std::map<const char*,int> mapsDirFile;
} INode;

typedef struct
{
	char superBlock[BLOCK_SIZE];
	char inodeBitmap[BLOCK_SIZE];
	char blockBitmap[BLOCK_SIZE];
	INode inodes[INODE_COUNT];
	char dataBlocks[DATA_BLOCK_COUNT*BLOCK_SIZE];
} FileSystem;

typedef struct
{
	int fd;
	short int inode;
	int mode;
} DescriptorTableEntry;

typedef struct
{
    char precendentDirName[20];
    char fileName[20];
}testStructure;

#endif // SHARED_MEMORY_H
