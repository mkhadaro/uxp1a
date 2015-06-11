#include <iostream>
#include "include/shared_memory.h"

#include "include/tests.h"
#include "include/server.h"
#include "include/client.h"
#include "include/interface.h"


using namespace std;

void showFiles(FileSystem * fs,INode *inode)
{
    for(int j =0; j < sizeof(inode->pointers)/sizeof(int); ++j)
    {
        std::cout<<inode->pointers[j]<<" ";
        if(fs->inodes[inode->pointers[j]].type == TYPE_HELPER)
        {
                std::cout<<"\n\t"<<"\t";
                showFiles(fs,&fs->inodes[inode->pointers[j]]);
        }
    }
}

void ls(FileSystem * fs)
{
    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(fs->inodes[i].type != -1)
        {
            std::cout<<fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(fs,&fs->inodes[i]);
            std::cout<<endl;
        }
    }
    std::cout<<endl;
}

int testCreateDir(server & server)
{
    server.simplefs_mkdir("/");
    server.simplefs_mkdir("/root");
    server.simplefs_mkdir("/opt");
    server.simplefs_mkdir("/run");
    server.simplefs_mkdir("/lusia");
    server.simplefs_mkdir("/ania");
    server.simplefs_mkdir("/param");
    server.simplefs_mkdir("/dom");
    server.simplefs_mkdir("/mam");
    server.simplefs_mkdir("/beke");

    server.simplefs_mkdir("/root");

    server.createFile("/pasha",TYPE_FILE, 1, 1, 1);

    server.createFile("/root/tut",TYPE_FILE, 1, 1, 1);
    server.createFile("/root/abs",TYPE_FILE, 1, 1, 1);
    server.simplefs_mkdir("/root/home");
    server.simplefs_mkdir("/root/home/ala");

}

int testUnlink(server & server,char *nazwa)
{
    server.simplefs_unlink(nazwa);
    return 0;
}

int testOpenFile(server & server,char *name)
{
    std::cout<<"file description "<<server.simplefs_open(name,READ)<<std::endl;
}

int testClient()
{
    client client;
    client.simplefs_create("/opt/system",1,1,0);
    client.simplefs_open("/opt/system",READ);
    std::cout<<std::endl;
    ls(client.fs);
}

int main(int argc,char** argv)
{

    server server;
    testCreateDir(server);
    ls(server.fs);
    testUnlink(server,"/root/abs");
    ls(server.fs);

    server.createFile("/opt/system",TYPE_FILE,1,1,0);

    int fd = server.simplefs_open("/opt/system",WRITE);

    if(fd == -1)
        return -1;

    int length = 10;
    int nodeNumber = server.getNodeNumberByFD(fd);
    char bufor2[10] = "kasialu";

    if(server.checkMode(nodeNumber,WRITE) < 0)
        return -1;

    int positionToWrite = server.writeToFile(nodeNumber,length);
    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor2[i];

    if(positionToWrite != -1)
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];

    positionToWrite = server.writeToFile(nodeNumber,length);

    //<-------------------------------------------------------------->
/*
    if(positionToWrite != -1)
    for(int i = 0; i < length; ++i)
            server.fs->dataBlocks[positionToWrite + i] = bufor2[i];
/*
    std::cout<<"poczatek po zapisie 2";
    for(int i = 0;i < length; ++i)
        std::cout<<server.fs->dataBlocks[positionToWrite + i ];
    */
    // <-------------------------------------------------------------->
    server.close(fd);
/*
    fd = server.simplefs_open("/opt/system",READ);
    if(fd == -1)
        return -1;

    char bufor[60] = {0};
    int blockNumber = server.simplefs_read(fd,length);
    if(blockNumber >= 0)
    for(int i = 0;i < length; ++i)
    {
        std::cout<<server.fs->dataBlocks[blockNumber + i ];
        bufor[i] = server.fs->dataBlocks[blockNumber + i ];
    }

    //for(int i = 0;i < length; ++i)
        //std::cout<<server.fs->dataBlocks[blockNumber + i ];
    std::cout<<"\nzawartosc bufora "<<bufor<<std::endl;
    */
    return 0;
}
