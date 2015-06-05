#include "../include/server.h"
#include <stdio.h>
#include <iostream>
#include <stdlib.h>
#include <string.h>
#include <map>
#include <set>

using namespace std;

int server::findBlockNumber(double size)
 {
 	int blocksNeeded = size / BLOCK_SIZE + 1;
 	int blocksFound = 0;
 	bool previousFree = false;
 	int blockNumber = -1;

 	printf("%d\n", blocksNeeded);
 	for(int i = 0; i < BLOCK_COUNT / 8; i++)
 	{
 		char check = fs->blockBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 			{
 				if(previousFree == false)
 					blockNumber = i * 8 + j;
 				previousFree = true;
 				blocksFound++;
 				if(blocksFound == blocksNeeded)
 					return blockNumber;

 			}
 			else
 			{
 				previousFree = false;
 				blocksFound = 0;
 			}
 			check >>= 1;
 		}
 	}
 	return -1;
 }

 int server::findInodeNumber()
 {
 	for(int i = 0; i < INODE_COUNT / 8; i++)
 	{
 		char check = fs->inodeBitmap[i];
 		for(int j = 0; j < 8; j++)
 		{
 			char bit = check & 1;
 			if(bit == 0)
 				return i * 8 + j;
 			check >>= 1;
 		}
 	}
 	return -1;
 }

 int server::findFreeIdInode()
 {
    for(int i = 0; i < INODE_COUNT ; ++i)
 	{
        INode inode = fs->inodes[i];

 		if(inode.type == FREE_INODE)
            return i;
 	}
 	return -1;
 }

int server::checkName(char* name,int INODE_TYPE)
{
    char *i =(char*) malloc(strlen(name)+1);
	i[strlen(name)] = 0;
	memcpy(i, name, strlen(name));

    char fileName[16] ;
    char dirName[16] ;

    for( int j = 0; j < sizeof(fileName); ++j)
    {
        fileName[ j ] = 0;
        dirName[ j ] = 0;
    }
    strcpy(i,name);

    if(i[0] !='/')
    {
        return -1;
    }

    fileName[0] = '/';
    dirName[0] = '/';

    INode* root_inode = &(fs->inodes[0]);

    if(strlen(name) == 1)
    {
        if(strcmp(root_inode->name,fileName) == 0)
        {
            printf("[ERROR]istnieje katalog główny\n");
            return -1;
        }
    }
    if(root_inode->type == FREE_INODE)
    {
        if(strcmp(name,"/"))
        {
            createInode(fileName,TYPE_DIR,findFreeIdInode());
            return 0;
        }
    }
    else
    {
        printf("[ERROR]nie istnieje katalog główny\n");
        return -1;
    }


    int j = 0;
    int isEnd = strlen(name);
    int nrInode = 0;
    int dirNode = 0;

    for(int l = 1; l!= strlen(name);++l)
    {
            if(i[l] =='/')
            {
                INode* inode = &(fs->inodes[nrInode]);
                nrInode = checkValueInMap(inode->mapsDirFile,fileName);
                if(nrInode == -1)
                {
                    printf("[ERROR]zle podana sciezka\n");
                    return -1;
                }
                inode = &(fs->inodes[nrInode]);

                if(inode->type != TYPE_DIR)
                {
                    printf("[ERROR]Taki plik/katalog juz istnieje w folderze\n");
                    return -1;
                }
                strcpy(dirName, fileName);

                for( int i = 0; i < sizeof( fileName); i++ )
                    fileName[ i ] = 0;
                j = 0;
                --isEnd;
                continue;
            }
            --isEnd;

            fileName[j] = i[l];
            ++j ;

            if(isEnd == 1)
            {
                INode* inode = &(fs->inodes[nrInode]);
                dirNode = nrInode;
                /*
                std::map<int,char*>::iterator it = inode->mapsDirFile.begin();
                for (; it != inode->mapsDirFile.end(); ++it )
                {
                    std::cout<<"value maps : "<<it->second<<endl;

                };*/
                int counterFiles = checkValueCount(inode->mapsDirFile,fileName);
                if(counterFiles == 2)
                {
                    printf("[ERROR]Taki plik/katalog juz istnieje w folderze\n");
                    return -1;
                }

                if(counterFiles == 1)
                {
                    nrInode = checkValueInMap(inode->mapsDirFile,fileName);
                    if(nrInode == -1)
                    {
                    printf("[ERROR]Taki plik juz istnieje w folderze\n");
                    return -1;
                    }
                    inode = &(fs->inodes[nrInode]);

                    if(inode->type == INODE_TYPE)
                        return -1;
                }
         }
    }
    return  updateLinksMapAndCreateFile(fileName,INODE_TYPE,dirNode);

}

int server::updateLinksMapAndCreateFile(char* fileName,int INODE_TYPE,int dirNode)
{
    INode* inode = &(fs->inodes[dirNode]);
    inode->pointers += 1;

    int nodeNumber = findFreeIdInode();

    char *file =(char*) malloc(strlen(fileName)+1);
	memcpy(file, fileName, strlen(fileName));
	strcpy(file,fileName);

    inode->mapsDirFile.insert(std::pair<int,char*>(nodeNumber,file));

    createInode(file,INODE_TYPE,nodeNumber);

    return nodeNumber;

}

int server::checkValueInMap(std::multimap<int,char*> maps,char* value)
{
/*
    std::map<int,char*>::const_iterator it = std::find_if(
        maps.begin(), maps.end(), boost::bind(&std::map<int,char*>::::value_type::second, _1) == value);
        return it;
*/
    std::map<int,char*>::iterator it;
    for (it = maps.begin(); it != maps.end(); ++it )
    {
        if (strcmp((*it).second ,value) == 0)
        {
            return it->first;
        }
    }
    return -1;
}

int server::checkValueCount(std::multimap<int,char*> maps,char* value)
{

    std::map<int,char*>::iterator it = maps.begin();
    int counter = 0;
    for (; it != maps.end(); ++it )
    {
                if (strcmp((*it).second ,value) == 0)
                ++counter;
    }

    return counter;
}
