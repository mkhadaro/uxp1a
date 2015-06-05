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

    server.setBlockBit(0, 7, true);
    server.setBlockBit(12, 7, true);
    bool inode = server.canAddToFile(7, 5);
    printf(inode ? "true\n" : "false\n");

    return 0;
}
