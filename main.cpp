#include <iostream>
#include "include/server.h"
#include "include/shared_memory.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <set>

using namespace std;

void showFiles(server & server,INode *inode)
{
    for(int j =0; j < sizeof(inode->pointers)/sizeof(int); ++j)
    {
        std::cout<<inode->pointers[j]<<" ";
        if(server.fs->inodes[inode->pointers[j]].type == TYPE_HELPER)
        {
                std::cout<<"\n\t"<<"\t";
                showFiles(server,&server.fs->inodes[inode->pointers[j]]);
        }
    }
}

int testCreateDir(server & server)
{
    server.simplefs_mkdir("/");

    server.simplefs_mkdir("/root");
    server.simplefs_mkdir("/opt");
    server.simplefs_mkdir("/run");
    server.simplefs_mkdir("/lusia");
    server.simplefs_mkdir("/ania");
    server.simplefs_mkdir("/blin");
    server.simplefs_mkdir("/cabaj");
    server.simplefs_mkdir("/mam");
    server.simplefs_mkdir("/beke");

    server.simplefs_mkdir("/root");

    //server.simplefs_mkdir("/root/abs");

    //server.simplefs_mkdir("/r/abs/k");

    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(server.fs->inodes[i].type != -1)
        {
            std::cout<<server.fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(server,&server.fs->inodes[i]);
            std::cout<<endl;
        }
    }
    /*
    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(server.fs->inodes[i].type != -1)
        {
            std::cout<<server.fs->inodes[i].name <<"\t"<<i<<"\t";
            for(int j =0; j < sizeof(server.fs->inodes[i].pointers)/sizeof(int);++j)
            {
                std::cout<<server.fs->inodes[i].pointers[j]<<" ";
            }
            std::cout<<endl;
        }
    }
    */

}


int main(int arc,char** argv)
{
    server server;
    testCreateDir(server);

    //server.setBlockBit(0, 7, true);
    //server.setBlockBit(12, 7, true);
    //bool inode = server.canAddToFile(7, 5);
    //printf(inode ? "true\n" : "false\n");

    return 0;
}
