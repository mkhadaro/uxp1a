#ifndef SERVER_H
#define SERVER_H

#include "../include/shared_memory.h"

class server
{
    public:
        server();
        ~server();

        int_l createFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE], int type, int r, int w, int x);
        int deleteFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE]);
        int_l writeToFile(int_l inodeNumber, int_l size);
        
        //private:
        FileSystem* fs;
        FileSystem* attachSegmentOfSharedMemory();
        void detachSegmentOfSharedMemory(FileSystem* shared_memory);

        void setNewInodeData(int_l inodeNumber, int type, int r, int w, int x);
        int_l findFreeBlockNumber(int_l blocksNeeded);
        void setBlockBit(int_l number, int_l amount, bool value);
        bool canAddToFile(int_l startingBlock, int_l amount);
        int_l findFreeInodeNumber();
        void setInodeBit(int_l number, bool value);
};

#endif // SERVER_H
