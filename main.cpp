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

void ls(server & server)
{
    for(int i = 0; i < INODE_COUNT ;++i)
    {
        if(server.fs->inodes[i].type != -1)
        {
            std::cout<<server.fs->inodes[i].name <<"\t"<<i<<"\t";
            showFiles(server,&server.fs->inodes[i]);
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
    server.simplefs_mkdir("/blin");
    server.simplefs_mkdir("/cabaj");
    server.simplefs_mkdir("/mam");
    server.simplefs_mkdir("/beke");

    server.simplefs_mkdir("/root");

    server.createFile("/pasha",TYPE_FILE, 1, 1, 1);

    server.createFile("/root/tut",TYPE_FILE, 1, 1, 1);
    server.createFile("/root/abs",TYPE_FILE, 1, 1, 1);
    server.simplefs_mkdir("/root/abs");
    server.simplefs_mkdir("/r/abs/k");
}

int testUnlink(server & server,char *nazwa)
{
    server.simplefs_unlink(nazwa);
}

int testOpenFile(server & server,char *name)
{
    std::cout<<"file description "<<server.simplefs_open(name,READ)<<std::endl;
}

int main(int arc,char** argv)
{
    server server;
    testCreateDir(server);
    ls(server);
    testUnlink(server,"/root/home");
    ls(server);
    server.simplefs_mkdir("/root/tut");
    ls(server);
    testOpenFile(server,"/root/tut");
    testUnlink(server,"/root/tut");
    ls(server);
    testOpenFile(server,"/root");

    return 0;
}
