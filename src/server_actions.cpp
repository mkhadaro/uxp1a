#include "../include/server.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <set>

using namespace std;

int_l server::createFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE], int type, int r, int w, int x)
{
	//int_l inodeNumber = findFreeInodeNumber();
	//if(inodeNumber == -1)
		//return -1;
    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,CREATE);

    if(dirNodeAndFileName.second == -1)
    {
        return -1;
    }
    dirNodeAndFileName = updateLinksMapAndCreateFile(dirNodeAndFileName,TYPE_FILE);

	setNewInodeData(dirNodeAndFileName.second, type, r, w, x,dirNodeAndFileName.first);
	setInodeBit(dirNodeAndFileName.second, true);
	// TODO
	// wstawic do odpowiedniego katalogu nazwe i numer inoda
	return dirNodeAndFileName.second;
}

void server::setNewInodeData(int_l inodeNumber, int type, int r, int w, int x, char* name)
{
    strcpy(fs->inodes[inodeNumber].name , name);
    //fs->inodes[inodeNumber].name = name;
	fs->inodes[inodeNumber].type = type;
	fs->inodes[inodeNumber].r = r;
	fs->inodes[inodeNumber].w = w;
	fs->inodes[inodeNumber].x = x;
	fs->inodes[inodeNumber].pointers = 0;//??
	fs->inodes[inodeNumber].size = 0;
	fs->inodes[inodeNumber].address = EMPTY_ADDRESS;
	fs->inodes[inodeNumber].mapsDirFile.clear();
}

int server::deleteFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE])
{
    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,CREATE);

    if(dirNodeAndFileName.second == -1)
        return -1;
    updateLinksMapAndDeleteFile(dirNodeAndFileName);
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

/*
 int server::findFreeIdInode()
 {
    for(int i = 0; i < INODE_COUNT ; ++i)
 	{
        INode inode = fs->inodes[i];

 		if(inode.type == FREE_INODE)
            return i;
 	}
 	return -1;
 }
 */

filesName & server::checkName(char* name,int INODE_TYPE,int type_of_operation)
{
    if(name[0] !='/')
    {
        printf("[Error]Blędnie podana ścieżka\n");
        filesName k("",-1);
        return k;
    }

    char *i =(char*) malloc(strlen(name)+1);
	i[strlen(name)] = 0;
	memcpy(i, name, strlen(name));

    char fileName[16] = {0};
    char dirName[16] = {0} ;

    fileName[ 0] = '/';
    strcpy(i,name);

    INode* root_inode = &(fs->inodes[0]);

    if(strlen(name) == 1)
    {

        if(root_inode->type == FREE_INODE && type_of_operation == CREATE)
        {
            //createInode("/",0,findFreeIdInode());
            int nodeNumebr = findFreeInodeNumber();
            setNewInodeData(nodeNumebr,INODE_TYPE, 1, 1, 1,fileName);
            setInodeBit(nodeNumebr, true);
            printf("%d inode Number kuk\n",nodeNumebr);

        }
        filesName k(fileName,-1);
        free(i);
        return k;
    }

    int j = 0;
    int nrInode = 0;
    int dirNode = 0;
    for(int l = 1; l!= strlen(name);++l)
    {
            if(i[l] =='/')
            {
                INode* inode = &(fs->inodes[nrInode]);
                nrInode = checkValueInMap(inode->mapsDirFile,fileName);
                if(nrInode == -1)
                {
                    free(i);
                    filesName k("",-1);
                    return k;
                }
                inode = &(fs->inodes[nrInode]);
                if(inode->type != TYPE_DIR)
                {
                    free(i);
                    filesName k("",-1);
                    return k;
                }
                for( int i = 0; i < sizeof( fileName); i++ )
                        fileName[ i ] = '\0';

                j = 0;
                continue;
            }

            fileName[j] = i[l];
            ++j ;

            if(l + 1 == strlen(name))
            {
                INode* inode = &(fs->inodes[nrInode]);
                dirNode = nrInode;

                int counterFiles = checkValueCount(inode->mapsDirFile,fileName);

                if(counterFiles == 2)
                {
                    if(type_of_operation == CREATE)
                    {
                        dirNode = -1;
                    }
                    filesName k(fileName,dirNode);
                    free(i);
                    return k;
                }

                nrInode = checkValueInMap(inode->mapsDirFile,fileName);
                inode = &(fs->inodes[nrInode]);

                if(counterFiles == 1)
                {
                    if((type_of_operation == CREATE && inode->type == INODE_TYPE) || (type_of_operation == DELETE && inode->type != INODE_TYPE))
                        dirNode = -1;

                    filesName k(fileName,dirNode);
                    free(i);
                    return k;
                    }
                }
    }

    if(type_of_operation == DELETE)
        dirNode = -1;
    filesName k (fileName,dirNode);
    free(i);
    return  k;

}

filesName & server::updateLinksMapAndCreateFile(filesName & fileStruct,int INODE_TYPE)
{
    int dirNode = fileStruct.second;
    INode* inode = &(fs->inodes[dirNode]);
    inode->pointers += 1;

    int nodeNumber = findFreeInodeNumber();//findFreeIdInode();

    char *file =(char*) malloc(strlen(fileStruct.first)+1);
	memcpy(file, fileStruct.first, strlen(fileStruct.first));
	strcpy(file,fileStruct.first);

    //int nodeNumber = findInodeNumber();//findFreeIdInode();
    if(nodeNumber == -1)
    {
        filesName response("",0);
        return response;
    }
    //inode->mapsDirFile.insert(std::pair<int,char*>(nodeNumber,file));
    inode->mapsDirFile.insert(std::pair<int,char*>(nodeNumber,file));

    filesName fileName(file,nodeNumber);
    //setNewInodeData(int_l inodeNumber, int type, int r, int w, int x,char* name);
    //createInode(file,INODE_TYPE,nodeNumber);
    //int_l inodeNumber = findFreeInodeNumber();

    return fileName;
}

//return nodeNumer deleted file
int server::updateLinksMapAndDeleteFile(filesName & fileStruct)
{
    int dirNode = fileStruct.second;
    INode* inode = &(fs->inodes[dirNode]);
    inode->pointers -= 1;

    int counter = checkValueCount(inode->mapsDirFile,fileStruct.first);
    int nodeNumber = checkValueInMap(inode->mapsDirFile,fileStruct.first);
    if(counter == 2)
        nodeNumber = returnSecondValueOfMaps(inode->mapsDirFile,fileStruct.first,nodeNumber);

    inode->mapsDirFile.erase(nodeNumber);
    free(fileStruct.first);

    return nodeNumber;
}

int server::checkValueInMap(std::multimap<int,char*> maps,char* value)
{
    std::map<int,char*>::iterator it;
    for (it = maps.begin(); it != maps.end(); ++it )
    {
        if (strcmp((*it).second ,value) == 0)
        {
            return it->first;
        }
    }
    return -1;
}

int server::checkValueCount(std::multimap<int,char*> maps,char* value)
{

    std::map<int,char*>::iterator it = maps.begin();
    int counter = 0;
    for (; it != maps.end(); ++it )
    {
                if (strcmp((*it).second ,value) == 0)
                ++counter;
    }

    return counter;
}

int server::returnSecondValueOfMaps(std::multimap<int,char*> maps,char* value,int anotherInode)
{

    std::map<int,char*>::iterator it = maps.begin();
    for (; it != maps.end(); ++it )
    {
                if (strcmp((*it).second ,value) == 0)
                   if(anotherInode !=it->first)
                        return it->first;
    }
}
