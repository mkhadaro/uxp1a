/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje
klasy sclienta
**/
#ifndef CLIENT_H
#define CLIENT_H

#include "communication.h"
#include "constant_definitions.h"
#include <stdio.h>
#include <iostream>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/shm.h>

class client
{
    public:
        client();
        ~client();

        /**API - black-box of clients*/
        void simplefs_open(char* name,int mode);
        void simplefs_unlink(char* name);
        void simplefs_mkdir(char* name);
        void simplefs_create(char* name,int r,int w,int x);
        void simplefs_read(int fd,char* buf,int len);
        void simplefs_write(int fd,char* buf,int len);
        void simplefs_lseek(int fd,int whence,int len);

        /**dodatkowe funkcjonalnosci do manipulacji na systemie plikow*/
        void close_file(int fd);
        void simplefs_list(char* path);
        void showFiles(INode *inode);

        /**funkcja do wysylania zapytan do serwera - dotyczy funkcji blokujących*/
        serverResponse sendRequest(int type, char path[128], int fd, int_l size, int mode);

        /**funkcje operujące na pamieci wspolzielonej*/
        FileSystem* attachSegmentOfSharedMemory();
        void detachSegmentOfSharedMemory(FileSystem* shared_memory);

        private:
            int fileDescription;
            char clientFifoId[24];
            FileSystem* fs;
};

#endif // CLIENT_H
