#include "../../include/server.h"

int server::createFile(char *name , int type, int r, int w, int x)
{
    //zwraca struktura z nr Inode katalogu nadrzędnego oraz nazwa pliku do utworzenia po rozbiorze slowa wejsciowego
    filesName dirNodeAndFileName = checkName(name,type,CREATE);
    if(dirNodeAndFileName.second == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }

    int adres_bloku_startowego = searchFreeBlock(FILE_SIZE);
    adres_bloku_startowego = adres_bloku_startowego;
    if(adres_bloku_startowego == -1)
    {
        printf("Błąd przy pobraniu adresu bloku\n");
        return -1;
    }

    int_l nodeNumber = updateLinksMapAndCreateFile(dirNodeAndFileName.second);
    if(nodeNumber == -1)
    {
        printf("Błąd przy utworzeniu pliku\n");
        return -1;
    }

	setNewInodeData(nodeNumber, type, r, w, x,dirNodeAndFileName.first,adres_bloku_startowego);

	free(dirNodeAndFileName.first);//zwalniam pamięć przydzieloną na nazwę po rozbiorze sciężki z nazwą
    setInodeBit(nodeNumber, true);

    int blockNumber = adres_bloku_startowego/BLOCK_SIZE;
    fs->blockBitmap[blockNumber] = 1;
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
    setNewInodeData(nodeNumber, TYPE_DIR, 1, 1, 1,dirNodeAndFileName.first,-1);
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
    deleteBlock(fs->inodes[inodeNumber].address,FILE_SIZE);
    setInodeBit(inodeNumber,false);

    int block = inodeAddress/BLOCK_SIZE;//ktory blok
    int isData = true;
    for(int i = block; i < (block + 1)*BLOCK_SIZE - 32; ++i)
        if(fs->dataBlocks[i] == 0)
        {
            i+=32;
        }
        else
        {
            isData = false;
            break;
        }
    if(isData == true)
        fs->blockBitmap[block] = 0;
    return 0;
}

int_l server::writeToFile(int_l inodeNumber, int_l size)
{
    int_l adressBlocku = fs->inodes[inodeNumber].address;
    int actualSize = fs->inodes[inodeNumber].size;
    if(actualSize + size > FILE_SIZE)
        return -1;
    fs->inodes[inodeNumber].size += size;//update rozmiaru pliku
    return adressBlocku + actualSize;
}

int server::simplefs_open(char* name,int mode)
{
    int inodeNumber = getInodeNumber(name,TYPE_FILE,CHILD);/*
    filesName dirNodeAndFileName = checkName(name,TYPE_FILE,DELETE);
    if(dirNodeAndFileName.second == -1)
        return -1;
    INode & node = fs->inodes[dirNodeAndFileName.second];
    int inodeNumber =  checkValueInMap(node.pointers,dirNodeAndFileName.first,TYPE_FILE);*/
    if(inodeNumber < 0)
        return -1;
    if(checkMode(inodeNumber,mode) == -1)
        return -1;
    return createDescription(inodeNumber,mode);
}


// fd - deskryptor pliku,whence - liczba, której znaczenie zależy od wartości trzeciego parametru - trybu zmiany położenia w pliku.
/*
nowa pozycja jest pozycja bezwzględna, czyli liczona od poczatku pliku.
nowa pozycja jest liczona względem aktualnej pozycji w pliku.
nowa pozycja jest liczona względem końca pliku.
*/
int server::simplefs_lseek(int fd,int whence,int len)
{
    int filePosition = getFilePositionByFD(fd);
    if(filePosition == -1)
        return -1;
    int nodeNumber = getNodeNumberByFD(fd);
    if(nodeNumber == -1)
        return -1;

    if(whence == ACTUAL_POSITION)
    {
        if(filePosition + len > fs->inodes[nodeNumber].size)//gdy "bieżąca pozycja" przekroczy koniec pliku, to read() zwraca 0
            return -1;
        filePosition += len;
    }
    if(whence == END_POSITION)
    {
        if(fs->inodes[nodeNumber].size - len < 0)
            return -1;
        filePosition += fs->inodes[nodeNumber].size - len;
    }
    if(whence == START_POSITION)
    {
        if(len > fs->inodes[nodeNumber].size)
            return -1;
        filePosition = len;
    }
    fs->descriprionTable[fd - 1].filePosition = filePosition;
    return filePosition;
}

int server::simplefs_read(int fd,int len)
{
    int filePositionPrev = getFilePositionByFD(fd);
    if(filePositionPrev == -1)
        return -1;
    int newFilePosition = simplefs_lseek(fd,ACTUAL_POSITION,len);
    if(newFilePosition < 0)
        return -1;
    int nodeNumber = getNodeNumberByFD(fd);
    if(nodeNumber == -1)
        return -1;
    return fs->inodes[nodeNumber].address + filePositionPrev;//zwracam adres bloku pod ktorym sa dane oraz wskazuje na aktualna pozycje pod ktorym czytam dane
}
