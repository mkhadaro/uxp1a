#ifndef SERVER_H
#define SERVER_H

#include "../include/shared_memory.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include "../include/communication.h"

#include <iostream>
#include <map>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h> 
#include <unistd.h>


//struktura opakowujaca nazwe pliku do utworzenia lub usuniecia oraz katalog nadrzedny
typedef std::pair<char*, int> filesName;

class server
{
    public:
        server();
        ~server();

        void work();

        int_l createFile(char *name, int type, int r, int w, int x);
        int_l writeToFile(int_l inodeNumber, int_l size);

        int findBlockNumber(double size);
        int_l findFreeBlockNumber(int_l blocksNeeded);
        void setBlockBit(int_l number, int_l amount, bool value);
        bool canAddToFile(int_l startingBlock, int_l amount);

        int_l findFreeInodeNumber();
        void setNewInodeData(int inodeNumber, int type, int r, int w, int x,char* name);
        void setInodeBit(int number, bool value);

        int simplefs_mkdir(char* name);
        int simplefs_unlink(char* name);
        int simplefs_open(char* name,int mode);

        FileSystem* fs;

        private:
            FileSystem* attachSegmentOfSharedMemory();
            void detachSegmentOfSharedMemory(FileSystem* shared_memory);

            filesName checkName(char* name,int INODE_TYPE,int typeOfOperation);

            int checkValueInMap(int *maps,char* value,int TYP_INODE);
            int updateLinksMapAndCreateFile(int & dirNode);
            int updateLinksMapAndDeletePointer(filesName & fileStruct,int TYP_INODE);

            int createDescription(int & nrInode,int & mode);
            int getInodeNumber(char *name,int TYP_INODE,int file_type);
            int checkMode(int & nrInode,int & mode);
            int & getNodeNumberByFD(int & fd);

};

#endif // SERVER_H
