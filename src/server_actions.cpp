#include "../include/server.h"
#include <stdio.h>

int_l server::createFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE], int type, int r, int w, int x)
{
	int_l inodeNumber = findFreeInodeNumber();
	if(inodeNumber == -1)
		return -1;
	
	setNewInodeData(inodeNumber, type, r, w, x);
	setInodeBit(inodeNumber, true);
	// TODO
	// wstawic do odpowiedniego katalogu nazwe i numer inoda
	return inodeNumber;
}

void server::setNewInodeData(int_l inodeNumber, int type, int r, int w, int x)
{
	fs->inodes[inodeNumber].type = type;
	fs->inodes[inodeNumber].r = r;
	fs->inodes[inodeNumber].w = w;
	fs->inodes[inodeNumber].x = x;
	fs->inodes[inodeNumber].pointers = 1;
	fs->inodes[inodeNumber].size = 0;
	fs->inodes[inodeNumber].address = EMPTY_ADDRESS;
}

int server::deleteFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE])
{
	// TODO
	// wyszukanie numeru inoda
	// usuniecie pliku z katalogu
	int_l inodeNumber;
	if(inodeNumber == -1)
		return -1;

	int_l inodeAddress = fs->inodes[inodeNumber].address;
	int_l inodeSize = fs->inodes[inodeNumber].size;
	int_l blocksAssigned = inodeSize / BLOCK_SIZE; 

	setBlockBit(inodeAddress, blocksAssigned, false);
	setInodeBit(inodeNumber, false);
	return 0;
}

int_l server::writeToFile(int_l inodeNumber, int_l size)
{
	int_l inodeAddress = fs->inodes[inodeNumber].address;
	int_l inodeSize = fs->inodes[inodeNumber].size;
	int_l blocksAssigned = inodeSize / BLOCK_SIZE;
	int_l blocksNeeded;
	int_l startingBlock;
	int_l newSize;

	if(inodeAddress == EMPTY_ADDRESS) 
	{
		blocksNeeded = size / BLOCK_SIZE;
		startingBlock = findFreeBlockNumber(blocksNeeded);
		inodeAddress = startingBlock;
		newSize = size;
		if(startingBlock == -1)
			return -1;
	}
	else
	{
		int_l newSize = inodeSize + size;
		blocksNeeded = (newSize / BLOCK_SIZE) - blocksAssigned;
		startingBlock = inodeAddress + blocksAssigned;
		if(canAddToFile(startingBlock, blocksNeeded) == false)
			return -1;
	}

	fs->inodes[inodeNumber].address = inodeAddress;
	fs->inodes[inodeNumber].size = newSize;
	setBlockBit(startingBlock, blocksNeeded, true);
	return inodeSize;
}

int_l server::findFreeBlockNumber(int_l blocksNeeded)
{
 	int_l blocksFound = 0;
 	bool previousFree = false;
 	int_l blockNumber = -1;

 	for(int_l i = 0; i < BLOCK_COUNT / 8; i++)
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

void server::setBlockBit(int_l number, int_l amount, bool value)
{
	int_l byteNumber = number / 8;
	int_l bitNumber = number % 8;

	while(amount > 0)
	{
		if(value == true)
			fs->blockBitmap[byteNumber] |= (1 << bitNumber);
		else
			fs->blockBitmap[byteNumber] &= ~(1 << bitNumber);
		bitNumber++;
		bitNumber %= 8;
		if(bitNumber == 0)
			byteNumber++;
		amount--;
	}
}

bool server::canAddToFile(int_l startingBlock, int_l amount) 
{
	int_l byteNumber = startingBlock / 8;
	int_l bitNumber = startingBlock % 8;

	while (amount > 0)
	{
		char bit = fs->blockBitmap[byteNumber] & (1 << bitNumber);
		if (bit == (1 << bitNumber))
			return false;

		bitNumber++;
		bitNumber %= 8;
		if(bitNumber == 0)
			byteNumber++;
		amount--;
	}
	return true;
}

int_l server::findFreeInodeNumber()
{
 	for(int_l i = 0; i < INODE_COUNT / 8; i++)
 	{
 		char check = fs->inodeBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 				return i * 8 + j;
 			check >>= 1;
 		}
 	}
 	return -1;
}

void server::setInodeBit(int_l number, bool value)
{
	int_l byteNumber = number / 8;
	int_l bitNumber = number % 8;

	if(value == true)
		fs->inodeBitmap[byteNumber] |= (1 << bitNumber);
	else
		fs->inodeBitmap[byteNumber] &= ~(1 << bitNumber);
}