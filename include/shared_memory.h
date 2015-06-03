#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

//Opis pliku: Plik naglowkowy zawierajacy definicje stalych wykorzystywanych
//w implementacji systemu plikow

 //klucz do odwolan do pamieci wspoldz.
#define MEMORY_KEY     998

// Stale dotyczace systemu plikow
#define INODE_COUNT 512
#define BLOCK_COUNT 1024
#define DATA_BLOCK_COUNT 1017
#define BLOCK_SIZE 4096

#define ROOT_INODE (INODE_COUNT + BLOCK_COUNT);
#define POINTER_COUNT 8
#define BLOCK_INDEX_OFFSET 32768

// Typy inodeow
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2

typedef struct inode
{
	int type;
	short int r;
	short int w;
	short int x;
	int pointers;
	double size;
	double address;
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

#endif // SHARED_MEMORY_H
