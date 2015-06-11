#include "../include/server.h"

void server::showServerState()
{
	printFreeBlockBitmap();
	printFreeInodeBitmap();
	printDirectories(0, 0);
	printfFileDescriptors();
}

void server::printFreeBlockBitmap()
{
	printf("%s\n", "Mapa wolnych blokow:");
	for(int i =0; i < 24; i++)
	{
		char check = fs->blockBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 1)
 				printf("%s", "1");
 			else
 				printf("%s", "0");
 			check >>= 1;
 		}
 		printf(" ");
 		if(i % 4 == 3)
 			printf("\n");
	}
}

void server::printFreeInodeBitmap()
{
	printf("%s\n", "Mapa wolnych inodow:");
	for(int i =0; i < 24; i++)
	{
		char check = fs->inodeBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 1)
 				printf("%s", "1");
 			else
 				printf("%s", "0");
 			check >>= 1;
 		}
 		printf(" ");
 		if(i % 4 == 3)
 			printf("\n");
	}
}

void server::printDirectories(int_l inodeNumber, int depth)
{
	for(int i = 0; i < depth; i++)
		printf("  ");
	std::cout << fs->inodes[inodeNumber].name << "\t\t\t" << fs->inodes[inodeNumber].type << "  ";
	std::cout << fs->inodes[inodeNumber].size << "\n";
	bool doWhile = true;

	while(doWhile)
	{
		doWhile = false;	
		for(int i = 0; i < sizeof(fs->inodes[inodeNumber].pointers)/sizeof(int); i++)
		{
			int pointedNumber = fs->inodes[inodeNumber].pointers[i];
			if(pointedNumber == 0)
				continue;

			if(fs->inodes[pointedNumber].type == TYPE_DIR)
				printDirectories(pointedNumber, depth + 1);
			else if(fs->inodes[pointedNumber].type == TYPE_HELPER)
			{
				doWhile = true;
				inodeNumber = pointedNumber;
			}
			else
			{
				for(int j = 0; j < depth + 1; j++)
					printf("  ");
				std::cout << fs->inodes[pointedNumber].name <<"\t\t\t" << fs->inodes[pointedNumber].type << "  ";
				std::cout << fs->inodes[pointedNumber].size << "\n";
			}
		}
	}
	
}

void server::printfFileDescriptors()
{
	for (int i = 0; i < DESCRIPTION_TABLE_SIZE; i++)
	{
		FileDescription fd = fs->descriprionTable[i];
		if (fd.fileDescriptor != 0)
			printf("%d  %d  %d  %d\n", fd.fileDescriptor, fd.nrInode, fd.mode, fd.filePosition);
	}
}