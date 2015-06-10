#include "../include/server.h"

int_l server::createFile(char *name , int type, int r, int w, int x)
{
    //zwraca struktura z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,type,CREATE);
    if(dirNodeAndFileName.second == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }

    int_l nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);
    if(nodeNumber == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }
	setNewInodeData(nodeNumber, type, r, w, x,dirNodeAndFileName.first);
	free(dirNodeAndFileName.first);//zwalniam pamięć przydzieloną na nazwę po rozbiorze sciężki z nazwą
    setInodeBit(nodeNumber, true);
	return 0;
}

int server::simplefs_mkdir(char* name)
{
    //zwraca strukturę z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,TYPE_DIR,CREATE);
    if(dirNodeAndFileName.second == -1)
    {
        return -1;
    }
    if(dirNodeAndFileName.second == -2)
    {
        free(dirNodeAndFileName.first);//zwalniam pamięć przydzieloną na nazwę po rozbiorze sciężki z nazwą
        return -2;
    }
    // nr inode - do dalszego utworzenia nowego nr inode dla katalogu
    int_l nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);
    if(nodeNumber == -1)
    {
        printf("Błąd przy tworzeniu katalogu \n");
        return -1;
    }
    setNewInodeData(nodeNumber, TYPE_DIR, 1, 1, 1,dirNodeAndFileName.first);
    free(dirNodeAndFileName.first);//zwalniam pamięć przydzieloną na nazwę po rozbiorze sciężki z nazwą
    setInodeBit(nodeNumber, true);

    return 0;
}

//funkcja usuwająca plik
int server::simplefs_unlink(char* name)
{
    //pobieram node pliku do usunięcia i sprawdzam czy istnieją deskryptory
    int inodeNumber = getInodeNumber(name,TYPE_FILE,CHILD);
    if(inodeNumber == -1)
        return -1;

    for(int i = 0; i < INODE_NAME_SIZE; ++i)
        if(fs->descriprionTable[i].nrInode == inodeNumber)
            return -1;


    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,GET_VALUE);
    if(dirNodeAndFileName.second == -1)
            return -1;
    INode & node = fs->inodes[dirNodeAndFileName.second];

    updateLinksMapAndDeletePointer(dirNodeAndFileName,TYPE_FILE);
    free(dirNodeAndFileName.first);//zwalniam pamięć przydzieloną na nazwę po rozbiorze sciężki z nazwą
    fs->inodes[inodeNumber].type = -1;

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

int server::simplefs_open(char* name,int mode)
{
    int inodeNumber = getInodeNumber(name,TYPE_FILE,CHILD);
    if(inodeNumber < 0)
        return -1;
    if(checkMode(inodeNumber,mode) == 0)
        return -1;
    return createDescription(inodeNumber,mode);
}

