#include "../include/server.h"
#include <stdio.h>

int server::findBlockNumber(double size)
 {
 	int blocksNeeded = size / BLOCK_SIZE + 1;
 	int blocksFound = 0;
 	bool previousFree = false;
 	int blockNumber = -1;

 	printf("%d\n", blocksNeeded);
 	for(int i = 0; i < BLOCK_COUNT / 8; i++)
 	{
 		char check = fs->blockBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 			{
 				if(previousFree == false)
 					blockNumber = i * 8 + j;
 				previousFree = true;
 				blocksFound++;
 				if(blocksFound == blocksNeeded)
 					return blockNumber;

 			}
 			else
 			{
 				previousFree = false;
 				blocksFound = 0;
 			}
 			check >>= 1;
 		}
 	}
 	return -1;
 }
