#include "../include/server.h"

void server::showServerState()
{
	printFreeBlockBitmap();
	printFreeInodeBitmap();
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
 		}
 		printf(" ");
 		if(i % 4 == 3)
 			printf("\n");
	}
}