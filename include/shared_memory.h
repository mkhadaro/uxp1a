#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

//Opis pliku: Plik naglowkowy zawierajacy definicje stalych wykorzystywanych
//w implementacji systemu plikow

 //klucz do odwolan do pamieci wspoldz.
#define MEMORY_KEY     998

// Stale dotyczace systemu plikow
#define INODE_COUNT 512
#define BLOCK_COUNT 1031
#define DATA_BLOCK_COUNT 1024
#define BLOCK_SIZE 4096

// Typy inodeow
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2

#define NAME_SIZE 24
#define PATH_SIZE
#define EMPTY_ADDRESS 0xFFFFFFFFFFFFFFFF

#define int_l long long int

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

#endif // SHARED_MEMORY_H
