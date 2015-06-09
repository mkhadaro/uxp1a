#include <stdio.h>

#include "../include/server.h"

int_l server::createFile(char *name , int type, int r, int w, int x)
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

void server::simplefs_mkdir(char* name)
{
    //zwraca strukturę z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,TYPE_DIR,CREATE);

    if(dirNodeAndFileName.second == -1)
    {
        printf("Błąd przy tworzeniu katalogu\n");
        return ;
    }
    if(dirNodeAndFileName.second == -2)//stworzylismy katalog główny "/"
        return;

    // nr inode - do dalszego utworzenia nowego nr inode dla katalogu
    int_l nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);
    if(nodeNumber == -1)
    {
        printf("Błąd przy tworzeniu katalogu \n");
        return ;
    }
    setNewInodeData(nodeNumber, TYPE_DIR, 1, 1, 1,dirNodeAndFileName.first);
    setInodeBit(nodeNumber, true);

    return;
}

int server::simplefs_unlink(char* name)
{
    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,DELETE);
    if(dirNodeAndFileName.second == -1)
        return -1;

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
