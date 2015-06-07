#ifndef SERVER_H
#define SERVER_H

#include "../include/shared_memory.h"
#include <iostream>
#include <map>


//struktura opakowujaca nazwe pliku do utworzenia lub usuniecia oraz katalog nadrzedny
typedef std::pair<char*, int> filesName;

class server
{
    public:
        server();
        ~server();

        int_l createFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE], int type, int r, int w, int x);
        int deleteFile(char name[NAME_SIZE], char path[PATH_SIZE][NAME_SIZE]);
        int_l writeToFile(int_l inodeNumber, int_l size);


        void setNewInodeData(int_l inodeNumber, int type, int r, int w, int x,char* name);
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

        filesName & checkName(char* name,int INODE_TYPE,int type_of_operation);

        private:
            //filesName & checkName(char* name,int INODE_TYPE,int type_of_operation);
            int checkValueInMap(std::multimap<int,char*> maps,char* value);
            int checkValueCount(std::multimap<int,char*> maps,char* value);
            int returnSecondValueOfMaps(std::multimap<int,char*> maps,char* value,int anotherInode);
            filesName & updateLinksMapAndCreateFile(filesName & fileStruct,int INODE_TYPE);
            int updateLinksMapAndDeleteFile(filesName & fileStruct);

};

#endif // SERVER_H
