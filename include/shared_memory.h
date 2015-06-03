#ifndef SHARED_MEMORY_H
#define SHARED_MEMORY_H

//Opis pliku: Plik naglowkowy zawierajacy definicje stalych wykorzystywanych
//w implementacji systemu plikow

 //klucz do odwolan do pamieci wspoldz.
#define KLUCZ_PAMIECI     998

// Stale dotyczace systemu plikow
#define INODE_COUNT 256
#define BLOCK_COUNT 1024
#define BLOCK_SIZE 1024
#define INODE_NAME_SIZE 16
#define ROOT_INODE (INODE_COUNT + BLOCK_COUNT);
#define POINTER_COUNT 8
#define BLOCK_INDEX_OFFSET 32768

// Typy inodeow
#define TYPE_DIR 0
#define TYPE_FILE 1
#define TYPE_HELPER 2

typedef struct inode
{
	char name[INODE_NAME_SIZE];
	short int file_size;
	int type;
	int r;
	int w;
	int x;
	short int pointers[POINTER_COUNT];
} INode;

typedef struct
{
	char inode_bitmap[INODE_COUNT/8];
	char block_bitmap[BLOCK_COUNT/8];
	INode inodes[INODE_COUNT];
	char blocks[BLOCK_COUNT*BLOCK_SIZE];
} FileSystem;

typedef struct
{
	int fd;
	short int inode;
	int mode;
} DescriptorTableEntry;

#endif // SHARED_MEMORY_H
