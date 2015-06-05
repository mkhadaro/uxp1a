#ifndef SERVER_H
#define SERVER_H

#include "../include/shared_memory.h"
#include <iostream>
#include <map>

class server
{
    public:
        server();
        ~server();

        int_l createFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE], int type, int r, int w, int x);
        int deleteFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE]);
        int_l writeToFile(int_l inodeNumber, int_l size);


        void setNewInodeData(int_l inodeNumber, int type, int r, int w, int x);
        int_l findFreeBlockNumber(int_l blocksNeeded);
        void setBlockBit(int_l number, int_l amount, bool value);
        bool canAddToFile(int_l startingBlock, int_l amount);
        int_l findFreeInodeNumber();
        void setInodeBit(int_l number, bool value);

        void simplefs_mkdir(char* name);


        //private:
        int findFreeIdInode();

        int findBlockNumber(double size);
        int findInodeNumber();
        FileSystem* fs;
        FileSystem* attachSegmentOfSharedMemory();
        void detachSegmentOfSharedMemory(FileSystem* shared_memory);
        void createInode(char* name,int type,int nrInode);

        private:
            int checkName(char* name,int INODE_TYPE);
            int checkValueInMap(std::multimap<int,char*> maps,char* value);
            int checkValueCount(std::multimap<int,char*> maps,char* value);
            int updateLinksMapAndCreateFile(char* fileName,int INODE_TYPE,int dirNode);
};

#endif // SERVER_H
