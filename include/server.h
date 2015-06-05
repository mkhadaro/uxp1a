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

        void simplefs_open(char* name,int mode);
        void simplefs_unlink(char* name);
        void simplefs_mkdir(char* name);
        void simplefs_create(char* name,int mode);
        void simplefs_read(int fd,char* buf,int len);
        void simplefs_write(int fd,char* buf,int len);
        void simplefs_lseek(int fd,int whence,int len);

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
