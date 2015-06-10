#ifndef CLIENT_H
#define CLIENT_H

#include <iostream>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#include "../include/communication.h"


class client
{
    public:
        client();
        ~client();

        void simplefs_open(char* name,int mode);
        void simplefs_unlink(char* name);
        void simplefs_mkdir(char* name);
        void simplefs_create(char* name,int r,int w,int x);
        void simplefs_read(int fd,char* buf,int len);
        void simplefs_write(int fd,char* buf,int len);
        void simplefs_lseek(int fd,int whence,int len);
        void close_file(int fd);
        void simplefs_list(char* path);

      // private:
        char clientFifoId[24];
        FileSystem* fs;

        serverResponse sendRequest(int type, char path[128], int fd, int_l size, int mode);
        void showFiles(INode *inode);
        FileSystem* attachSegmentOfSharedMemory();
        void detachSegmentOfSharedMemory(FileSystem* shared_memory);

        private:
            int fileDescription;

};

#endif // CLIENT_H
