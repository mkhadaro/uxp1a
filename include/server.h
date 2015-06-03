#ifndef SERVER_H
#define SERVER_H

#include "../include/shared_memory.h"

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

        void initDataInSharedMemory();
        
        //private:
        int findBlockNumber(double size);
        int findInodeNumber();
        FileSystem* fs;
        FileSystem* attachSegmentOfSharedMemory();
        void detachSegmentOfSharedMemory(FileSystem* shared_memory);
};

#endif // SERVER_H
