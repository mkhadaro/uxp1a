/**
<--Opis pliku -->
Plik naglowkowy zawiera definicje
klasy serwera
**/

#ifndef SERVER_H
#define SERVER_H

#include "shared_memory.h"
#include "communication.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


/**
struktura pomocnicza opakowujaca
1) char*    -- nazwe pliku do utworzenia/usuniecia po rozbiorze sciezki na tokeny
   int      -- oraz inode katalogu nadrzedny
*/
typedef std::pair<char*, int> filesName;

class server
{
    public:
        server();
        ~server();

        void work();

        int createFile(char *name, int type, int r, int w, int x);
        int_l writeToFile(int_l inodeNumber, int_l size);

        int findBlockNumber(double size);
        int_l findFreeBlockNumber(int_l blocksNeeded);
        void setBlockBit(int_l number, int_l amount, bool value);

        int_l findFreeInodeNumber();
        void setNewInodeData(int inodeNumber, int type, int r, int w, int x,char* name,int adress);
        void setInodeBit(int number, bool value);

        int simplefs_mkdir(char* name);
        int simplefs_unlink(char* name);
        int simplefs_open(char* name,int mode);
        int simplefs_read(int fd,int len);
        int simplefs_lseek(int fd,int whence,int len);
        int close(int & fd);
        void printfFreeDataTable(int adressBloku);

        FileSystem* fs;

        void showServerState();
        void printFreeBlockBitmap();
        void printFreeInodeBitmap();
        void printDirectories(int_l inodeNumber, int depth);
        void printfFileDescriptors();
        int getInodeNumber(char *name,int TYP_INODE,int file_type);
        int checkMode(int & nrInode,int mode);

        int getNodeNumberByFD(int & fd);

        int searchFreeBlock(int size);
        void deleteBlock(int address,int size);

        private:
            FileSystem* attachSegmentOfSharedMemory();
            void detachSegmentOfSharedMemory(FileSystem* shared_memory);
            filesName checkName(char* name,int INODE_TYPE,int typeOfOperation);
            int checkValueInMap(int *maps,char* value,int TYP_INODE);
            int updateLinksMapAndCreateFile(int & dirNode);
            int updateLinksMapAndDeletePointer(filesName & fileStruct,int TYP_INODE);
            int createDescription(int & nrInode,int & mode);
            int getFilePositionByFD(int & fd);

};
#endif // SERVER_H
