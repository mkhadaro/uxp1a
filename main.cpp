#include <iostream>
#include "include/server.h"
#include <stdlib.h>
#include <stdio.h>

using namespace std;

int main(int arc,char** argv)
{
    server server;
    //sever.initDataInSharedMemory();
    //int size = sizeof(server.fs->inodes);
    server.fs->blockBitmap[0] = 0xFF;
    server.fs->blockBitmap[1] = 0xFF;
    int block = server.findBlockNumber(10001);
    printf("numer bloku: %d \n", block);

    server.fs->inodeBitmap[0] = 0x3;
    int inode = server.findInodeNumber();
    printf("numer inoda: %d\n", inode);
    //printf("%c\n",server.fs->inodeBitmap[0]);
    return 0;
}
