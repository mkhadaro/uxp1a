#include <iostream>
#include "include/server.h"
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <set>

using namespace std;

int testCreateDir(server & server)
{
    server.simplefs_mkdir("/");
    server.simplefs_mkdir("/root");

    server.simplefs_mkdir("/kasia");

    server.simplefs_mkdir("/root/abs");

    server.simplefs_mkdir("/root/asia");
    server.simplefs_mkdir("/r/abs/lusia");
    server.simplefs_mkdir("/root/abs");

    server.simplefs_mkdir("/");

    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(server.fs->inodes[i].type != -1)
        std::cout<<server.fs->inodes[i].name <<" "<<i<<" "<<server.fs->inodes[i].pointers<<std::endl;
    }
    int key = server.fs->inodes[1].mapsDirFile.count(4);
    std::cout<<key<<std::endl;

}

int main(int arc,char** argv)
{
    server server;

    testCreateDir(server);

    //server.fs->blockBitmap[0] = 0xFF;
    //server.fs->blockBitmap[1] = 0xFF;
    //int block = server.findBlockNumber(10001);
    //printf("numer bloku: %d \n", block);

    //server.fs->inodeBitmap[2] = 0x3;
    //int inode = server.findInodeNumber();
    //printf("numer inoda: %d\n", inode);

    //printf("%c\n",server.fs->inodeBitmap[0]);
    return 0;
}
