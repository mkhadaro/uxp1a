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
    //zwraca struktura z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,CREATE);
    if(dirNodeAndFileName.second == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }
    if(dirNodeAndFileName.second == -2)
        return -1;
    /**
    @return nowy nrInode do utworzenia  pliku
    */
    int_l nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);

    if(nodeNumber == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }

	setNewInodeData(nodeNumber, type, r, w, x,dirNodeAndFileName.first);
    setInodeBit(nodeNumber, true);
	return nodeNumber;
}

void server::setNewInodeData(int inodeNumber, int type, int r, int w, int x, char* name)
{
    strcpy(fs->inodes[inodeNumber].name , name);
	fs->inodes[inodeNumber].type = type;
	fs->inodes[inodeNumber].r = r;
	fs->inodes[inodeNumber].w = w;
	fs->inodes[inodeNumber].x = x;
	fs->inodes[inodeNumber].size = 0;
	fs->inodes[inodeNumber].address = EMPTY_ADDRESS;
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

void server::setInodeBit(int number, bool value)
{
	int_l byteNumber = number / 8;
	int_l bitNumber = number % 8;

	if(value == true)
		fs->inodeBitmap[byteNumber] |= (1 << bitNumber);
	else
		fs->inodeBitmap[byteNumber] &= ~(1 << bitNumber);
}

filesName server::checkName(char* name,int INODE_TYPE,int type_of_operation)
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

        if(root_inode->type == -1 && type_of_operation == CREATE)
        {
            int_l nodeNumebr = findFreeInodeNumber();
            char *file =(char*) malloc(strlen(fileName)+1);
            memcpy(file, fileName, strlen(fileName));
            strcpy(file,fileName);
            setNewInodeData(nodeNumebr,INODE_TYPE, 1, 1, 1,file);
            setInodeBit(nodeNumebr, true);

        }
        filesName k("",-2);
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

                nrInode = checkValueInMap(inode->pointers,fileName,INODE_TYPE);
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
                int counterFiles = 0;
                counterFiles = checkValueCount(inode->pointers,fileName,counterFiles) ;
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

                if(counterFiles == 1)
                {
                    nrInode = checkValueInMap(inode->pointers,fileName,INODE_TYPE);
                    inode = &(fs->inodes[nrInode]);
                    if((type_of_operation == CREATE && inode->type == INODE_TYPE) || (type_of_operation == DELETE && inode->type != INODE_TYPE))
                        dirNode = -1;

                    filesName k(fileName,dirNode);
                    free(i);
                    return k;
                }
            }
    }
    if(type_of_operation == CREATE)
    {
        char *nameOfFile =(char*) malloc(strlen(fileName)+1);
        nameOfFile[strlen(fileName)] = 0;
        memcpy(nameOfFile, fileName, strlen(fileName));
        filesName k (nameOfFile,dirNode);
        free(i);
        return  k;
    }
    filesName k (fileName,-1);
    free(i);
    return  k;

}

/**
@return nowy nrInode - do utworzenia katalogu
*/
int server::updateLinksMapAndCreateFile(int & dirNode)
{
    int nodeNumber = findFreeInodeNumber();
    if(nodeNumber == -1)
        return -1;
    INode* inode = &(fs->inodes[dirNode]);
    for(int i = 0; i < sizeof (inode->pointers)/sizeof(int); ++i)
    {
        if(inode->pointers[i] == 0 && i < 7)
        {
            inode->pointers[i] = nodeNumber;
            break;
        }
        if(i == 7)
        {
            if(inode->pointers[i] == 0)//warunek czy istnieje przypisanie do tego nr inodu, 0 -nr inode roota
            {
                    inode->pointers[i] = nodeNumber;//dowiązuje pomocniczy inode do tablicy inode-ow danego katalogu
                    //tworze pomocniczy inode,który będzie przechowywał dowiązania do kojenych nrInode,należących do katalogu
                    setNewInodeData(nodeNumber, TYPE_HELPER, 1, 1, 1,"help");
                    setInodeBit(nodeNumber, true);
                    inode = &(fs->inodes[nodeNumber]);//pobieram inode helpera
                    return updateLinksMapAndCreateFile(nodeNumber);
            }
            else
                return updateLinksMapAndCreateFile(inode->pointers[i]);
        }

    }
    return nodeNumber;
}


//return nodeNumer deleted file
int server::updateLinksMapAndDeleteFile(filesName & fileStruct)
{
/*
    int dirNode = fileStruct.second;
    INode* inode = &(fs->inodes[dirNode]);
    for(int i =0; i < sizeof (inode->pointers); ++i)
        if(inode->pointers[i] == fileStruct.first && i < 8)
            inode->pointers[i] = 0;
    //inode->pointers -= 1;

    int counter = checkValueCount(inode->mapsDirFile,fileStruct.first);
    int nodeNumber = checkValueInMap(inode->mapsDirFile,fileStruct.first);
    if(counter == 2)
        nodeNumber = returnSecondValueOfMaps(inode->mapsDirFile,fileStruct.first,nodeNumber);

    inode->mapsDirFile.erase(nodeNumber);
    free(fileStruct.first);

    return nodeNumber;
    */
}

int server::checkValueInMap(int *maps,char* value,int TYP_INODE)
{
    for(int i = 0; i < sizeof(maps)/sizeof(int) ; ++i)
    {
        if(strcmp(fs->inodes[maps[i]].name , value) == 0)
        {
           if(fs->inodes[maps[i]].type == TYP_INODE)
                return maps[i];
        }
        if(fs->inodes[maps[i]].type == TYPE_HELPER)
            checkValueInMap(fs->inodes[maps[i]].pointers,value,TYP_INODE);
    }
    return -1;
}

int & server::checkValueCount(int *maps,char* value,int & counter)
{
    //int counter = 0;
    for(int i = 0; i < sizeof(maps)/sizeof(int) ; ++i)
    {
        if(strcmp(fs->inodes[maps[i]].name, value) == 0)
             ++counter;
        if(fs->inodes[maps[i]].type == TYPE_HELPER)
            checkValueCount(fs->inodes[maps[i]].pointers,value,counter);
    }
    return counter;
}
